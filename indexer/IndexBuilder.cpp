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

void
IndexBuilder::saveWordPositionsToFile(docid_t id, std::vector<WordAndPositions*> const & words)
    {
    char* arr = new char[9];
    auto_ptr<char> arr_deleter (arr);
    
    ofstream wpf (getWordsFilePath(id));
    ostreambuf_iterator<char> char_out (wpf);
    for (auto w : words)
        {
        if (w->word().length() <= 255)
            {
            wpf << static_cast<unsigned char>(w->word().length());
            // what a subtle bug!! << '\n';
            wpf << w->word() /* << '\n' */;
            VBOutputIterator<ostreambuf_iterator<char>, position_t, false> num_out (char_out);
            position_t largest (0);
            for (position_t pos: w->positions())
                {
                assert(largest < pos);
                num_out = (pos - largest);
                largest = pos;
                }
            num_out.terminate();
            }
        else
            {
            std::cerr << "some word is longer than 255, "
                << "so we don't write it" << '\n';
            }
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
        if (/* DISABLES CODE */ (false) or (std::regex_match(word, std::regex("[a-z]+")) && word.length() < 255))
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

template <class OutputIterator>
void saveWordsToIterator(
    map<string, VBGapList<docid_t> > const & wordToIdMap,
    OutputIterator & out)
    {
    for (auto const & string_pair_VBGapList : wordToIdMap)
        {
        assert(string_pair_VBGapList.first.length() <= 255);
        *out++ = static_cast<char>(string_pair_VBGapList.first.length());
        // q: if I write to first_idx, will ostreambuf_iterator update its position?
        // a: yes it will
        copy(string_pair_VBGapList.first.begin(), string_pair_VBGapList.first.end(), out);
        boost::algorithm::copy_until(
            string_pair_VBGapList.second.inner_data().begin(),
            string_pair_VBGapList.second.inner_data().end(),
            out,
            [] (char c) { return static_cast<unsigned char>(c) == 128; });
        *out++ = 128;
        }
    }
void saveWordIdsToFile(
    map<string, VBGapList<docid_t> > const & wordToIdMap,
    string const & filePath )
    {
    std::ofstream file_ofstream(filePath);
    std::ostreambuf_iterator<char> file_charout (file_ofstream);
    saveWordsToIterator(wordToIdMap, file_charout);
    }

void
IndexBuilder::index(string filePaths)
    {
    size_t const merging_treshold (1000);
    
    ifstream filePathsFile (filePaths);
    ofstream indexedFiles (_indexFilePath);
        
    string path;

    docid_t ID = 0;
    
    // the in-memory index
    map<string, VBGapList<docid_t> > wordToIDMap;
    
    size_t first_idx_word_counter(0);
    while (getline(filePathsFile, path))
        {
        std::cout << path << '\n';
        // fill _words with words/positions
        indexFile(path);
        
        // write it to database
        // come up with an ID
        docid_t id = ++ID;
        indexedFiles << id << ' ' << path << '\n';
        saveWordPositionsToFile(id, _words);
        
        first_idx_word_counter += _words.size();
        for (auto w: _words)
            {
            // woow
            // i just spent so much time finding a bug in main
            // and it turned out to be here
            // no stacktrace were available.
            // how to never search for a bug for that long again?
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
            
            wordToIDMap[w->word()].push_back(id);

            delete w;
            }
            
        if (first_idx_word_counter > merging_treshold)
            {
            // merge some indexes
            // we need to know what indexes we have.
            // we can find them in a folder
            
            // writeIndexToFile(wordToID.begin(), wordToID.end())
            // we need to come up with filename. It will be 1.index
            // mergeIfNeeded(); we need merging only if file 1.index exists
            // so the technology is:
            
            // how to merge indexes?
            // first determine how indexes are stored.
            // probably word and positions as always.
            // maybe write the process of writing to file as a distinct object/funciton?
            // we give it an iterator and a way of retrieving information,
            // and it writes it to file.

            // OR
            // I could write a method to "serialize" a list
            // and then write word and
            
            
            {
            using namespace boost::filesystem;
            current_path(_databasePath);
            if (exists("_1.worddocs"))
                {
                throw std::runtime_error("there must not be _1.worddocs.");
                }

            // by the way, these indexes must be in memory!
            // because how otherwise you will search them?
            // moreover, as you remember, indexes don't take
            // too much memory when compressed.
            // of course you will be able to save them
            // and load back and even maybe some sort of
            // searching from the disk
            //

            // write map to file
            // i'll add byte iterator to my lists
            saveWordIdsToFile(wordToIDMap, _databasePath + "/_1.worddocs");
            wordToIDMap.clear();
            first_idx_word_counter = 0;

            // construct the first index!!
            string first_index;
            ostringstream first_index_fill (first_index); // ahaha
            // q: what is faster: writing rdbuf() or not?
            // a: no difference
            ostreambuf_iterator<char> first_index_fill_iter (first_index_fill/*.rdbuf()*/);
            saveWordsToIterator(wordToIDMap, first_index_fill_iter);
            first_index.shrink_to_fit();
            IndexIterator<string::iterator, docid_t> i;
            // yeah nigga constructed!!
            // now what. merge?
            // to merge, i first need to compare strings and then
            // to iterate all the numbers, not in binary format.
            
            // iterator
            // use cases:
            // take string so merge
            

            // and how do I store indexes?
            // sort of an array of strings.
            // but what if these strings are necessarily copied when
            // assigned?
            // i need to take two strings (as references)
            // take iterator for worddocs,
            // and merge it into another string, the third one.
            // then I have to assign it to some index in my array.
            // it can't be copied!!
            //
//            string s1 ("asdasddaadasdasads");
//            string s2;
//            s2.assign(std::move(s1));
            // looks like there's some solution.
            // ok, then vector of strings
            // somehow we need to denote empty strings:: length() == 0
            
            
            
            // writes it as _1.worddocs
            for (unsigned i = 0; ; ++i)
                {
                if (indexExists(i))
                    {
                    // merge _i, i to _i+1.worddocss
                    string index_dest;
                    ostringstream index_dest_fill (index_dest);
                    }
                else
                    {
                    // rename _i.worddocs to i.worddocs
                    break;
                    }
                }



            }
            
            // OR
            // get current "bit representation" of indexes
            // get next bit representation
            // basically
            // get first indexes in a row
            // complexities:
            // let we have m consequtive indexes
            // m1 = 2m0, m2=2m1, etc
            // so that every
            // size of the least two ones: s
            // 2s + 4s + 8s + 2^ms = (2^(m+1)-1) s)
            // S = 2^(m-1)s
            
            // in terms of the biggest one:
            // let S = size(m)
            // then there are also size(m-1), size(m-2)
            // let's enumerate
            // if m = 1, then complexity is 2size(m)
            // if m = 2, then 2size(m-1) + 2size(m)
            // general: 4size(m)
            // — WOW LINEAR TIME —

            // and merge them in the next one
            // we will always have the first one here,
            // so just search for 2.idx, 3.idx, 4.idx,
            // while n.idx is not present. then merge 'em all to n.idx
            // no need to rename anything
            // what the interface of iterators?
            // it will be WordAndDocuments objects
            // size(m)lg(m)
            // although what works faster in reality is a question
            }
        }
    
    std::cout << "end" << '\n';
    std::cout << first_idx_word_counter;
    }

// always write custom typedefs instead of integers