    //
//  IndexIterator.h
//  IR
//
//  Created by User on 3/8/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_IndexIterator_h
#define IR_IndexIterator_h

#include <algorithm>
#include "VBGapInputIterator.h"
#include "VBUtils.h"
#include <boost/operators.hpp>
#include "utils.h"

template <class InputIterator_t, typename num_t>
class IndexIterator
: public boost::equality_comparable<IndexIterator<InputIterator_t, num_t>&> // wtf why &, w/o doesn't work
//    , public boost::incrementable< IndexIterator<InputIterator_t, num_t> >
{
public:
    using item = std::pair<std::string, VBGapInputIterator<std::vector<char>::iterator, num_t>>;
    using value_type = item;
    
    IndexIterator()
    : end_(true)
    {
    }

    IndexIterator(InputIterator_t const& data_iter)
    : data_iter_(data_iter)
//    , data_(10) // TODO: is there difference betweeen (10) and .reserve(10)?
    , end_(false) // no way
    // lool
    // I didn't initialize end_, and sometimes happened to be initialized to true (garbage)
    // and that caused exception in mergeIndexes()
    {
        data_.reserve(10);
        operator++();
    }
    IndexIterator& operator++() {
        using namespace std;
        if (*data_iter_ == 0) {
            end_ = true;
            return *this;
        }
        
        // first, read string len
        size_t word_len = static_cast<unsigned char>(*data_iter());
        ++(data_iter());
        std::string word;
        word.resize(word_len);
        // unfortunately, this doesn't work
        // copy_n(data_iter_, word_len, (word.begin()));
        for (unsigned i = 0; i != word_len; ++i) {
            word[i] = *data_iter();
            ++(data_iter());
        }
        data_.resize(0);
        VB::Utils::copyInnerData<InputIterator_t &>(data_iter(), back_inserter(data_));
        ++data_iter(); // move to next position
        // how do you know it's the end?
        // probably write zero in the very end of the stream
        current_.first = move(word);
        current_.second = VBGapInputIterator<std::vector<char>::iterator, num_t>(data_.begin());
        return *this;
    }
    item const & operator*() { return current_; }
    item const * operator->() { return &current_; }
    bool operator==(IndexIterator const & other) { return end_; }
    bool end() { return end_; }
    
private:
    inline InputIterator_t & data_iter() { return data_iter_; }
    InputIterator_t /*&*/ data_iter_;
    std::vector<char> data_;
    item current_;
    bool end_;
};

#endif
