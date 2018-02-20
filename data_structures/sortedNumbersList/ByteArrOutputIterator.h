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
    
    ByteArrOutputIterator(OutputIterator_t  & arr)
        : _arr(arr)
        , _n(0)
        {
        *_arr = 0x80;
        }
    
    ByteArrOutputIterator&
    operator=(num n)
        {
        if (n == 0) throw ;
        // iterator to _arr buf
        // so that we can write it
        // in reverse order later
        
        char* i = _arr_buf;
        char c;
        do  {
            // take first 7 bytes
            c = (n & 0x7F);
            *i = c;
            ++i;
            n >>= 7;
            } while (n > 0);
        // now i points to the one past end character of our sequence

        _arr_buf[0] |= 0x80;
        
        while (i != _arr_buf)
            {
            --i;
            *_arr = *i;
            ++_arr;
            }
        *_arr = 0x80; // terminate the stream
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
    OutputIterator_t & _arr;
    num _n;
    };

#endif
