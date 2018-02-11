//
// Created by Oleg on 10/24/17.
//

#ifndef SORTING_OLEJA_INSERTION_SORT_H
#define SORTING_OLEJA_INSERTION_SORT_H

#ifndef NDEBUG
#define NDEBUG
#endif

#include <cassert>
#include "compare_functions.h"
#include "sort_utilities.h"
#include <iso646.h>

template<class obj>
void insertion_sort(obj *arr, unsigned len, bool (*less)(const obj &a, const obj &b) = compare) {
    assert(len > 0);

    obj* iter = arr + 1;
    obj* arrEnd = arr + len;
    obj* curr;

    while (iter != arrEnd) {
        curr = iter;
        ++iter;
        while (curr != arr and less(*curr, *(curr - 1))) {
            swap(curr, curr - 1);
            --curr;
        }
    }

    assert(isSorted(arr, arr + len, less));
}

#endif //SORTING_OLEJA_INSERTION_SORT_H
