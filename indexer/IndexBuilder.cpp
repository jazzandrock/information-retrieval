//
//  IndexBuilder.cpp
//  IR
//
//  Created by User on 2/11/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <forward_list>
#include <map>
#include "boost/filesystem.hpp"
#include "IndexBuilder.h"
#include "Dedup.h"
#include "WordAndPositions.h"
#include "ByteArrOutputIterator.h"

using namespace std;
using namespace IndexBuilding;

IndexBuilder::IndexBuilder(
    std::string const & databasePath,
    std::string const & indexFilePath
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
    , _databasePath (databasePath)
    , _indexFilePath (indexFilePath)
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
    res << _databasePath << '/' << id << ".wrdps";
    return res.str();
    }

//std::string
//IndexBuilder::getWordPositionsFilePath(
//    docid_t id,
//    std::string const & word)
//    {
//    ostringstream res;
//    res << _databasePath << '/';
//    for ( size_t i=0, l=min<size_t>(3, word.length())
//         ; i!=l; ++i)
//        {
//        res << word[i] << '/';
//        }
//    res << id << '_' << word << ".txt";
//    return res.str();
//    }


size_t
fillArrayWithNumber(
    unsigned number,
    char * arr)
    {
    auto i = arr;
    do {
        auto n = static_cast<unsigned char>(number & 127);
        number >>= 7;
        if (number == 0) n |= 128;
        *i++ = n;
        } while (number);
    
//    // reverse
//    auto s = arr;
//    auto e = i - 1;
//    while (s < e)
//        swap(*s, *e);
    
    return i - arr;
    }

// changes the array position
// so that you can know the size later
unsigned
readNumberFromArr(char const * & arr)
    {
    unsigned res (0);
    while ((*arr & 128) == 0)
        {
        res |= *arr;
        res <<= 7;
        ++arr;
        }
    res |= (*arr & 127);
    return res;
    }

void
IndexBuilder::writeToDatabase(docid_t id)
    {
    using namespace boost;
    char* arr = new char[9];
    auto_ptr<char> arr_deleter (arr);
    
    ofstream wpf (getWordsFilePath(id));
    ostreambuf_iterator<char> char_out (wpf);
    for (auto w : _words)
        {
        if (w->word().length() > 255)
            {
            std::cerr << "some word is longer than 255, "
                << "so we don't write it" << '\n';
            }
        wpf << static_cast<unsigned char>(w->word().length());
        // what a subtle bug!! << '\n';
        wpf << w->word() /* << '\n' */;
        
        vector<char> v;
        v.reserve(100000);
        char* arr_iter_ = &v[0];
        ByteArrOutputIterator<char*> num_out (arr_iter_);
        // write the words
        for (unsigned pos: w->positions())
            {
            *num_out++ = pos;
//            size_t end = fillArrayWithNumber(pos, arr);
//            while (end --> 0)
//                {
//                wpf << arr[end];
//                }
            }
        std::copy(&v[0], arr_iter_ + 1, char_out);
//        wpf << static_cast<char>(128);
        }
    }

void
IndexBuilder::indexFile(std::string filePath)
    {
    ifstream file (filePath);
    unsigned wordCount (0);
    string word;
    _words.clear();
    while (file >> word)
        {
        if (std::regex_match(word, std::regex("[a-z]+")) && word.length() < 255)
            {
            auto wordPos = new WordAndPositions(word, ++wordCount);
            _words.push_back(wordPos);
            }
        }
    size_t new_size = (wordCount > 0)
        ? _dedup->sortDedup(&_words[0], 0, wordCount)
        : 0;
    _words.resize(new_size);
    }

void
IndexBuilder::index(string filePaths)
    {
    ifstream filePathsFile (filePaths);
    ofstream files ( _indexFilePath);
        
    string path;

    docid_t ID = 0;
    
    // the in-memory index
    map<string, vector<docid_t> > wordToID;
    
    unsigned counter(0);
    while (getline(filePathsFile, path))
        {
        std::cout << path << '\n';
        // fill _words with words/positions
        indexFile(path);
        
        // write it to database
        // come up with an ID
        docid_t id = ++ID;
        files << id << ' ' << path << '\n';
        writeToDatabase(id);
        
        counter += _words.size();
        for (auto w: _words)
            {
            // woow
            // i just spent so much time finding a bug in main
            // and it turned out to be here
            // no stacktrace were available.
            // how to never search for a bug that long again?
            // shet i donnow
            
            // add another id to word --> ids
            // or, more precisely, distance from positions
            
            // we need iterators of wordToId
            // and indexes to be consistent
            // you don't store gaps between ids
            // you only can write your own data structure
            // that lets you forward iterate on it
            // which you actually need to write
            // so write it.
            
            wordToID[w->word()].push_back(id);

            delete w;
            }
            
        if (counter > 100000)
            {
            // merge some indexes
            // we need to know what indexes we have.
            // we can find them in a folder
            
            // writeIndexToFile(wordToID.begin(), wordToID.end())
            // we need to come up with filename. It will be 1.index
            // mergeIfNeeded(); we need merging only if file 1.index exists
            // so the technology is:
            
            wordToID.clear();
            counter = 0;
            }
        }
    
    
    std::cout << "end" << '\n';
    std::cout << counter;
    }

