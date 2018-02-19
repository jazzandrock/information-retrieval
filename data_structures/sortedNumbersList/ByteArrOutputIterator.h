//
//  ByteArrOutputIterator.h
//  IR
//
//  Created by User on 2/19/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_ByteArrOutputIterator_h
#define IR_ByteArrOutputIterator_h


// I want to allow only iterators of char
// to be allowed. (what I want even more is basic_ifstream<unsigned char>)
// one way: https://stackoverflow.com/questions/4354665/function-that-takes-an-stl-iterator-over-any-container-of-a-elements-of-a-specif
template <class OutputIterator_t, class num=unsigned>
class ByteArrOutputIterator
    {
    public:
    typedef num value_type;
    typedef num& reference;
    typedef num* pointer;
    typedef std::output_iterator_tag iterator_category;
    
    ByteArrOutputIterator(OutputIterator_t const & arr)
        : _arr(arr)
        , _n(0)
        {
        }
    
    ByteArrOutputIterator&
    operator=(num n)
        {
//        if (n < _n) throw ;
        num gap = n; // - _n;
        
        char* c = _arr_buf - 1;
        do {
            *(++c) = gap & 127;
            gap >>= 7;
            } while (gap > 0);
        *_arr_buf |= static_cast<char>(128);
        ++c;
        do {
            char cc = *(--c);
            *_arr++ = cc;
        } while (c != _arr_buf);
        *_arr = 128;
        _n = n;
        return *this;
        }
    ByteArrOutputIterator&
    operator*()
        {
        return *this;
        }
    ByteArrOutputIterator&
    operator++()
        {
        return *this;
        }
    ByteArrOutputIterator&
    operator++(int)
        {
        return *this;
        }
    
    
    private:
    char _arr_buf[9];
    OutputIterator_t _arr;
    num _n;
    };

#endif
