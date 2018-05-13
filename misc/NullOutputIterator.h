//
//  NullOutputIterator.h
//  IR
//
//  Created by User on 4/13/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_NullOutputIterator_h
#define IR_NullOutputIterator_h


struct NullOutputIterator: std::iterator<std::output_iterator_tag, NullOutputIterator> {
    /* no-op assignment */
    template<typename T>
    void operator=(T const&) { }
    NullOutputIterator& operator++() { return *this; }
    NullOutputIterator  operator++(int) { return *this; }
    NullOutputIterator& operator*() { return *this; }
};



#endif
