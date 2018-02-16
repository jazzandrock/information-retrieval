//
//  main.cpp
//  IR
//
//  Created by User on 2/8/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <iostream>
#include <boost/filesystem.hpp>
#include <cassert>
#include <string>
#include <map>
#include <forward_list>
#include "sorting/Dedup.h"
#include "data_structures/linked_list/linked_list.h"
#include "indexer/IndexBuilder.h"

namespace
    {
    template<typename T>
    bool
    less(T const & a, T const & b)
        {
        return a < b;
        }

    template<typename T>
    bool
    equals(T const & a, T const & b)
        {
        return a == b;
        }

    template<typename T>
    void
    doSomethingWithEqualElements(T & a, T const & b)
        {
        }
    }

int main(int /*argc*/, const char * /*argv*/[]) {
    IndexBuilding::IndexBuilder b (
        "/Users/user/XCodeProjects/IR/db",
        "/Users/user/XCodeProjects/IR/db/files_list_processed_with_ids.txt");
        
    b.index("/Users/user/XCodeProjects/IR/db/files_list.txt");
    return 0;
}
