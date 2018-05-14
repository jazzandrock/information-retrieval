//
//  RangedQuery.h
//  IR
//
//  Created by User on 4/13/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_RangedQuery_h
#define IR_RangedQuery_h
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <numeric>
#include "Index.h"
#include "IndexIterator.h"
#include <map>
#include "IndexMerging.h"
#include "HeapTopKSelection.h"
#include "BM25.h"
#include "defs.h"
#include <map>

class RangedQuerySearcher {
    public:
    void search(std::string query);
    
    RangedQuerySearcher(Index const * index): index_(index) {}

    size_t getDocumentLength(docid_t d);
    
    void printResult(SearchResults& ans);
    SearchResults queryBM(QueryWithWeights const& q);

    SearchResults queryWithScoringFunction(QueryWithWeights const& q, std::function<double(size_t, size_t, size_t, double)> scoring_fun);

    
    SearchResults query(QueryWithWeights const& q);
    
    
    private:
    Index const * index_;
    QueryWithWeights pair_;
};

#endif
