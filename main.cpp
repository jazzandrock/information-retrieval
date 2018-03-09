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
#include "defs.h"
#include "sorting/Dedup.h"
#include "data_structures/linked_list/linked_list.h"
#include "indexer/IndexBuilder.h"
#include "VBInputIterator.h"
#include "VBOutputIterator.h"
#include "VBList.h"
#include "VBGapList.h"
#include "VBGapInputIterator.h"
#include <boost/algorithm/cxx11/copy_if.hpp>
#include <boost/operators.hpp>


struct S : public boost::equality_comparable<S>, public boost::less_than_comparable<S> {
    S(int i) : i_(i) {}
    bool operator==(S const & o) const {return i_ == o.i_; }
    bool operator<(S const & o) const {return i_ < o.i_; }
    int i_;
};

//template <class ForwardIterator_t, class num_t>
//class Classss
//    : public boost::equality_comparable<Classss<ForwardIterator_t, num_t>&>
////    , public boost::incrementable<Classss<ForwardIterator_t, num_t>>
//    {
//    public:
//    bool operator==(Classss const & o) { return true; }
//    };

int main(int /*argc*/, const char * /*argv*/[]) {
    using namespace std;
//    S s1(2);
//    S s2(5);
//    cout << (s1 < s2) << '\n';
//    cout << (s1 > s2) << '\n';
//    cout << (s1 == s2) << '\n';
//    cout << (s1 != s2) << '\n';
//    cout << (s1 <= s2) << '\n';
//    cout << (s1 >= s2) << '\n';
//    Classss<unsigned, unsigned> c;
//    Classss<unsigned, unsigned> cc;
//    cout << (c != cc) << '\n';
    
//    VBGapList<unsigned> gapl;
//    gapl.push_back(123);
//    gapl.push_back(456);
//    gapl.push_back(789);
//    vector<char> v = gapl.inner_data();
//    VBGapInputIterator<std::vector<char>::iterator, unsigned> l (v.begin());
//    while (l != l) {
//        cout << *l << '\n';
//        ++l;
//    }

//    VBList<int> l (std::move(v));
////    vector<char> v2 (std::move(v));
//    cout << v.size();

//    VBGapList<unsigned> l (std::move(v));
    
//    Trai

//    vector <unsigned> v;
//    v.at(10) = 2;
    
//    vector<unsigned> v;
//    for (unsigned i = 0; i != 100; i++) v.push_back(i);
//    vector<unsigned> dest;
//    boost::algorithm::copy_until(
//        v.begin(), v.end(),
//        back_inserter(dest),
//        [] (unsigned e) {
//            return e == 30;
//        });
//    for (auto e: dest) std::cout << e << '\n';
    
    
//    vector<uint8_t> v;
//    v.push_back(1);
//    v.push_back(1);
//    v.push_back(1);
//    v.resize(6);
//    vector<uint8_t>::iterator i(v.begin()); // invalidates vector
//    *i++ = 1;
//    *i++ = 1;
//    *i++ = 1;
//    *i++ = 1;
//    *i++ = 1;
//    *i++ = 1;
////    cout << v.end() - v.begin() << '\n';
//    for (unsigned i: v) cout << i << '\n';
//    *i = 1;
//    i++;
//    i++;
//    *i = 3;
//    *i = 4;
//    for (auto e: v) cout << e << '\n';
//    VBOutputIterator<vector<uint8_t>::iterator, unsigned, true>
//    VBGapList<unsigned> l;
//    l.push_back(124);
//    l.push_back(126);
//    l.push_back(1266);
//    l.push_back(1566);
//    l.push_back(16566);
//    l.push_back(17566);
//    for (auto i : l)
//        {
//        cout << i << '\n';
//        }
//    VBList<unsigned> ll;
//    VBList<unsigned>::iterator i = ll.begin();
    
//    ofstream fout("file");
//    ostreambuf_iterator<char> ofstr_iter (fout.rdbuf());
//    VBOutputIterator<ostreambuf_iterator<char>, unsigned, false> out(ofstr_iter);
//    out = 1;
//    out = 333;
//    out = 18;
//    out.terminate();
//    fout.flush();
//    
//    ifstream fin("file");
//    VBInputIterator<istreambuf_iterator<char>, unsigned> in(fin.rdbuf());
//    VBInputIterator<istreambuf_iterator<char>, unsigned> end;
//    while (in != end)
//        {
//        cout << *in << endl;
//        ++in;
//        }
    

//    VBGapList<unsigned> glull;
//    unsigned const start_val = 1;
//    unsigned const end_val = 1000000;
//    for (unsigned i = start_val; i != end_val; ++i) {
//        glull.push_back(i);
//    }
////    glull.push_back(124);
//    std::cout << "size: " << glull.size() << '\n';
//    auto const & cglull = glull;
//    unsigned i = start_val;
//    for (auto e : cglull) {
//        if (e != i) {
//            std::cout << "e: " << e << ", i:" << i << '\n';
//        }
//        ++i;
//    }
    
//    vector<char> v;
////    v.reserve(100);
//    back_insert_iterator<vector<char>>  ins(v);
//    char * arr = &v[0];
//    ByteArrOutputIterator<back_insert_iterator<vector<char>>, unsigned, false> o(ins);
////    ByteArrOutputIterator<char*, unsigned, false> o(arr);
////    *o = 1234;
////    *o = 11111;
////    *o = 11111;
////    *o = 11111;
//    o.terminate();
////    for (char c : v) {
////        std::cout << (int)(unsigned char)(c) << '\n';
////    }
//    VBInputIterator<vector<char>::iterator> i (v.begin());
//    VBInputIterator<vector<char>::iterator> e;
//    while (i != e)
//        {
//        std::cout << *i << ' ';
//        ++i;
//        }

//    VBInputIterator<vector<char>::iterator> a(v.begin());
//    VBInputIterator<vector<char>::iterator> e;
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
//    VBInputIterator<char*> i (arr);
//    VBInputIterator<char*> e;
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
//    VBInputIterator<char*> i (arr);
//    VBInputIterator<char*> end;
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
//    
    IndexBuilding::IndexBuilder b (
        "/Users/user/XCodeProjects/IR/db",
        "/Users/user/XCodeProjects/IR/db/files_list_processed_with_ids.txt");
        
    b.index("/Users/user/XCodeProjects/IR/db/_files_list.txt");

//    basic_ifstream<char> fin ("/Users/user/XCodeProjects/IR/db/2.wrdps", std::ios::binary);
//    istreambuf_iterator<char> f_iter (fin);
//    istreambuf_iterator<char> f_end;
//    struct WP {
//        char* _str;
//        size_t _strlen;
//        vector<unsigned> _l;
//    };
//    
//    vector<WP> words;
//    
//    while (f_iter != f_end)
//        {
//        WP w;
//        w._strlen = static_cast<unsigned char>(*f_iter);
//        w._str = new char[w._strlen+1];
//        for ( size_t i=0; i!=w._strlen; ++i )
//            {
//            ++f_iter;
//            char c = static_cast<char>(*f_iter);
//            w._str[i] = c;
//            }
//        ++f_iter;
//        w._str[w._strlen] = '\0';
//        VBInputIterator< istreambuf_iterator<char>, position_t > nums (f_iter);
//        VBInputIterator< istreambuf_iterator<char>, position_t > nums_end;
//        while (nums != nums_end)
//            {
////            cout << *nums << '\n';
//            w._l.push_back(*nums);
//            ++nums;
//            }
//        ++f_iter;
//        words.push_back(w);
//        }
//    
//    
//    for (auto wp: words)
//        {
//        cout << '\n' << wp._str << '\n';
//        unsigned acc (0);
//        for (auto pos: wp._l)
//            {
//            acc = pos;
//            cout << acc << ' ';
//            }
//        }

    return 0;
}