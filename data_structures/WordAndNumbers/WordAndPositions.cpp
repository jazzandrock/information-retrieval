//
//  WordAndPositions.cpp
//  IR
//
//  Created by User on 4/5/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <stdio.h>
#include "WordAndPositions.h"

WordAndPositions::WordAndPositions(
    std::string const & word,
    position_t position)
    : WordNumberBase(word, position)
    {
    }

std::ostream& operator<< (std::ostream& stream, WordAndPositions const& wordPositions) {
    using namespace std;
    stream << "WordAndPositions: " << wordPositions.word() << '\n';
    stream << "positions: ";
    copy(wordPositions.positions().begin(), wordPositions.positions().end(),
        (ostream_iterator<position_t>(stream, " ")));
//    for (position_t pos: wordPositions.positions()) { stream << pos << ' '; }
    stream << endl;
    return stream;
}

bool
wordAndPositionsLess(
    WordAndPositions *const & a,
    WordAndPositions *const & b)
    {
    return *a < *b;
    }

bool
wordAndPositionsEquals(
    WordAndPositions *const & a,
    WordAndPositions *const & b)
    {
    return *a == *b;
    }