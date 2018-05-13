//
//  VBUtils.h
//  IR
//
//  Created by User on 4/8/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_VBUtils_h
#define IR_VBUtils_h

#include <cstdlib>
#include "NullOutputIterator.h"

namespace VB {

class Utils {
    public:
    template <class InIter, class OutIter>
    static size_t copyInnerData(InIter innerDataIter, OutIter out) {
        size_t len (0);
        unsigned char c = *innerDataIter;
        auto incrementAll = [&] () { *out++ = c; ++len; ++innerDataIter; c = *innerDataIter; };
        while (c != 128) {
            while ((c & 128) == 0) {
                incrementAll();
            }
            incrementAll();
        }
        *out++ = 128; ++len; // ++innerDataIter; c = *innerDataIter;
        return len;
    }
    
    template <class InIter>
    static size_t lengthOfInnerData(InIter innerDataIter) {
        using namespace misc;
        return copyInnerData(innerDataIter, NullOutputIterator());
    }
};

}

#endif
