//
//  GapList.h
//  IR
//
//  Created by User on 2/17/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__GapList__
#define __IR__GapList__

#include <stdio.h>
#include <vector>

/**
 * can be used only with unsigned numbers
 * num can be only unsigned int or long long int
 */
template <typename num_t>
class GapList {
    public:
    GapList()
        : _largest_val(0)
        , _size(0)
        {}
    
    void
    addNumber(num_t n)
        {
        if (n < _largest_val) throw ;
        num_t gap = n - _largest_val;
        
        size_t prev_v_size = _v.size();
        while (gap != 0)
            {
            unsigned char c = gap & 127;
            _v.push_back(c);
            gap >>= 7;
            }
        for (size_t i=prev_v_size, j=_v.size()-1
            ; i < j; ++i, --j)
            {
            swap(_v[i], _v[j]);
            }
        _v.end()-1 |= 128;
            
        _largest_val = n;
        ++_size;
        }
    

    // i need an iterator
    // shit
    // it has to be an internal class
    // oh
//    template<typename num>
//    static class iterator
//        {
//        public:
//        iterator()
//            {
//            // end
//            }
//            
//        iterator()
//        };
//    
    
    private:
    num_t _largest_val;
    size_t _size;
    
    // this one is needed just for memory management
    // a substitution of malloc, realloc, stuff
    std::vector<unsigned char> _v;
};

#endif /* defined(__IR__GapList__) */
