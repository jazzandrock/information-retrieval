//
//  VBGapInputIterator.h
//  IR
//
//  Created by User on 3/8/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_VBGapInputIterator_h
#define IR_VBGapInputIterator_h

#include "VBInputIterator.h"

template <class ForwardIterator_t, class num_t>
class VBGapInputIterator
    : public boost::equality_comparable<VBGapInputIterator<ForwardIterator_t, num_t>&>
    , public boost::incrementable<VBGapInputIterator<ForwardIterator_t, num_t>>
    {
    public:
    using difference_type = typename VBInputIterator<ForwardIterator_t, num_t>::difference_type;
    using reference = typename VBInputIterator<ForwardIterator_t, num_t>::reference;
    using pointer = typename VBInputIterator<ForwardIterator_t, num_t>::pointer;
    using value_type = typename VBInputIterator<ForwardIterator_t, num_t>::value_type;
    using iterator_category = typename VBInputIterator<ForwardIterator_t, num_t>::iterator_category;
    
    VBGapInputIterator() = default; // end
    VBGapInputIterator(VBGapInputIterator const & src) = default;
    VBGapInputIterator & operator=(VBGapInputIterator const & src) = default;
//    VBGapInputIterator & operator=(VBGapInputIterator const & src) {
//        iter_ = src.iter_;
//        return *this;
//    };
    VBGapInputIterator(ForwardIterator_t const & data): iter_(data), current_(*iter_) {}
    bool operator== (VBGapInputIterator const & other) { return iter_ == other.iter_; }
    num_t operator*() { return current_; }
    VBGapInputIterator & operator++() {
        ++iter_;
        current_ += *iter_;
        return *this;
    }

    private:
    VBInputIterator<ForwardIterator_t, num_t> iter_;
    num_t current_;
    };

#endif
