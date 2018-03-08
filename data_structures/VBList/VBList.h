//
//  VBList.h
//  IR
//
//  Created by User on 2/17/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__VBList__
#define __IR__VBList__

#include <stdio.h>
#include <vector>
#include "VBOutputIterator.h"

/**
 * can be used only with unsigned numbers
 * num can be only unsigned int or long long int
 */
template <typename num_t>
class VBList
    {
    public:
    typedef VBInputIterator<char*> iterator;
    VBList()
        : VBList(max_vb_size)
        {
        }
    VBList(size_t capacity)
        : _size(0)
        , _capacity(capacity)
        , _arr_start(static_cast<char*>(malloc(_capacity)))
        , _arr_iter(_arr_start)
        , _output(_arr_iter)
        {
        ensure_capacity();
        }
    ~VBList()
        {
        free(_arr_start);
        }
    void
    push_back(num_t n)
        {
        ensure_capacity();
        _output = n;
        ++_size;
        }
        
    iterator
    begin()
        {
        return iterator(_arr_start);
        }
    iterator
    end()
        {
        return iterator();
        }
    
    private:
    static size_t const max_vb_size = sizeof(num_t)*8/7 + 1;
    size_t _size;
    size_t _capacity;
    char * _arr_start;
    char * _arr_iter;
    VBOutputIterator<char*, num_t> _output;
    
    // =====================
    // note how I store char* _arr_iter,
    // I initialize VBOutputIterator with it,
    // but VBOutputIterator takes a reference, not a copy.
    // so it changes where _arr_iter points to.
    // when I ensure_capacity, I create a new array (realloc)
    // and assign _arr_iter to a new value,
    // and VBOutputIterator starts writing to a new place
    // because it holds a reference to _arr_iter!
    // looks creepy but I couldn't find better solution.
    // Does anyone have any suggestions?
    // =====================

    void
    ensure_capacity()
        {
        size_t const arr_size = _arr_iter - _arr_start;
        if (_capacity - arr_size  <  max_vb_size)
            {
            _capacity = _capacity*2 + max_vb_size;
            _arr_start = static_cast<char*>(realloc(_arr_start, _capacity));
            _arr_iter = _arr_start + arr_size;
            }
        }
    };

#endif /* defined(__IR__VBList__) */
