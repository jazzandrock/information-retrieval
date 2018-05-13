//
//  defs.h
//  IR
//
//  Created by User on 2/16/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include "IndexIterator.h"
#include "VBGapList.h"
#include <string>
#include "streamOutput.h"

#ifndef IR_defs_h
#define IR_defs_h

typedef unsigned long long docid_t;
typedef unsigned position_t;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(TypeName const&) = delete;        \
void operator=(TypeName const &) = delete;

using WordToIDsMap = std::map<std::string, VBGapList<docid_t>>;
using StringIndexIterator = IndexIterator<std::string::const_iterator, docid_t>;

template <class T1, class T2>
using VectorsPair = std::pair<std::vector<T1>, std::vector<T2>>;

//using QueryWithWeights = VectorsPair<std::string, double>;
struct WordWithWeight {
    std::string word_;
    double weight_;
};
using QueryWithWeights = std::vector<WordWithWeight>;
struct DocWithRelevance {
    docid_t doc_;
    double relevance_;
};
using SearchResults = std::vector<DocWithRelevance>;
using StringIndexIterator = IndexIterator<std::string::const_iterator, docid_t>;



#endif
