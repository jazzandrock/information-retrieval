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
#include <boost/algorithm/cxx11/copy_if.hpp>
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

using namespace std;
using namespace IndexBuilding;

IndexBuilder::IndexBuilder(
                           std::string const & databasePath,
                           std::string const & indexFilePath,
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
    static auto const regexpr = regex("[А-Яа-яa-z]+", ECMAScript|icase);
    ifstream file (_base + filePath);
    unsigned wordCount (0);
    string word;
    _words.clear();
    while (file >> word) {
//        all_words << word << '\n';
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (word == "Олежка") {
            cout << "їїїїЄ";
        }
        if (/* DISABLES CODE */ (false) or (word.length() < 255  and  regex_match(word, regexpr))) {
//            all_words_captured << word << '\n';
            auto wordPos = new WordAndPositions(word, ++wordCount);
            _words.push_back(wordPos);
        }
    }
    size_t new_size = (wordCount > 0) ? _dedup->sortDedup(&_words[0], 0, wordCount) : 0;
    _words.resize(new_size);
}

void IndexBuilder::loadIndex() {
    _index->loadIndexes();
}

void
IndexBuilder::index(string filePaths) {
    size_t const merging_treshold (100000);
    
    ifstream filePathsFile (filePaths);
    ofstream indexedFiles (_indexFilePath);
    
    string path;
    
    docid_t ID = 0;
    
    // the in-memory index
    map<string, VBGapList<docid_t> > wordToIDMap;
    
//    static int n (0);
    size_t first_idx_word_counter(0);
    while (getline(filePathsFile, path)) {
        cout << path << '\n';
        // fill _words with words/positions
        indexFile(path);
        
        // write it to database
        // come up with an ID
        docid_t id = ++ID;
        indexedFiles << id << ' ' << path << '\n';
        saveWordPositionsToFile(id, _words);
        
        first_idx_word_counter += _words.size();
        for (auto w: _words) {
            wordToIDMap[w->word()].push_back(id);
            delete w;
        }
        if (first_idx_word_counter > merging_treshold) {
            string first_index = mapIndex2StringIndex(wordToIDMap);
            if (not indexValid(first_index, wordToIDMap)) {
                { ofstream("idxThatCausedException") << first_index; }
                writeReadableIndexToFile(wordToIDMap, "mapThatCausedException.txt");
                mapIndex2StringIndex(wordToIDMap);
            }
            
//            string indexFileName = "idx" + to_string(++n);
//            { ofstream(indexFileName) << first_index; }
//            writeReadableIndex(wordToIDMap, "readable" + indexFileName);
//            auto map2 = readIndex(indexFileName);
//            writeReadableIndex(map2, "readable" + indexFileName + "second");
            wordToIDMap.clear();
            first_idx_word_counter = 0;
            _index->addStringIndex(move(first_index));
        }
    }
    
    {
        // add the last index
        string first_index = mapIndex2StringIndex(wordToIDMap);
        if (not indexValid(first_index, wordToIDMap)) {
            { ofstream("idxThatCausedException") << first_index; }
            writeReadableIndexToFile(wordToIDMap, "mapThatCausedException.txt");
            mapIndex2StringIndex(wordToIDMap);
        }
        
//            string indexFileName = "idx" + to_string(++n);
//            { ofstream(indexFileName) << first_index; }
//            writeReadableIndex(wordToIDMap, "readable" + indexFileName);
//            auto map2 = readIndex(indexFileName);
//            writeReadableIndex(map2, "readable" + indexFileName + "second");
        wordToIDMap.clear();
        first_idx_word_counter = 0;
        _index->addStringIndex(move(first_index));
    }
//    n = 13;
//    string idx = copyFileIntoString("idx1");
//    for (int i = 2; i <= n; ++i) {
//        string currIdx = copyFileIntoString("idx" + to_string(i));
//        cout << "currIdx len: " << currIdx.size() << '\n';
//        cout << "merging\n";
//        string idxCopy = idx;
//        idx = getMergedIndex(idxCopy, currIdx);
//        { ofstream("mergedIdx" + to_string(i)) << idx; }
//    }
//    
//    auto finalIdxMap = readIndex("mergedIdx13");
//    writeReadableIndex(finalIdxMap, "readableMergedIdx13");
//    for_each(indexes().begin(), indexes().end(), []

//    {
//        string out;
//        size_t n = 0;
//        for (string const & idx : indexes()) {
//            if (idx.size() == 0) continue;
//            out = getMergedIndex(idx, out);
//            string indexPath = _databasePath + "/idx/" + to_string(n) + ".index";
//            { ofstream (indexPath) << idx; }
//             writeReadableIndexToFile(readIndexFromFile(indexPath), _databasePath + "/idx/" + to_string(n) + ".readableindex");
//            
//            ++n;
//        }
//        { ofstream (_databasePath + "/idx/mergedIndex.index") << out; }
//         writeReadableIndexToFile(readIndexFromFile(_databasePath + "/idx/mergedIndex.index"), _databasePath + "/idx/mergedIndex.readableindex");
//    }
    _index->saveIndexes();
    cout << "almost end\n";
    {
        string search_word = "argument";
        _index->execForEveryDocId(search_word, [&] (docid_t d) {
            cout << "df __in that particular index" << to_string(_index->indexInArray()) << ", unftntly__ for " << search_word << ": "
                << _index->documentFrequencies()[_index->indexInArray()][_index->indexOfWordInPositionsAndFrequenciesVectors()] << '\n';
            cout << d << ' ';
    //        cout << "їїїї for latin characters\n";
    //        cout << "їїїї для наших букв\n";
        });
    }
    cout << "end" << '\n';
    cout << first_idx_word_counter;
}


// always write custom typedefs instead of integers