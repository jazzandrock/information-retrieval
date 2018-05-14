//
//  IndexBuilder.h
//  IR
//
//  Created by User on 2/11/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__IndexBuilder__
#define __IR__IndexBuilder__

#include <stdio.h>
#include <string>
#include <vector>
#include <limits>
#include "Index.h"
#include "defs.h"

class WordAndPositions;

template <class T>
class Dedup;


namespace IndexBuilding {
class IndexBuilder {
public:
    IndexBuilder(
        std::string const databasePath,
        std::string const indexFilePath,
        std::string const base);
        
    ~IndexBuilder();
    
    void
    index(std::string filePaths, size_t firstLineToProcess=0, size_t lastLineToProcess = std::numeric_limits<size_t>::max());
    
    void loadIndex();
    
    bool indexMoreLines(std::string filePaths, size_t numberOfLinesToIndex);
    
    void
    indexFile(std::string const& filePath);
    
    void saveWordPositionsToFile(
        docid_t id,
        std::vector<WordAndPositions*> const & words);

    
    inline Index const * getIndex() { return _index; }
private:
    IndexBuilder(IndexBuilder &);
    
    std::string
    getWordsFilePath(docid_t);
    
    std::string
    getWordPositionsFilePath(
        docid_t,
        std::string const& word);
    
    void
    createDirectoriesIfNeeded(
        std::string const& word);
    
    // fields
    Dedup<WordAndPositions*> * _dedup;
    std::vector<WordAndPositions*> _words;
//    std::vector<std::string> _indexes;
    Index * _index;
    std::string _databasePath;
    std::string _indexFilePath;
    std::string _base;
    
//    inline std::vector<std::string> & indexes() { return _indexes; }
//    inline bool indexExists(size_t idxidx)
//        {
//        return indexes().size() > idxidx  &&  indexes()[idxidx].length() > 0;
//        }
//    inline void addIndex(std::string & idx, size_t idxidx)
//        {
//        indexes().reserve(idxidx + 1);
//        if (indexes().size() < (idxidx + 1))
//            indexes().resize(idxidx + 1);
//        indexes()[idxidx] = std::move(idx);
//        }
//    void addNewIndexAndMerge(std::string const& first_index);
    };
}

#endif /* defined(__IR__IndexBuilder__) */
