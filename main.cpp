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

struct A
    {
    ~A() { std::cout << "katrin destroyer" << '\n'; }
    };

int main(int /*argc*/, const char * /*argv*/[]) {
//    std::vector<A*> v(1);
//    A a;
//    v.push_back(&a);
//    v.resize(2);
//    std::cout << "that's it" << '\n';
//    
    
    IndexBuilding::IndexBuilder b;
    b.index(
        "/Users/user/XCodeProjects/IR/IR/indexer/_files_list.txt",
        "/Users/user/XCodeProjects/IR/IR/indexer/database",
        "/Users/user/XCodeProjects/IR/IR/indexer/files_list_processed_with_ids.txt");
    
    return 0;
}
