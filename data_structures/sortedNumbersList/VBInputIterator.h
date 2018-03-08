//
//  ByteArrIterator.h
//  IR
//
//  Created by User on 2/18/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__ByteArrIterator__
#define __IR__ByteArrIterator__

#include <stdio.h>
#include <iterator>
#include <type_traits>

// shit
// i need an iterator or sth to
// iterate through array of bytes
// and spit numbers.
// i will be able to use it in several files
// so, the array must start with 128
// 128 will indicate end pos
// first n bytes can be 0-127,
// and the last one must be > 128
// so 128 will always be the terminating char
//

template <class ForwardIterator_t, class num=unsigned>
class ByteArrIterator
    {
    public:
    typedef num value_type;
    typedef num& reference;
    typedef num* pointer;
    typedef std::forward_iterator_tag iterator_category;

    // default gives the end
    ByteArrIterator()
        {
//        if (std::is_pointer<ForwardIterator_t>::value)
//            {
//            _arr = nullptr;
//            }
//        else
            {
            _arr = ForwardIterator_t();
            }
        }
    ByteArrIterator(ByteArrIterator const & other)
        : _arr(other._arr)
        , _n(other._n)
        {
        }
    explicit
    ByteArrIterator(ForwardIterator_t const & arr)
        : _arr (arr)
        {
        ++*this;
        }
    ~ByteArrIterator()
        {
        }
        
    void swap(ByteArrIterator& other) noexcept {
        std::swap(_arr, other._arr);
    }
        
    ByteArrIterator&
    operator++ ()
        {
        _n = 0;
        // if the first char == 128, it's the end of the stream
        if (static_cast<unsigned char>(*_arr) == 0x80)
            {
            _arr = ForwardIterator_t();
            return *this;
            }
        
        // while char < 128
        while (*_arr & 0x80 ^ 0x80)
            {
            _n |= *_arr;
            _n <<= 7;
            ++_arr;
            }
        _n |= *_arr & 0x7F;
        ++_arr;
            
        return *this;
        }
        
    ByteArrIterator&
    operator++ (int)
        {
        ByteArrIterator r (*this);
        ++*this;
        return r;
        }

    bool
    operator== (ByteArrIterator const & other) const
        {
        // lol please forgive me
        return _arr == other._arr;
        }
        
    bool
    operator!= (ByteArrIterator const & other) const
        {
        return ! (*this == other);
        }

    num const operator* () const
        {
        return _n;
        }

    private:
    bool end;
    num _n;
    ForwardIterator_t _arr;
    };

#endif /* defined(__IR__ByteArrIterator__) */
