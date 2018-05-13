//
//  IteratorMerger.h
//  IR
//
//  Created by User on 3/9/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_IteratorMerger_h
#define IR_IteratorMerger_h

/**
 * here's the code of STL set_intersection, with little addition: 
 * it now assigns to _OutputIterator a new object, the result of
 * _merger(obj, obj2). _merger can do something with 'equal' objects,
 * for example, merging fields-vectors of numbers in an object
 * that is compared by single sting field.
 *
 * it can also just return the first argument, 
 * then it won't differ from std::set_intersection at all.
 */
template <class _Compare, class _InputIterator1, class _InputIterator2, class _Merger, class _OutputIterator>
_OutputIterator
set_merge(_InputIterator1 __first1, _InputIterator1 __last1,
                   _InputIterator2 __first2, _InputIterator2 __last2,
                   _OutputIterator __result, _Compare __comp, _Merger __merger)
{
    while (__first1 != __last1 && __first2 != __last2)
        {
        if (__comp(*__first1, *__first2))
            ++__first1;
        else
            {
            if (!__comp(*__first2, *__first1))
                {
                *__result = __merger(*__first1, *__first2);
                ++__result;
                ++__first1;
                }
            ++__first2;
            }
        }
    return __result;
}


#endif
