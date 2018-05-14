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
    void search(std::string query) {
        using namespace std;
        map<string, double> hist;
        istringstream buffer(query);
        string word;
        while (buffer >> word) {
            hist[word] += 1.0;
        }
        QueryWithWeights q;
        for (auto const& p: hist)
            q.push_back({p.first, p.second});
        
//        auto ans = searcher.query(q);
        auto ans = queryBM(q);
        printResult(ans);
    }
    
    RangedQuerySearcher(Index const * index): index_(index) {}

    size_t getDocumentLength(docid_t d) {
        using namespace std;
        auto wordPositions = readIndexFromFile(index_->documentWordPositionsPath(d));
        return accumulate(
            wordPositions.begin(), wordPositions.end(), 0,
            [] (size_t acc, decltype(wordPositions)::const_reference p) {
                return acc + p.second.size();
            });
    }
    
    void printResult(SearchResults& ans) {
        using namespace std;
        cout << "size: " << ans.size() << '\n';
        sort(ans.begin(), ans.end(), COMPARE_FIELDS_LESS(DocWithRelevance const&, .doc_));
        struct DocWithRelevanceWithPath {
            DocWithRelevance dr_;
            string path_;
        };
        vector<DocWithRelevanceWithPath> res;
        {
            // TODO: hardcoded path
            ifstream lines ("/Volumes/160GB/do/wiki/long_list.txt");
            string path;
            
            docid_t n = 0;
            for (auto const& p : ans) {
                while (n != p.doc_ and getline(lines, path)) ++n;
                if (n == p.doc_) {
                    res.push_back({p, path});
                }
            }
        }
        sort(res.begin(), res.end(), COMPARE_FIELDS(DocWithRelevanceWithPath const&, .dr_.relevance_, >));
        for (auto const& p : res) {
            cout << "doc: " << p.dr_.doc_ << ", score: " << p.dr_.relevance_ << '\n';
            cout << "path: " << p.path_ << "\n\n";
        }
    }

    SearchResults queryBM(QueryWithWeights const& q) {
        using namespace std;
        static const size_t K = 10;
    
        using score_t = double;
        
        auto N = index_->collectionSize();
        unsigned long long collectionWordNumber (0);
//        collectionWordNumber = N * 400;
        for (docid_t i = 1; i < (N-1); ++i)
            collectionWordNumber += getDocumentLength(i);
        
        BM25::setCollectionSize(N);
        BM25::setAvgDocLen(collectionWordNumber / N);
        auto scoring_fun = BM25(10, 0.5);
        
        map<docid_t, score_t> scores; // map initializes double to zero
        for (WordWithWeight p: q) {
            auto df = index_->documentFrequency(p.word_);
            double idf = log(static_cast<double>(N) / df);
            index_->execForEveryDocId(p.word_,
            [&] (docid_t d) {
                auto document_length = index_->documentLength(d);
                auto tf = index_->termFreq(p.word_, d);
                scores[d] += scoring_fun(document_length, df, tf);
            });
        }

//        assert(not scores.empty());

        SearchResults res;
        res.reserve(scores.size());
        transform(scores.begin(), scores.end(), back_inserter(res),
            [] (map<docid_t, score_t>::const_reference p) {
                return DocWithRelevance {p.first, p.second};
            });
        
//        assert(res.size());

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
