//
//  WordNumberBase.cpp
//  IR
//
//  Created by User on 4/5/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <stdio.h>
#include "WordNumberBase.h"

WordNumberBase::WordNumberBase(
        std::string const & word,
        position_t position
        )
        : _word(word)
        , _positions(1, position)
        {
        }

    void
    WordNumberBase::merge(WordNumberBase * const & b)
        {
        _positions.merge(b->_positions);
        delete b;
        }

    std::string const &
    WordNumberBase::word() const
        {
        return _word;
        }
    std::forward_list<position_t> const &
    WordNumberBase::positions() const
        {
        return _positions;
        }
    bool
    WordNumberBase::operator< (
        WordNumberBase const & b) const
        {
        return _word.compare(b._word) < 0;
        }
    bool
    WordNumberBase::operator== (
        WordNumberBase const & b) const
        {
        return _word.compare(b._word) == 0;
        }