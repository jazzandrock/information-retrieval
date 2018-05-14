//
//  RangedQuery.cpp
//  IR
//
//  Created by User on 5/15/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <stdio.h>
#include "RangedQuerySearcher.h"


void RangedQuerySearcher::search(std::string query) {
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

size_t RangedQuerySearcher::getDocumentLength(docid_t d) {
    using namespace std;
    auto wordPositions = readIndexFromFile(index_->documentWordPositionsPath(d));
    return accumulate(
                      wordPositions.begin(), wordPositions.end(), 0,
                      [] (size_t acc, decltype(wordPositions)::const_reference p) {
                          return acc + p.second.size();
                      });
}

void RangedQuerySearcher::printResult(SearchResults& ans) {
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
SearchResults RangedQuerySearcher::queryBM(QueryWithWeights const& q) {
    auto N = index_->collectionSize();
    unsigned long long collectionWordNumber (0);
    collectionWordNumber = N * 400;
    //        for (docid_t i = 1; i < (N-1); ++i)
    //            collectionWordNumber += getDocumentLength(i);
    
    BM25::setCollectionSize(N);
    BM25::setAvgDocLen(collectionWordNumber / N);
    auto scoring_fun_bm = BM25(10, 0.5);
    auto scoring_fun = [&] (size_t document_length, size_t df, size_t tf, double idf) {
        return scoring_fun_bm(document_length, df, tf);
    };
    return queryWithScoringFunction(q, scoring_fun);
}

SearchResults RangedQuerySearcher::queryWithScoringFunction(QueryWithWeights const& q, std::function<double(size_t, size_t, size_t, double)> scoring_fun) {
    using namespace std;
    static const size_t K = 10;
    
    using score_t = double;
    
    auto N = index_->collectionSize();
    
    map<docid_t, score_t> scores; // map initializes double to zero
    for (WordWithWeight p: q) {
        auto df = index_->documentFrequency(p.word_);
        double idf = log(static_cast<double>(N) / df);
        index_->execForEveryDocId(p.word_,
              [&] (docid_t d) {
                  auto document_length = index_->documentLength(d);
                  auto tf = index_->termFreq(p.word_, d);
                  auto score = scoring_fun(document_length, df, tf, idf);
                  scores[d] = p.weight_ * (scores[d] + score + scores[d] * score);
              });
    }
    
    SearchResults res;
    
    using doc_score_t = pair<docid_t, score_t>;
    vector<doc_score_t> topResults =
    selectTopK<doc_score_t>(
                            scores.begin(), scores.end(), K,
                            COMPARE_FIELDS(doc_score_t const&, .second, >));
    
    res.reserve(scores.size());
    transform(topResults.begin(), topResults.end(), back_inserter(res),
              [] (doc_score_t p) {
                  return DocWithRelevance {p.first, p.second};
              });
    
    return move(res);
}


SearchResults RangedQuerySearcher::query(QueryWithWeights const& q) {
    return queryWithScoringFunction(q,
        [] (size_t document_length, size_t df, size_t tf, double idf) {
            return idf * tf / document_length;
        });
}