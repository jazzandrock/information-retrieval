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


class WordAndPositions;

template <class T>
class Dedup;


namespace
IndexBuilding
    {
    class
    IndexBuilder
        {
        public:
        IndexBuilder();
        ~IndexBuilder();
        
        void
        index(
            std::string filePaths,
            std::string databasePath,
            std::string indexFilePath);
        
        void
        indexFile(std::string filePath);
        
        void writeToDatabase(
            WordAndPositions const* words[],
            size_t size);
        
        private:
        IndexBuilder(IndexBuilder &);
        
        Dedup<WordAndPositions*> * _dedup;
        std::vector<WordAndPositions*> _words;
        };
    }

#endif /* defined(__IR__IndexBuilder__) */
