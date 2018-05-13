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

class RangedQuerySearcher {
    public:
    RangedQuerySearcher(Index const * index): index_(index) {}

    size_t getDocumentLength(docid_t d) {
        using namespace std;
        auto wordPositions = readIndexFromFile("/Volumes/160GB/do/db/pos/" + to_string(d) + ".wrdps");
        return accumulate(
            wordPositions.begin(), wordPositions.end(), 0,
            [] (size_t acc, decltype(wordPositions)::const_reference p) {
                return acc + p.second.size();
            });
    }

    SearchResults queryBM(QueryWithWeights const& q) {
        using namespace std;
        static const size_t K = 10;
    
        using score_t = double;
        
        auto N = index_->collectionSize();
        unsigned long long collectionWordNumber (N * 400);
//        for (docid_t i = 0; i != N / 2; ++i)
//            collectionWordNumber += getDocumentLength(i);
        BM25::setCollectionSize(N);
        BM25::setAvgDocLen(collectionWordNumber / N);
        auto scoring_fun = BM25(10, 0.5);
        
        map<docid_t, score_t> scores; // map initializes double to zero
        for (WordWithWeight p: q) {
            auto df = index_->documentFrequency(p.word_);
            double idf = log(static_cast<double>(N) / df);
            
            index_->execForEveryDocId(p.word_,
            [&] (docid_t d) {
                auto wordPositions = readIndexFromFile("/Volumes/160GB/do/db/pos/" + to_string(d) + ".wrdps");
                size_t document_length = accumulate(
                    wordPositions.begin(), wordPositions.end(), 0,
                    [] (size_t acc, decltype(wordPositions)::const_reference p) {
                        return acc + p.second.size();
                    });
//                scores[d] += p.weight_ * idf * tf / document_length;
                size_t tf = wordPositions[p.word_].size();
                scores[d] += scoring_fun(document_length, df, tf);
            });
        }

        assert(not scores.empty());

        SearchResults res;
        res.reserve(scores.size());
        transform(scores.begin(), scores.end(), back_inserter(res),
            [] (map<docid_t, score_t>::const_reference p) {
                return DocWithRelevance {p.first, p.second};
            });
        
        assert(res.size());

        return move(selectTopK<DocWithRelevance>(
            res.begin(), res.end(), K,
            [] (DocWithRelevance const& lhs, DocWithRelevance const& rhs) -> bool {
                return lhs.relevance_ > rhs.relevance_;
            }));
    }

    
    SearchResults query(QueryWithWeights const& q) {
        using namespace std;
        static const size_t K = 10;
    
        using score_t = double;
        
        map<docid_t, score_t> scores; // map initializes double to zero
        auto N = index_->collectionSize();
        for (WordWithWeight p: q) {
            auto df = index_->documentFrequency(p.word_);
            double idf = log(static_cast<double>(N) / df);
            
            index_->execForEveryDocId(p.word_,
            [&] (docid_t d) {
                auto wordPositions = readIndexFromFile("/Volumes/160GB/do/db/pos/" + to_string(d) + ".wrdps");
                size_t document_length = accumulate(
                    wordPositions.begin(), wordPositions.end(), 0,
                    [] (size_t acc, decltype(wordPositions)::const_reference p) {
                        return acc + p.second.size();
                    });
                size_t tf = wordPositions[p.word_].size();
                scores[d] += p.weight_ * idf * tf / document_length;
            });
        }

        assert(not scores.empty());

        SearchResults res;
        res.reserve(scores.size());
        transform(scores.begin(), scores.end(), back_inserter(res),
            [] (map<docid_t, score_t>::const_reference p) {
                return DocWithRelevance {p.first, p.second};
            });
        
        assert(res.size());

        return move(selectTopK<DocWithRelevance>(
            res.begin(), res.end(), K,
            [] (DocWithRelevance const& lhs, DocWithRelevance const& rhs) {
                return lhs.relevance_ > rhs.relevance_;
            }));
    }
    
    private:
    Index const * index_;
    QueryWithWeights pair_;
};

#endif
