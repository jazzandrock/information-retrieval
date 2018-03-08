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
#include "VBInputIterator.h"

/**
 * can be used only with unsigned numbers
 * num can be only unsigned int or long long int
 */
template <typename num_t>
class VBList
    {
    public:
    typedef VBInputIterator<std::vector<char>::iterator, num_t> iterator;
    typedef VBInputIterator<std::vector<char>::const_iterator, num_t> const_iterator;
    
    VBList(std::vector<char> && inner_data)
        : _size(0)
        , _vector(std::move(inner_data))
        , _v_iter(_vector.end())
        , _v_output(_v_iter)
        {
        }
    VBList()
        : VBList(max_vb_size)
        {
        }
    VBList(size_t capacity)
        : _size(0)
        , _vector(capacity)
        , _v_iter(_vector.begin())
        , _v_output(_v_iter)
        {
        }
    ~VBList() = default;
    void
    push_back(num_t n)
        {
        ensure_capacity();
        _v_output = n;
        ++_size;
        }
    
        
    
    const_iterator begin() const { return const_iterator(_vector.cbegin()); }
    const_iterator end() const { return const_iterator(); }
    iterator begin() { return iterator(_vector.begin()); }
    iterator end() { return iterator(); }
    size_t size() { return _size; }
    
    // if you want to write the numbers to file
    // you can just copy all the _vector
    // instead of decoding it to numbers
    // and then encoding back to write them
    std::vector<char> const &
    inner_data() const
        {
        return _vector;
        }
    
    private:
    static size_t const max_vb_size = sizeof(num_t)*8/7 + 1;
    size_t _size;
    std::vector<char> _vector;
    std::vector<char>::iterator _v_iter;
    VBOutputIterator<std::vector<char>::iterator, num_t, true> _v_output;
    
    void
    ensure_capacity()
        {
        size_t const arr_size = _v_iter - _vector.begin();
        if (_vector.size() - arr_size  <  max_vb_size)
            {
            _vector.resize(_vector.size() + max_vb_size);
            _v_iter = _vector.begin() + arr_size;
            }
        }
    };

#endif /* defined(__IR__VBList__) */


////
////  VBList.h
////  IR
////
////  Created by User on 2/17/18.
////  Copyright (c) 2018 Oleg. All rights reserved.
////
//
//#ifndef __IR__VBList__
//#define __IR__VBList__
//
//#include <stdio.h>
//#include <vector>
//#include "VBOutputIterator.h"
//#include "VBInputIterator.h"
//
///**
// * can be used only with unsigned numbers
// * num can be only unsigned int or long long int
// */
//template <typename num_t>
//class VBList
//    {
//    public:
//    typedef VBInputIterator<char*, num_t> iterator;
//    
//    
//    VBList()
//        : VBList(max_vb_size)
//        {
//        }
//    VBList(size_t capacity)
//        : _size(0)
//        , _capacity(capacity)
//        , _arr_start(static_cast<char*>(malloc(_capacity)))
//        , _arr_iter(_arr_start)
//        , _output(_arr_iter)
//        {
//        ensure_capacity();
//        }
//    ~VBList()
//        {
//        free(_arr_start);
//        }
//    void
//    push_back(num_t n)
//        {
//        ensure_capacity();
//        _output = n;
//        ++_size;
//        }
//        
//    iterator
//    begin()
//        {
//        return iterator(_arr_start);
//        }
//    iterator
//    end()
//        {
//        return iterator();
//        }
//        
//    size_t
//    size()
//        {
//        return _size;
//        }
//    
//    private:
//    static size_t const max_vb_size = sizeof(num_t)*8/7 + 1;
//    size_t _size;
//    size_t _capacity;
//    char * _arr_start;
//    char * _arr_iter;
//    VBOutputIterator<char*, num_t, true> _output;
//    
//    // =====================
//    // note how I store char* _arr_iter,
//    // I initialize VBOutputIterator with it,
//    // but VBOutputIterator takes a reference, not a copy.
//    // so it changes where _arr_iter points to.
//    // when I ensure_capacity, I create a new array (realloc)
//    // and assign _arr_iter to a new value,
//    // and VBOutputIterator starts writing to a new place
//    // because it holds a reference to _arr_iter!
//    // looks creepy but I couldn't find better solution.
//    // Does anyone have any suggestions?
//    // =====================
//
//    void
//    ensure_capacity()
//        {
//        size_t const arr_size = _arr_iter - _arr_start;
//        if (_capacity - arr_size  <  max_vb_size)
//            {
//            _capacity = _capacity*2 + max_vb_size;
//            _arr_start = static_cast<char*>(realloc(_arr_start, _capacity));
//            _arr_iter = _arr_start + arr_size;
//            }
//        }
//    };
//
//#endif /* defined(__IR__VBList__) */
//
