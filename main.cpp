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

//struct S {
//    S(): i_(1) {}
//    int i_;
//    int& i() { return i_;}
//    ~S() { std::cout << "katrin destroyer\n"; }
//};

int main(int /*argc*/, const char * /*argv*/[]) {
    using namespace std;
    
//    {
//        vector<int> v1 {1, 6, 8, 10, 12, 13};
//        vector<int> v2 {1, 8, 9, 10, 11, 13};
//        vector<int> res;
//        struct Handler: NullOutputIterator {
//            Handler& operator*() { return *this; }
//            void operator=(int i) {
//                cout << "handled " << i << '\n';
//            }
//        };
//        set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), Handler(), [] (int a, int b) {
//            if (a == b) cout << a << '\n';
//            return a < b;
//        });
//        cout << res << '\n';
//        return 0;
//    }
    
//    {
//        vector<int> v { 2, 5, 1, 5, 3, 2, 4, 2 };
//        cout << selectTopK<int>(v.begin(), v.end(), 2, std::greater<int>());
////        priority_queue<int, array<int, 3>> q;
////        q.push(5);
////        q.push(3);
////        q.push(8);
////        q.push(10);
////        q.push(1);
////        vector<int> vec_heap;
////        vec_heap.resize(10);
////        cout << vec_heap << '\n';
////        vec_heap.pop_back();
////        vec_heap.push_back(5);
////        push_heap(vec_heap.begin(), vec_heap.end());
////        cout << vec_heap << '\n';
////
////        for (int i = 0; i != 3; ++i) {
////            cout << q.top() << ' ';
////            q.pop();
////        }
//        
//        
//        return 0;
//    }

//    {
//        int& i = S().i();
//        cout << i;
//        cout << "end\n";
//        return 0;
//    }
//    {
//        S s = S();
//        int& i = s.i();
//        cout << i;
//        cout << "end\n";
//        return 0;
//    }
//    {
//        vector<string> words { "aba", "asa", "asda", "bb", "ccc", "kma", "ol" };
////        sort(words.begin(), words.end());
////        copy(words.begin(), words.end(), (ostream_iterator<string>(cout, "\n")));
//        vector<int> v{0, 1, 2, 3, 4, 5, 6};
////        string find = "asda";
//        vector<int> foundIndexes;
//        for (string find: words) {
//            int curr_idx = 0;
//            curr_idx = binarySearch(v.begin(), v.end(), [&] (int i) {
//                return words[i].compare(find);
//            });
//            foundIndexes.push_back(curr_idx);
//        }
//        
//        cout << foundIndexes << endl;
//        return 0;
//    }
//    {
//        string idx = mapIndex2StringIndex(readIndexFromFile("/Volumes/160Gb/do/db/idx/0.index"));
//        string::const_iterator it = idx.begin();
//        IndexIterator<decltype(it)&, docid_t> iter(it);
////        ++iter;
//        cout << (it - idx.begin()) << endl;
//        
//        
//        return 0;
//    }
    
    // we need our documents in vector space    
    
//    {
//        vector<char> out_v;
//        auto out = back_inserter(out_v);
//        vector<char> in { 1, -128, 1, 127, -128 };
//        auto innerDataIter = in.begin();
//        while (*innerDataIter != -128) {
//            while ((*innerDataIter >> 7) == 0) {
//                *out++ = *innerDataIter;
//                ++innerDataIter;
//            }
//            *out++ = *innerDataIter;
//            ++innerDataIter;
//        }
//        *out++ = *innerDataIter;
//        cout << charVectorToBinaryString(out_v);
//
//        return 0;
//    }
//    writeReadableIndexToFile(readIndexFromFile("mergeIndexes_err_1_idx1_str"), "mergeIndexes_err_1_idx1_str.txt");
//    auto idx = readIndex("mergeIndexes_err_1_idx2_str");
//    writeReadableIndex(idx, "mergeIndexes_err_2_idx1_str.txt");
//    
//    return 0;
//
//    vector<int> v1 {1, 2};
//    vector<int> v2 {1, 3};
//    vector<int> dest;
//    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(dest));
//    cout << dest;
//    return 0;

//    vector<int> v1 {1, 3, 6, 9, 9, 10};
//    vector<int> v2 {1, 3, 6, 9, 9, 10};
//    vector<int> dest;
//    vector<int>::const_iterator v1i = v1.begin();
//    vector<int>::const_iterator v1e = v1.end();
//    vector<int>::const_iterator v2i = v2.begin();
//    vector<int>::const_iterator v2e = v2.end();
//    back_insert_iterator<vector<int>> desti (dest);
//    Merger<vector<int>::const_iterator, back_insert_iterator<vector<int>>, int>
//        merger (lesss<int>, equals<int>, ifEquals);
//    merger.merge(v1i, v1e, v2i, v2e, desti);
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
//    IndexBuilding::IndexBuilder (
//        "/Users/user/XCodeProjects/IR/db",
//        "/Users/user/XCodeProjects/IR/db/files_list_processed_with_ids.txt",
//        "")
//    .index("/Users/user/XCodeProjects/IR/db/_files_list.txt");

//    {
//        using namespace IndexBuilding;
//        string const prefix = "/Volumes/160GB/do/4_tr/information_retrieval/gutenberg/";
////        string const dbpath = prefix + "/long_db";
////        string const dbpath = prefix + "/short_db";
////        string const dbpath = prefix + "/3600_db";
//        string const dbpath = prefix + "/3270_bug_finding";
//        IndexBuilder builder(
//            dbpath,
//            dbpath + "/files_list_processed_with_ids.txt",
//            prefix);
//        builder.index(dbpath + "/list");
////        return 0;
//    }
 
    {
        using namespace chrono;
        using t_t = milliseconds;
        t_t start = duration_cast<t_t>(system_clock::now().time_since_epoch());
        IndexBuilding::IndexBuilder builder(
            "/Volumes/160GB/do/db",
            "/Volumes/160GB/do/wiki/files_list_processed_with_ids.txt",
            "/Volumes/160GB/do/wiki/");
//        .index("/Volumes/160GB/do/wiki/medium_list.txt");
        builder.index("/Volumes/160GB/do/wiki/short_list.txt");
//        builder.loadIndex();
//        builder.index("/Volumes/160GB/do/wiki/long_list.txt");
        t_t end = duration_cast<t_t>(system_clock::now().time_since_epoch());
        cout << "\n\ntime: ";
        ofstream("duration.txt") << (end - start).count() << endl << endl;
        
        // TODO: this probably should be returned from builder.method
        RangedQuerySearcher searcher(builder.getIndex());
        QueryWithWeights q {
            {"Олежка", 100.0},
            {"be", 2.0},
            {"умовними", 1.0},
            {"argument", 2.0}
        };
//        auto ans = searcher.query(q);
        auto ans = searcher.queryBM(q);
        cout << "size: " << ans.size() << '\n';
        for (auto p : ans) {
            cout << "doc: " << p.doc_ << ", score: " << p.relevance_ << '\n';
        }
        
        cout << "cosine similarity:\n";
        cout << builder.getIndex()->cosineSimilarity(2, 3);
    }
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
//    assert(testIndexMerging());

    return 0;
}