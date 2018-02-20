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
#include "ByteArrIterator.h"
#include "ByteArrOutputIterator.h"

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
using namespace std;

int main(int /*argc*/, const char * /*argv*/[]) {
//    ByteArrIterator<vector<char>::iterator> a(v.begin());
//    ByteArrIterator<vector<char>::iterator> e;
//    while (a != e)
//        {
//        cout << *a << '\n';
//        ++a;
//        }
//    
////    char arr[256];
////    char c = 0;
////    do {arr[static_cast<unsigned char>(c)] = c;} while (++c != 0);
////    for (char & c: arr)
////        {
////        c &= 128;
////        cout << (unsigned int)(unsigned char)c << '\n';
////        }
//    char* arr = new char[10];
//    ByteArrOutputIterator<char*> o(arr);
//    for (unsigned i = 1; i != 200; ++i)
//        *o++ = i;
////    for (size_t i = 0; i < 30; i++)
////        std::cout << (int)(unsigned char)(arr[i]) << '\n';
//    ByteArrIterator<char*> i (arr);
//    ByteArrIterator<char*> e;
//    while (i != e)
//        {
//        cout << *i << '\n';
//        ++i;
//        }
//    
//    ofstream fout("addr");
//    ostreambuf_iterator<char> i (fout);
//    *i++ = 'a';

// how to test my vector?
// i can use it for an array of chars
//    unsigned char arr[] = {125, 10, 131, 123, 3, 155, 10, 128};
//    char arr[] = {1, (char)129, 1, (char)130, (char)128};
//    ByteArrIterator<char*> i (arr);
//    ByteArrIterator<char*> end;
//    for ( ; i != end; ++i)
//        std::cout << *i << '\n';

//    iterator_traits<forward_list<int>::iterator>::iterator_category::;
//    iterator_traits<vector<int>::iterator>::iterator_category::

    // for some reason or another
    // these are the smartest 5
    // lines of code I have ever wrote
//    map<string, vector<docid_t>> m;
//    m["aa"].push_back(222);
//    for (auto v : m)
//        {
//        v.second.shrink_to_fit();
//        }
    
    
   
    
    
    
    
//    IndexBuilding::IndexBuilder b (
//        "/Users/user/XCodeProjects/IR/db",
//        "/Users/user/XCodeProjects/IR/db/files_list_processed_with_ids.txt");
//        
//    b.index("/Users/user/XCodeProjects/IR/db/_files_list.txt");

    basic_ifstream<char> fin ("/Users/user/XCodeProjects/IR/db/2.wrdps", std::ios::binary);
    istreambuf_iterator<char> f_iter (fin);
    istreambuf_iterator<char> f_end;
    struct WP {
        char* _str;
        size_t _strlen;
        forward_list<unsigned> _l;
    };
    
    forward_list<WP> words;
    
    while (f_iter != f_end)
        {
        WP w;
        w._strlen = static_cast<unsigned char>(*f_iter);
        w._str = new char[w._strlen+1];
        for ( size_t i=0; i!=w._strlen; ++i )
            {
            ++f_iter;
            char c = static_cast<char>(*f_iter);
            w._str[i] = c;
            }
        ++f_iter;
        w._str[w._strlen] = '\0';
        ByteArrIterator< istreambuf_iterator<char> > nums (f_iter);
        ByteArrIterator< istreambuf_iterator<char> > nums_end;
        while (nums != nums_end)
            {
//            cout << *nums << '\n';
            w._l.push_front(*nums);
            ++nums;
            }
        ++f_iter;
        words.push_front(w);
        }
    
    
    for (auto wp: words)
        {
        cout << '\n' << wp._str << '\n';
        unsigned acc (0);
        for (auto pos: wp._l)
            {
            acc = pos;
            cout << acc << ' ';
            }
        }

    return 0;
}
