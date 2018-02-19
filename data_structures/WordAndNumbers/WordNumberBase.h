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

class
WordNumberBase
    {
    public:
    WordNumberBase(
        std::string const & word,
        unsigned position
        )
        : _word(word)
        , _positions(1, position)
        {
//        ++freeID;
        }
    
    ~WordNumberBase()
        {
//        ++deletedNum;
        }
    
    void
    merge(WordNumberBase * const & b)
        {
        _positions.merge(b->_positions);
        delete b;
        }
    
    std::string const &
    word() const
        {
        return _word;
        }

    std::forward_list<unsigned> const &
    positions() const
        {
        return _positions;
        }
        
    bool
    operator< (
        WordNumberBase const & b) const
        {
        return _word.compare(b._word) < 0;
        }
        
    bool
    operator== (
        WordNumberBase const & b) const
        {
        return _word.compare(b._word) == 0;
        }

//    static size_t createdCount() { return freeID; }
//    static size_t deletedCount() { return deletedNum; }
    
    private:
//    static size_t freeID;
//    static size_t deletedNum;
    std::string _word;
    std::forward_list<unsigned> _positions;
    };

#endif /* defined(__IR__WordNumberBase__) */
