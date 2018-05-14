//
//  IndexBuilder.cpp
//  IR
//
//  Created by User on 2/11/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <algorithm>
#include <regex>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include "IndexBuilder.h"
#include "Dedup.h"
#include "WordAndPositions.h"
#include "VBOutputIterator.h"
#include "VBGapList.h"
#include "IndexIterator.h"
#include <boost/range/algorithm/merge.hpp>
#include "IndexMerging.h"
#include "utils.h"
#include "TestingFunctions.h"
#include <boost/locale.hpp>


using namespace std;
using namespace IndexBuilding;

IndexBuilder::IndexBuilder(
                           std::string const databasePath,
                           std::string const indexFilePath,
                           std::string const base
                           )
: _dedup(
         new Dedup<WordAndPositions*>(
                                      wordAndPositionsEquals,
                                      wordAndPositionsLess,
                                      [] (WordAndPositions * & a,
                                          WordAndPositions * const & b)
                                      { a->merge(b); }
                                      )
         )
, _words(1000000)
, _index(new Index(databasePath))
, _databasePath (databasePath)
, _indexFilePath (indexFilePath)
, _base(base)
{
}

IndexBuilder::~IndexBuilder()
{
    delete _dedup;
}


std::string
IndexBuilder::getWordsFilePath(
                               docid_t id)
{
    ostringstream res;
    res << _databasePath << "/pos/" << id << ".wrdps";
    return res.str();
}

void
IndexBuilder::saveWordPositionsToFile(docid_t id, std::vector<WordAndPositions*> const & words) {
    char* arr = new char[9];
    auto_ptr<char> arr_deleter (arr);
    
    ofstream wpf (getWordsFilePath(id));
    ostreambuf_iterator<char> char_out (wpf);
    for (auto w : words) {
        if (w->word().length() <= 255) {
            wpf << static_cast<unsigned char>(w->word().length());
            // what a subtle bug!! << '\n';
            wpf << w->word() /* << '\n' */;
            VBOutputIterator<ostreambuf_iterator<char>, position_t, false> num_out (char_out);
            position_t largest (0);
            for (position_t pos: w->positions()) {
                assert(largest < pos);
                num_out = (pos - largest);
                largest = pos;
            }
            num_out.terminate();
        } else {
            cerr << "some word is longer than 255, "
                << "so we don't write it" << '\n';
        }
    }
}

//ofstream all_words_captured("all_words_captured.txt"); // TODO:
//ofstream all_words("all_words.txt");

void
IndexBuilder::indexFile(std::string const& filePath) {
    using namespace std;
    using namespace std::regex_constants;
//    static auto const regexpr = regex("[А-ЯІЇЄа-яіїєa-z]+", ECMAScript|icase);
    ifstream file (_base + filePath);
    unsigned wordCount (0);
    string word;
    _words.clear();
    
    ofstream deb(_databasePath + "/debug.txt");
    string line;
    
    string regex_part = u8"[\\wа-яїєь]+";
    string regex_full = regex_part + "'?" + regex_part;
    regex re(regex_full);
    while (getline(file, line)) {
//        line = boost::locale::to_lower(line);
        boost::algorithm::to_lower(line);
        sregex_iterator next (line.begin(), line.end(), re);
        sregex_iterator end;
        while (next != end) {
            string word = next->str();
            if (word.size() < 255) {
                deb << word << '\n';
                ++wordCount;
                auto wordPos = new WordAndPositions(word, ++wordCount);
                _words.push_back(wordPos);
            }
            ++next;
        }
    }

    size_t new_size = (wordCount > 0) ? _dedup->sortDedup(&_words[0], 0, wordCount) : 0;
    _words.resize(new_size);
    
}

void IndexBuilder::loadIndex() {
    _index->loadIndexes();
}

void
IndexBuilder::index(string filePaths, size_t firstLineToProcess, size_t lastLineToProcess) {
    size_t const merging_treshold (100000);
    
    ifstream filePathsFile (filePaths);
    ofstream indexedFiles (_indexFilePath);
    assert(filePathsFile);
    string path;
    
    docid_t ID = firstLineToProcess;
    
    // the in-memory index
    map<string, VBGapList<docid_t> > wordToIDMap;
    
    size_t first_idx_word_counter(0);
    size_t numberOfLinesProcessed = 0;

    for ( ; numberOfLinesProcessed != firstLineToProcess; ++numberOfLinesProcessed)
        getline(filePathsFile, path);
    
    while (numberOfLinesProcessed <= lastLineToProcess and getline(filePathsFile, path)) {
        cout << path << '\n';
        ++numberOfLinesProcessed;
        // fill _words with words/positions
        indexFile(path);
        
        // write it to database
        // come up with an ID
        docid_t id = ++ID;
        indexedFiles << id << ' ' << path << '\n';
        saveWordPositionsToFile(id, _words);
        
        first_idx_word_counter += _words.size();
        for (WordAndPositions * w: _words) {
            wordToIDMap[w->word()].push_back(id);
            delete w;
        }
        if (first_idx_word_counter > merging_treshold) {
            string first_index = mapIndex2StringIndex(wordToIDMap);
#ifndef NDEBUG
            if (not indexValid(first_index, wordToIDMap)) {
                { ofstream("idxThatCausedException") << first_index; }
                writeReadableIndexToFile(wordToIDMap, "mapThatCausedException.txt");
                mapIndex2StringIndex(wordToIDMap);
            }
#endif
            wordToIDMap.clear();
            first_idx_word_counter = 0;
            _index->addStringIndex(move(first_index));
        }
    }
    
    {
        // add the last index
        string first_index = mapIndex2StringIndex(wordToIDMap);
#ifndef NDEBUG
        if (not indexValid(first_index, wordToIDMap)) {
            { ofstream("idxThatCausedException") << first_index; }
            writeReadableIndexToFile(wordToIDMap, "mapThatCausedException.txt");
            mapIndex2StringIndex(wordToIDMap);
        }
#endif
        wordToIDMap.clear();
        first_idx_word_counter = 0;
        _index->addStringIndex(move(first_index));
        ofstream(_databasePath + "/lastLine.txt") << numberOfLinesProcessed;
    }

//    _index->saveIndexes();
//    _index->saveReadableIndexes();
//    cout << "almost end\n";
//    {
//        string search_word = "argument";
//        _index->execForEveryDocId(search_word, [&] (docid_t d) {
//            cout << "df __in that particular index" << to_string(_index->indexInArray()) << ", unftntly__ for " << search_word << ": "
//                << _index->documentFrequencies()[_index->indexInArray()][_index->indexOfWordInPositionsAndFrequenciesVectors()] << '\n';
//            cout << d << ' ';
//        });
//    }
    cout << "end" << '\n';
    cout << first_idx_word_counter;
}

bool IndexBuilder::indexMoreLines(std::string file, size_t numberOfLinesToIndex) {
    size_t indexedLinesNum;
    ifstream fin (_databasePath + "/lastLine.txt");
    assert(fin);
    fin >> indexedLinesNum;
    index(file, indexedLinesNum, indexedLinesNum + numberOfLinesToIndex);
    
    return false; // TODO: it must return whether
    // the file with lines is exhausted
}

// always write custom typedefs instead of integers