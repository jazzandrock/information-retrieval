
//
//  HeapTopKSelection.h
//  IR
//
//  Created by User on 4/14/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_HeapTopKSelection_h
#define IR_HeapTopKSelection_h

#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>

/**
 * returns a vector with no more than K elements
 * that are sorted with cmp.
 * it's the same as taking first K elements
 * of sorted range, but works faster
 *
 * running time: N log K
 */
template <class T, class InputIterator, class Comp>
std::vector<T> selectTopK(InputIterator start, InputIterator end, size_t K, Comp cmp) {
    using namespace std;
    assert(start <= end);
    
    vector<T> v;
    v.reserve(K + 1);
    
    while (start != end and v.size() < K) {
        v.push_back(*start);
        push_heap(v.begin(), v.end(), cmp);
        ++start;
    }
    while (start != end) {
        v.push_back(*start); ++start;
        push_heap(v.begin(), v.end(), cmp);
        pop_heap(v.begin(), v.end(), cmp);
        v.pop_back();
    }
    
    sort(v.begin(), v.end(), cmp);

    return move(v);
}



#endif
