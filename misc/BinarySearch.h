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

template <class _Compare, class _ForwardIterator>
_ForwardIterator
binarySearchIter(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp)
{
    typedef typename std::iterator_traits<_ForwardIterator>::difference_type difference_type;
    difference_type __len = _VSTD::distance(__first, __last);
    while (__len != 0)
        {
        difference_type __l2 = __len / 2;
        _ForwardIterator __m = __first;
        _VSTD::advance(__m, __l2);
        if (__comp(*__m /*, __value_ */ ) < 0)
            {
            __first = ++__m;
            __len -= __l2 + 1;
            }
        else
            __len = __l2;
        }
    return __first;
}

template <class _Compare, class _ForwardIterator>
int
binarySearch(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp) {
    auto res = binarySearchIter(__first, __last, __comp);
    if (res == __last || __comp(*res)) return -1;
    return _VSTD::distance(__first, res);
}

//template <class RAIter, class ComparingFunction>
//int binarySearch(RAIter start, RAIter end, ComparingFunction comparator) {
//    if ((end - start - 1) > std::numeric_limits<int>::max())
//        throw std::runtime_error("sorry for disappointing, but this value of size_t actually won't fit into int");
//    
//    int lo = 0;
//    int hi = static_cast<int>(end - start) - 1;
//    
//    while (lo <= hi) {
//        int middle = lo + (hi - lo) / 2;
//
//        int cmp = comparator(static_cast<int>(start[middle]));
//        
//        if (cmp < 0) { lo = middle + 1; }
//        else if (cmp > 0) { hi = middle - 1; }
//        else { return middle; }
//    }
//    
//    return -1;
//}


#endif
