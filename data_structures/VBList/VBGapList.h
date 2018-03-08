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

template <typename num_t>
class VBGapList
    {
    public:
    void
    push_back(num_t n)
        {
        if (_biggest_number >= n) throw ;
        _list.push_back(n - _biggest_number);
        _biggest_number = n;
        }
    
    private:
    VBList<num_t> _list;
    num_t _biggest_number;
    }

#endif
