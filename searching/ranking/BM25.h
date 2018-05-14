//
//  BM25.h
//  IR
//
//  Created by User on 4/16/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__BM25__
#define __IR__BM25__
#include "defs.h"

#include <stdio.h>
class BM25 {
    public:
    BM25(unsigned k, double b);
    double operator() (unsigned doc_len, unsigned df, unsigned tf);
    
    static void setAvgDocLen(size_t newLen);
    static void setCollectionSize(size_t newLen);
    
    private:
    static size_t collectionSize;
    static size_t avgDocLen;
    
    unsigned k_; // калібрується частота терміну (???:)
    double b_; // result scales with doc
};

#endif /* defined(__IR__BM25__) */
