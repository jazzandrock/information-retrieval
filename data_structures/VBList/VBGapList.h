//
//  VBGapList.h
//  IR
//
//  Created by User on 3/4/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_VBGapList_h
#define IR_VBGapList_h

#include "VBList.h"
#include "choose.h"

template <typename num_t, bool is_const=false>
class VBGapListIterator
    {
    public:
    using value_type = num_t;
    using reference = typename choose<is_const, num_t const &, num_t &>::type;
    using pointer = typename choose<is_const, num_t const *, num_t *>::type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using iter_t = typename choose<is_const, typename VBList<num_t>::const_iterator, typename VBList<num_t>::iterator>::type;
    
    
    VBGapListIterator(VBGapListIterator<num_t, false> const & src)
        : VBGapListIterator(src._iter)
        {
        }
        
    VBGapListIterator(iter_t const & iter)
        : _iter(iter) // create a copy
        , _n(*_iter)
        {
        }
        
    VBGapListIterator&
    operator++()
        {
        ++_iter;
        _n += *_iter;
        return *this;
        }
    VBGapListIterator
    operator++(int)
        {
        VBGapListIterator prev(*this);
        ++*this;
        return prev;
        }
    num_t
    operator*()
        {
        return _n;
        }
    bool
    operator== (VBGapListIterator const & other) const
        {
        return _iter == other._iter;
        }
    bool
    operator!= (VBGapListIterator const & other) const
        {
        return not (*this == other);
        }
    private:
    iter_t _iter;
    num_t _n;
    };


template <typename num_t>
class VBGapList
    {
    public:
    typedef VBGapListIterator<num_t> iterator;
    typedef VBGapListIterator<num_t, true> const_iterator;
    
    VBGapList()
        : _biggest_number(0)
        {
        }
    const_iterator
    begin() const
        {
        return const_iterator(_list.begin());
        }
    const_iterator
    end() const
        {
        return const_iterator(_list.end());
        }
    iterator
    begin()
        {
        return iterator(_list.begin());
        }
    iterator
    end()
        {
        return iterator(_list.end());
        }
    void
    push_back(num_t n)
        {
        if (_biggest_number >= n)
            {
//             throw std::invalid_argument("expected T_PAAMAYIM_NEKUDOTAYIM");
            throw std::invalid_argument("push_back in VBGapList: n must be greater than last element.");
            }
        _list.push_back(n - _biggest_number);
        _biggest_number = n;
        }
    size_t
    size()
        {
        return _list.size();
        }
    // if you want to write the numbers to file
    // you can just copy all the _vector
    // instead of decoding it to numbers
    // and then encoding back to write them
    std::vector<char> const &
    inner_data() const
        {
        return _list.inner_data();
        }

    private:
    VBList<num_t> _list;
    num_t _biggest_number;
    };

#endif
