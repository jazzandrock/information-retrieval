//
// Created by Oleg on 10/24/17.
//

#ifndef OLEJA_MERGE_SORT_H
#define OLEJA_MERGE_SORT_H

#ifndef NDEBUG
#define NDEBUG
#endif

#include <cassert>
#include <algorithm>
#include "oleja_insertion_sort.h"
#include "compare_functions.h"
#include "sort_utilities.h"

// if array is shorter than 7
// then insertion sort is faster than merge
static const unsigned LENGTH_FOR_INSERTION = 7;

template<class obj>
static void oleja_merge(obj *dest, obj *first_iter, obj *second_iter, obj *first_end, obj *second_end, bool (*less)(const obj &a, const obj &b)) {
    assert (first_iter < first_end);
    assert (first_end <= second_iter);
    assert (second_iter < second_end);

    while (first_iter != first_end and second_iter != second_end)
        if (less(*second_iter, *first_iter))
            *dest++ = *second_iter++;
        else
            *dest++ = *first_iter++;

    while (first_iter != first_end)
        *dest++ = *first_iter++;

    while (second_iter != second_end)
        *dest++ = *second_iter++;
}

template<class obj>
void oleja_merge_sort(obj *arr, const unsigned len, bool (*less)(const obj &a, const obj &b), obj *aux) {
    assert (len > 0);

    if (len < LENGTH_FOR_INSERTION) {
        insertion_sort(arr, len, less);
        return;
    }

    unsigned firstPartLen  = len >> 1;
    unsigned secondPartLen = len - firstPartLen;

    oleja_merge_sort(arr, firstPartLen, less, aux);
    oleja_merge_sort(arr + firstPartLen, secondPartLen, less, aux);

    std::copy(arr, arr + len, aux);

    oleja_merge(arr, aux, aux + firstPartLen, aux + firstPartLen, aux + len, less);

    assert(isSorted(arr, arr + len, less));
}

template<class obj>
inline void oleja_merge_sort(obj *arr, const unsigned len, bool (*less)(const obj &a, const obj &b) = compare) {
    obj* aux = new obj[len];
    oleja_merge_sort(arr, len, less, aux);
    delete[] aux;
}

#endif //OLEJA_MERGE_SORT_H





