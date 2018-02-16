//
//  IndexBuilder.cpp
//  IR
//
//  Created by User on 2/11/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <forward_list>
#include "boost/filesystem.hpp"
#include "IndexBuilder.h"
#include "../sorting/Dedup.h"

using namespace std;
using namespace IndexBuilding;

class
WordAndPositions
    {
    friend
    void
    mergeWordAndPositions(
        WordAndPositions * & a,
        WordAndPositions *const & b);
        
    public:
    WordAndPositions(
        string const & word,
        unsigned position
        )
        : _word(word)
        , _positions(1, position)
        {
        ++freeID;
        }
    
    ~WordAndPositions()
        {
        ++deletedNum;
        }
    
    string const &
    word() const
        {
        return _word;
        }

    forward_list<unsigned> const &
    positions() const
        { return _positions; }
        
//    forward_list<unsigned> &
//    positions()
//        { return _positions; }
        

    bool
    operator< (
        WordAndPositions const & b) const
        {
        return _word.compare(b._word) < 0;
        }
        
    bool
    operator== (
        WordAndPositions const & b) const
        {
        return _word.compare(b._word) == 0;
        }

    static size_t createdCount() { return freeID; }
    static size_t deletedCount() { return deletedNum; }
    
    private:
    static size_t freeID;
    static size_t deletedNum;
    std::string _word;
    std::forward_list<unsigned> _positions;
    };

    size_t WordAndPositions::freeID = 0;
    size_t WordAndPositions::deletedNum = 0;

// end class WordAndPositions

    std::ostream&
    operator<< (
        std::ostream & stream,
        WordAndPositions const & wordPositions)
        {
        stream << "WordAndPositions: " << wordPositions.word() << '\n';
        stream << "positions: ";
        for (unsigned pos: wordPositions.positions())
            {
            stream << pos << ' ';
            }
        stream << std::endl;
        return stream;
        }

    bool
    wordAndPositionsLess(
        WordAndPositions *const & a,
        WordAndPositions *const & b)
        {
        return *a < *b;
        }
    
    bool
    wordAndPositionsEquals(
        WordAndPositions *const & a,
        WordAndPositions *const & b)
        {
        return *a == *b;
        }


    void
    mergeWordAndPositions(
        WordAndPositions * & a,
        WordAndPositions * const & b)
        {
        a->_positions.merge(b->_positions);
        delete b;
        }


IndexBuilder::IndexBuilder(
    std::string const & databasePath,
    std::string const & indexFilePath
    )
    : _dedup(
        new Dedup<WordAndPositions*>(
            wordAndPositionsEquals,
            wordAndPositionsLess,
            mergeWordAndPositions
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


std::string IndexBuilder::getWordsFilePath(
    unsigned long long id)
    {
    ostringstream res;
    res << _databasePath << '/' << id << "_words.txt";
    return res.str();
    }

std::string IndexBuilder::getWordPositionsFilePath(
    unsigned long long id,
    std::string const & word)
    {
    ostringstream res;
    res << _databasePath << '/';
    for ( size_t i=0, l=min<size_t>(3, word.length())
         ; i!=l; ++i)
        {
        res << word[i] << '/';
        }
    res << id << '_' << word << ".txt";
    return res.str();
    }

size_t fillArrayWithNumbers(
    unsigned number,
    char * arr)
    {
    auto i = arr;
    while (number)
        {
        auto n = static_cast<unsigned char>(number & 127);
        number >>= 7;
        if (number == 0) n |= 128;
        *i++ = n;
        }
    return i - arr;
    }

void IndexBuilder::writeToDatabase(unsigned long long id)
    {
    using namespace boost;
    ofstream wordsFile (getWordsFilePath(id));
    ostringstream directory_address;

    for (auto w : _words)
        {
        wordsFile << w->word() << '\n';
    
        try
            {
            ostringstream directory_address;
            directory_address << _databasePath;
            for (size_t c=0, l=min<size_t>(3, w->word().length())
                ; c!=l; ++c)
                {
                    directory_address << '/' << w->word()[c];
                    filesystem::create_directory(directory_address.str());
                }
            }
        catch (filesystem::filesystem_error const & err)
            {
            cout << "can't write " << directory_address.str() << '\n';
            }

            
        ofstream positionsFile(getWordPositionsFilePath(id, w->word()));
        char* arr = new char[10];
        auto_ptr<char> arr_deleter (arr);
        for (unsigned pos: w->positions())
            {
            size_t end = fillArrayWithNumbers(pos, arr);
            arr[end]='\0';
            positionsFile << arr;
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
        auto wordPos = new WordAndPositions(word, ++wordCount);
        _words.push_back(wordPos);
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

    unsigned long long ID = 0;
    
    unsigned counter(0);
    while (getline(filePathsFile, path))
        {
        std::cout << path << '\n';
        // fill _words with words/positions
        indexFile(path);
        
        // write it to database
        // come up with an ID
        unsigned long long id = ++ID;
        
        writeToDatabase(id);
        
        counter += _words.size();
        
        for (auto w: _words)
            {
            delete w;
            }
        }
    
    
    std::cout << "end" << '\n';
    std::cout << counter;
    }



