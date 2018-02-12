//
//  IndexBuilder.cpp
//  IR
//
//  Created by User on 2/11/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include "IndexBuilder.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <forward_list>
#include "../sorting/Dedup.h"


class
WordAndPositions
    {
    friend
    std::ostream&
    operator<< (
        std::ostream & stream,
        WordAndPositions const & wordPositions);
        
    friend
    void
    mergeWordAndPositions(
        WordAndPositions * & a,
        WordAndPositions *const & b);
        
    public:
    WordAndPositions(
        std::string & word,
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
        stream << "WordAndPositions: " << wordPositions._word << '\n';
        stream << "positions: ";
        for (unsigned pos : wordPositions._positions)
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
        
        // TODO:
//        auto bb = const_cast<WordAndPositions *>(b);
//        bb = nullptr;
//        assert(b == nullptr);
        }


using namespace std;
using namespace IndexBuilding;

IndexBuilder::IndexBuilder()
    : _dedup(
        new Dedup<WordAndPositions*>(
            wordAndPositionsEquals,
            wordAndPositionsLess,
            mergeWordAndPositions
            )
        )
    // , _words — default constructor suffices
    {
    }

IndexBuilder::~IndexBuilder()
    {
    delete _dedup;
    }


void IndexBuilder::writeToDatabase(
    WordAndPositions const* words[],
    size_t size)
    {
    WordAndPositions const * * i = words, * * l = i + size;
    for ( ; i!=l; ++i )
        {
        
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
IndexBuilder::index(
    string filePaths,
    string databasePath,
    string indexFilePath)
    {
    ifstream filePathsFile (filePaths);
    ofstream files ( indexFilePath);
        
    string path;
    while (getline(filePathsFile, path))
        {
        std::cout << path << '\n';
        // fill _words with words/positions
        indexFile(path);
        
        // write it to database
        // come up with an ID
        
        
        for (auto w: _words)
            {
            delete w;
            }
        }
        
    std::cout << "end" << '\n';
    }










