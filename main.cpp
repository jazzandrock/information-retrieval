//
//  main.cpp
//  IR
//
//  Created by User on 2/8/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>
#include <boost/operators.hpp>
#include <cassert>
#include <string>
#include <map>
#include <forward_list>
#include "defs.h"
#include "sorting/Dedup.h"
#include "indexer/IndexBuilder.h"
#include "VBInputIterator.h"
#include "VBOutputIterator.h"
#include "VBList.h"
#include "VBGapList.h"
#include "VBGapInputIterator.h"
#include "TestingFunctions.h"
#include "IndexMerging.h"
#include "utils.h"
#include <chrono>
#include "RangedQuery.h"
#include <queue>
#include <array>
#include <clocale>
#include "Timer.h"
#include <regex>
#include <boost/locale.hpp>


int main(int /*argc*/, const char * /*argv*/[]) {
    using namespace std;

    {
        using namespace boost::locale;
        generator gen;
        auto loc = gen("en_US.UTF-8");
        locale::global(loc);
    }
    
//    {
//        string regex_part = u8"[\\wа-яїєь]+";
//        auto regex_full = regex_part + "'?" + regex_part;
//        regex re(regex_full);
//        smatch match;
//        string s = "/Volumes/160GB/do<Thtml>, **don't' Яблонський їжа*/4_tr/information_retrieval/gutenberg/";
//
//        sregex_iterator next (s.begin(), s.end(), re);
//        sregex_iterator end;
//        while (next != end) {
//            auto word = next->str();
//            word = boost::locale::to_lower(word);
//            cout << word << '\n';
//            ++next;
//        }
//        
//        return 0;
//    }
//
//    {
//        using namespace IndexBuilding;
//        string const prefix = "/Volumes/160GB/do/4_tr/information_retrieval/gutenberg/";
//        string const dbpath = prefix + "/long_db";
////        string const dbpath = prefix + "/short_db";
////        string const dbpath = prefix + "/3600_db";
////        string const dbpath = prefix + "/3270_bug_finding";
//        IndexBuilder builder(
//            dbpath,
//            dbpath + "/files_list_processed_with_ids.txt",
//            prefix);
//        {
//            Timer _ (dbpath + "/duration.txt");
//            builder.indexMoreLines(dbpath + "/list", 100);
//        }
// 
//        
//        {
//            cout << "start loading\n";
//            Timer _ ("");
//            builder.loadIndex();
//            cout << "loaded\n";
//        }
//        Index const * index = builder.getIndex();
//        
//        {
//            cout << "saving indexes:\n";
//            Timer _ ("");
//            index->saveReadableIndexes();
//        }
//        RangedQuerySearcher searcher(index);
//        
////        {
////            Timer _ ("");
////            searcher.search("important world class listening problem");
////        }
//
//        return 0;
//    }
// 
    {
        const string prefix = "/Volumes/160GB/do/4_tr/information_retrieval/wiki";
        const string dbpath = prefix + "/small_db";
        IndexBuilding::IndexBuilder builder(
            dbpath,
            dbpath + "/files_list_processed_with_ids.txt",
            prefix + "/wiki uk 2008/");

        {
            Timer _ (dbpath + "/duration.txt");
            builder.indexMoreLines(dbpath + "/list", 100);
        }
        // total time to index wikipedia: 4401 seconds

        
        return 0;
        {
            cout << "start loading\n";
            Timer _ ("");
            builder.loadIndex();
            cout << "loaded\n";
        }
        Index const * index = builder.getIndex();

        {
            cout << "saving indexes:\n";
            Timer _ ("");
            index->saveReadableIndexes();
        }
        RangedQuerySearcher searcher(index);

        {
            Timer _ ("");
            searcher.search("important world class listening problem");
        }
        
//        cout << "cosine similarity:\n";
//        cout << builder.getIndex()->cosineSimilarity(2, 3);
    }
    return 0;
}