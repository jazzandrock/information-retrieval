//
//  Test.cpp
//  IR
//
//  Created by User on 3/10/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include "gtest/gtest.h"
#include <string>
#include <map>
#include "VBGapList.h"
#include "TestingFunctions.h"

namespace {


TEST(IndexTest, merging) {
//    using namespace std;
//    map<string, VBGapList<unsigned>> wordToId;
//    wordToId["abc"] = 4;
//    wordToId["cde"] = 6;
//    wordToId["abc"] = 5;
//    wordToId["cde"] = 10;
//    
//    string first_index;
//	back_insert_iterator<string> first_index_out_iter (first_index);
//	saveWordsToIterator(wordToIDMap, first_index_out_iter);
//	for (unsigned i = 0; 1; ++i) {
//	    if (indexExists(i)) {
//	        auto dest = back_inserter(index_dest);
//	        mergeIndexes(first_index, indexes()[i], dest);
//	        first_index = move(index_dest);
//        } else {
//	        addIndex(first_index, i);
//	        break;
//        }
//    }
//
//    EXPECT_TRUE(testIndexMerging);
}


}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
