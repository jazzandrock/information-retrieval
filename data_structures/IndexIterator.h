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
#include <boost/operators.hpp>

template <class InputIterator_t, typename num_t>
class IndexIterator
    : public boost::equality_comparable<IndexIterator<InputIterator_t, num_t>&> // wtf why &, w/o doesn't work
//    , public boost::incrementable< IndexIterator<InputIterator_t, num_t> >
    {
    public:
    using item = std::pair<std::string, VBGapInputIterator<std::vector<char>::iterator, num_t>>;
    
    IndexIterator()
        : end_(true)
        {
        }
    IndexIterator(InputIterator_t const & data_iter)
        : data_iter_(data_iter)
        , data_(10)
        {
        operator++();
        }
    IndexIterator &
    operator++()
        {
        // first, read string len
        size_t word_len = *data_iter();
                        ++(data_iter());
        std::string word;
        word.resize(word_len);
//        std::copy_n(data_iter(), word_len, word.begin());
        for (unsigned i = 0; i != word_len; ++i)
            {
            word[i] = *data_iter();
            ++(data_iter());
            }
//        ++(data_iter()); // TODO wtf
        // read all the input
        char c;
        data_.resize(0);
//        std::vector<char>::iterator inner_data_iter = data_.begin();
        while ((c = *data_iter()) != -128)
            {
            data_.push_back(c);
//            if (inner_data_iter == data_.end())
//                {
//                // to ensure that everything will be copied on reserve()
//                data_.resize(inner_data_iter - data_.begin());
//                data_.reserve(data_.capacity() * 2);
//                inner_data_iter = data_.begin();
//                }
//            *inner_data_iter = c;
//            ++inner_data_iter;
            ++data_iter_;
            }
        data_.push_back(-128);
        ++data_iter_; // move to next position
        // how do you know it's the end?
        // probably write zero in the very end of the stream
        
        if (*data_iter_ == 0)
            {
            end_ = true;
            }

//        current_ = item(std::move(word), VBGapInputIterator<std::vector<char>::iterator, num_t>(data_.begin()));
        current_.first = word;
        current_.second = VBGapInputIterator<std::vector<char>::iterator, num_t>(data_.begin());
        return *this;
        }
    item const &
    operator*()
        {
        return current_;
        }
    bool operator==(IndexIterator const & other)
        {
        return end_;
        }
    
    private:
    inline InputIterator_t & data_iter() { return data_iter_; }
    InputIterator_t /*&*/ data_iter_;
    std::vector<char> data_;
    item current_;
    bool end_;
    };

#endif
