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
#include "RangedQuerySearcher.h"
#include <queue>
#include <array>
#include <clocale>
#include "Timer.h"
#include <regex>
#include <thread>
#include <boost/locale.hpp>


int main(int /*argc*/, const char * /*argv*/[]) {
    using namespace std;

    {
        using namespace boost::locale;
        generator gen;
        auto loc = gen("en_US.UTF-8");
        locale::global(loc);
    }
    
    {
        const string prefix = "/Volumes/160GB/do/4_tr/information_retrieval/wiki";
        const string dbpath = prefix + "/db";
//        IndexBuilding::IndexBuilder builder(dbpath, prefix + "/wiki uk 2008/");

//        builder.indexForSeconds(3600*7);
//        builder.indexForSeconds(10);

        Index index(dbpath);
        index.loadIndexes();
        RangedQuerySearcher searcher(&index);

        {
            Timer _ ("");
//            searcher.search("склали список геніїв сучасності одну четверту списку склали британці");
//            searcher.search("дмитро");
            searcher.search("мова програмування haskell");
        }
    }
    return 0;
}