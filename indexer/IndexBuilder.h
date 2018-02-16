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
        IndexBuilder(
            std::string const & databasePath,
            std::string const & indexFilePath);
        ~IndexBuilder();
        
        void
        index(std::string filePaths);
        
        void
        indexFile(std::string filePath);
        
        /**
         * writes:
         * a file with title: ID_words.txt,
         * words separated by \n
         *
         * multiple files named ID_$word.positions
         * a binary file with positions
         * located in database/w/o/r/ID_$word.positions
         */
        void writeToDatabase(unsigned long long id);
        
        private:
        IndexBuilder(IndexBuilder &);
        
        std::string getWordsFilePath(
            unsigned long long id);
        
        std::string getWordPositionsFilePath(
            unsigned long long id,
            std::string const & word);
        
        void createDirectoriesIfNeeded(
            std::string const & word);
        
        Dedup<WordAndPositions*> * _dedup;
        std::vector<WordAndPositions*> _words;
        std::string _databasePath;
        std::string _indexFilePath;
        };
    }

#endif /* defined(__IR__IndexBuilder__) */
