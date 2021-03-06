//
//  WordAndPositions.h
//  IR
//
//  Created by User on 2/17/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_WordAndPositions_h
#define IR_WordAndPositions_h
#include "WordNumberBase.h"
#include "defs.h"

class
WordAndPositions: public WordNumberBase {
    public:
    WordAndPositions(std::string const & word, position_t position);
};

std::ostream & operator<< (std::ostream & stream, WordAndPositions const & wordPositions);
bool wordAndPositionsLess(WordAndPositions *const & a, WordAndPositions *const & b);
bool wordAndPositionsEquals(WordAndPositions *const & a, WordAndPositions *const & b);

#endif
