//
//  BM25.cpp
//  IR
//
//  Created by User on 4/16/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include "BM25.h"
//#include "RangedQuery.h"
#include <cassert>
#include <cmath>

size_t BM25::avgDocLen (0);
size_t BM25::collectionSize(0);

void BM25::setAvgDocLen(size_t newLen) { BM25::avgDocLen = newLen; }
void BM25::setCollectionSize(size_t newLen) { BM25::collectionSize = newLen; }

BM25::BM25(unsigned k, double b): k_(k), b_(b) {
    if (not (0 <= b_ and b_ <= 1)) throw std::runtime_error("b must be in [0...1]");
}

double BM25::operator() (unsigned doc_len, unsigned df, unsigned tf) {
    return log(double(collectionSize) / df)
        * (
            (k_+1)*tf
            /
            k_*(
                (1 - b_) + b_*(
                    doc_len / avgDocLen
                ) + tf
            )
        );
}



