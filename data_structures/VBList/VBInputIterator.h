//
//  VBInputIterator.h
//  IR
//
//  Created by User on 2/18/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__VBInputIterator__
#define __IR__VBInputIterator__

#include <stdio.h>
#include <iterator>
#include <type_traits>
#include <boost/operators.hpp>

template <class ForwardIterator_t, class num_t>
class VBInputConstIterator;

template <class ForwardIterator_t, class num_t>
class VBInputIterator
    : public boost::equality_comparable<VBInputIterator<ForwardIterator_t, num_t>&> // wtf why &, w/o doesn't work
    , public boost::incrementable<VBInputIterator<ForwardIterator_t, num_t>>
    {
    static_assert(std::is_unsigned<num_t>::value, "num_t must be unsigned integer type");
    public:
    typedef size_t difference_type;
    typedef num_t value_type;
    typedef num_t& reference;
    typedef num_t* pointer;
    typedef std::forward_iterator_tag iterator_category;

    // default gives the end
    // see operator==
    VBInputIterator()
        : _end(true)
        {
        }
    VBInputIterator(VBInputIterator const & other) = default;
    VBInputIterator & operator=(VBInputIterator const & src)
        {
        _n = src._n;
        _in = src._in;
        _end = src._end;
        return *this;
        }

    explicit
    VBInputIterator(ForwardIterator_t const & arr)
        : _in (arr)
        , _end(false)
        {
        ++*this;
        }
    ~VBInputIterator()
        {
        }
    VBInputIterator&
    operator++ ()
        {
        _n = 0;
        // if the first char == 128, it's the end of the stream
        if (static_cast<unsigned char>(*_in) == 0x80)
            {
            _end = true;
            return *this;
            }
        
        while ((*_in & 0x80) != 0x80) {
            _n |= *_in;
            _n <<= 7;
            ++_in;
        }
        _n |= *_in & 0x7F;
        ++_in;
            
        return *this;
        }
        
    bool
    operator== (VBInputIterator const & other) const
        {
        return _end;
        }
    
    num_t const
    operator* () const
        {
        return _n;
        }

    bool
    end() const
        {
        return _end;
        }

    private:
    num_t _n; // current number iterator points to
    ForwardIterator_t _in;
    bool _end; // see operator==
    };

#endif /* defined(__IR__VBInputIterator__) */
