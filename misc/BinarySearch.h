//
//  BinarySearch.h
//  IR
//
//  Created by User on 4/13/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_BinarySearch_h
#define IR_BinarySearch_h
#include <cassert>
#include <limits>
namespace misc {
template <class RAIter, class ComparingFunction>
int binarySearch(RAIter start, RAIter end, ComparingFunction comparator) {
    if ((end - start - 1) > std::numeric_limits<int>::max())
        throw std::runtime_error("sorry for disappointing, but this value of size_t actually won't fit into int");
    
    int lo = 0;
    int hi = static_cast<int>(end - start) - 1;
    
    while (lo <= hi) {
        int middle = lo + (hi - lo) / 2;

        int cmp = comparator(static_cast<int>(start[middle]));
        
        if (cmp < 0) { lo = middle + 1; }
        else if (cmp > 0) { hi = middle - 1; }
        else { return middle; }
    }
    
    return -1;
}
}

#endif
