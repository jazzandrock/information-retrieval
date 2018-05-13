//
//  IteratorMerger.h
//  IR
//
//  Created by User on 3/9/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_IteratorMerger_h
#define IR_IteratorMerger_h

template <class ForwardInputIteratorT, class ForwardOutputIteratorT, class value>
class Merger{
    public:
    using input = ForwardInputIteratorT const &;
    using output = ForwardOutputIteratorT &;
    Merger(
        bool (*less) (value const & a, value const & b),
        bool (*equals) (value const & a, value const & b),
        value (*ifEquals) (value const & a, value const & b)
        )
        : less_(less)
        , equals_(equals)
        , ifEquals_(ifEquals)
        {
        }
    void merge(
        input src1,
        input src1end,
        input src2,
        input src2end,
        output dest
        )
        {
        // write the first
        }
    private:
    bool (*less_) (value const & a, value const & b);
    bool (*equals_) (value const & a, value const & b);
    value (*ifEquals_) (value const & a, value const & b);
};

#endif
