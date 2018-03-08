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

template <class InputIterator, typename num>
class IndexIterator {
public:
    using item = std::pair<std::string, VBGapInputIterator<std::vector<char>::iterator, num>>;
    
    
    IndexIterator &
    operator++()
        {
        // first, read string len
        size_t word_len = *data_iter();
        std::string word;
        word.reserve(word_len);
        std::copy_n(data_iter(), word_len, word.begin());
        // read all the input
        char c;
        std::vector<char>::iterator inner_data_iter = data_.begin();
        while ((c = *data_iter()) != -128)
            {
            if (inner_data_iter == data_.end())
                {
                // to ensure that everything will be copied on reserve()
                data_.resize(inner_data_iter - data_.begin());
                data_.reserve(data_.capacity() * 2);
                }
            *inner_data_iter = c;
            ++inner_data_iter;
            ++data_iter_;
            }
        *inner_data_iter = -128;
        ++data_iter_; // move to next position
        // how do you know it's the end?
        // probably write zero in the very end of the stream
        
        if (*data_iter_ == 0) end_ = true;
        
        current_ = item(std::move(word), VBGapInputIterator<std::vector<char>::iterator, num>(data_.begin()));
        }
    item const &
    operator*()
        {
        // will it return pair (if so, VBGapList needs to be copyconstructible)
        // or maybe just reference? reference
        return current_;
        }
private:
    inline InputIterator & data_iter() { return data_iter_; }
    item current_;
    std::vector<char> data_;
    InputIterator /*&*/ data_iter_;
    bool end_;
};

#endif
