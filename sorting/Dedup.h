//
//  Dedup.h
//  IR
//
//  Created by User on 2/11/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_Dedup2_h
#define IR_Dedup2_h

#include <stdlib.h>
#include <cassert>
#include <signal.h>

template <typename T>
class Dedup
    {
    public:
    Dedup(
    bool (*equals)(T const & a, T const & b),
    bool (*less)(T const & a, T const & b),
    void (*doSomethingWithEqualElements)(T & a, T const & b))
        : _aux(initial_size)
        , _curr_size(initial_size)
        , equals(equals)
        , less(less)
        , doSomethingWithEqualElements(doSomethingWithEqualElements)
        {
        }
    
    ~Dedup() = default;
    
    size_t
    sortDedup(T* arr, size_t start, size_t end)
        {
        // > instead of <
        // не той код неправильний пароль в космосі аварія екіпаж загине
        // другий раз, в тому ж файлі. ех
        if ((end - start) > _curr_size) {
            _curr_size = end - start;
            _aux.reserve(_curr_size);
        }
        return sortDedupHelper(arr, start, end) - start;
        }

    private:
    // so that i can't copy it
    Dedup(Dedup const & d) = default;
    
    // initial size of container
    static size_t const initial_size = 1000000;
    
    std::vector<T> _aux;
    size_t _curr_size;
    bool (*equals)(T const & a, T const & b);
    bool (*less)(T const & a, T const & b);
    void (*doSomethingWithEqualElements)(T & a, T const & b);
    
    bool
    lessEquals(T const & a, T const & b)
        {
        return not less(b, a);
        }
    
    size_t
    sortDedupHelper(T arr[], size_t start, size_t end)
        {
        assert (start < end);
        if (end - start == 1) return end;

        size_t center = (start + end) >> 1;
        size_t firstPartEndIdx = sortDedupHelper(arr, start, center);
        size_t secondPartEndIdx = sortDedupHelper(arr, center, end);

        std::copy(arr + start, arr + end, &_aux[0] + start);
        return merge(
                &_aux[0], start, firstPartEndIdx,
                &_aux[0], center, secondPartEndIdx,
                arr, start
            );
        }

    // return new end position
    size_t
    merge(T src1[], size_t start1, size_t end1, T src2[], size_t start2, size_t end2, T dest[], size_t destStart)
        {
        size_t src1Idx = start1;
        size_t src2Idx = start2;
        size_t destIdx = destStart;

        // we want the first element to be present before the loop starts
        dest[destIdx++] = lessEquals(src1[src1Idx], src2[src2Idx]) ? src1[src1Idx++] : src2[src2Idx++];

        T less;
        while (src1Idx != end1 || src2Idx != end2)
            {
            if (src1Idx < end1 && src2Idx < end2)
                {
                // < instead of <=
                // не той код неправильний пароль в космосі аварія екіпаж загине
                less = lessEquals(src1[src1Idx], src2[src2Idx]) ? src1[src1Idx++] : src2[src2Idx++];
                }
            else if (src1Idx < end1)
                less = src1[src1Idx++];
            else
                less = src2[src2Idx++];

            // compare last existing element with next
            if (equals(dest[destIdx - 1], less))
                doSomethingWithEqualElements(dest[destIdx - 1], less);
            else
                dest[destIdx++] = less;
            }

        return destIdx;
        };
    };


#endif
