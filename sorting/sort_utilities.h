//
// Created by Oleg on 10/24/17.
//

#ifndef SORTING_SORT_UTILITIES_H
#define SORTING_SORT_UTILITIES_H

#ifndef NDEBUG
#define NDEBUG
#endif

#include <cassert>
#include <iostream>
#include <algorithm>
#include "compare_functions.h"

template<class obj>
inline bool isSorted(const obj* arr, const obj* arrEnd, bool (*less)(const obj &a, const obj &b)) {
    assert(arr < arrEnd);

    while (++arr != arrEnd)
        if (less(*arr, *(arr - 1)))
            return false;

    return true;
}

template<class obj>
inline void swap(obj *a, obj *b) {
    obj c = *a;
    *a = *b;
    *b = c;
}

// well, if sortedArr consists of all elements of arr,
// then arr consists of sortedArr as well
// so we will find every element of unsorted array
// in the sorted one using binary search
template<class obj>
bool setsEqual(const obj* arr, const obj* sortedArr, const unsigned len, bool (*less)(const obj &a, const obj &b) = compare) {
    assert(isSorted(sortedArr, sortedArr + len, less));
    for (const obj* arrEnd = arr + len; arr != arrEnd; arr++)
        if (not std::binary_search(sortedArr, sortedArr + len, *arr))
            return false;

    return true;
}

template<class obj>
inline void printArr(const obj* arr, const obj* arrEnd) {
    assert(arr <= arrEnd);
    while (arr != arrEnd) {
        std::cout << *arr << ' ';
        ++arr;
    }
    std::cout << '\n';
}

#endif //SORTING_SORT_UTILITIES_H
