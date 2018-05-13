
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
#include <cassert>


template <class T, class InputIterator, class Comp>
std::vector<T> selectTopK(InputIterator start, InputIterator end, size_t K, Comp cmp) {
    using namespace std;
    assert(start != end);
    priority_queue<T, vector<T>, Comp> q(cmp);
    
    while (start != end) {
        auto& e = *start;
        q.push(e);
        if (q.size() == (K+1)) q.pop();
        ++start;
    }

    vector<T> res;
    res.resize(K);
    for (size_t i = K; i > 0; --i) {
        res[i - 1] = q.top();
        q.pop();
    }

    return move(res);
}



#endif
