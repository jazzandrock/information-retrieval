//
//  WordNumberBase.h
//  IR
//
//  Created by User on 2/17/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__WordNumberBase__
#define __IR__WordNumberBase__

#include <stdio.h>
#include <iostream>
#include <string>
#include <forward_list>
#include "defs.h"

class
WordNumberBase
    {
    public:
    WordNumberBase(
        std::string const & word,
        position_t position);
        
    ~WordNumberBase() = default;
    
    void
    merge(WordNumberBase * const & b);
    std::string const &
    word() const;
    
    std::forward_list<position_t> const &
    positions() const;
    
    bool
    operator< (
        WordNumberBase const & b) const;
    bool
    operator== (
        WordNumberBase const & b) const;
        
    private:
    std::string _word;
    std::forward_list<position_t> _positions;
    };

#endif /* defined(__IR__WordNumberBase__) */
