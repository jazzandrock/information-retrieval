//
//  Index.cpp
//  IR
//
//  Created by User on 4/13/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "Index.h"
#include "NullOutputIterator.h"
#include "IndexMerging.h"
#include "set_merge.h"
#include <cmath>


Index::score_t Index::cosineSimilarity(docid_t d1, docid_t d2) const {
    using namespace std;
    string d1wrdps = copyFileIntoString(documentWordPositionsPath(d1));
    string d2wrdps = copyFileIntoString(documentWordPositionsPath(d2));
    using Iter = IndexIterator<string::const_iterator, docid_t>;
    Iter d1iter (d1wrdps.begin());
    Iter d2iter (d2wrdps.begin());
    Iter end;
    
    static auto const square = [] (double x) { return x*x; };
    
    score_t res (0);
    double a_length (0);
    double b_length (0);
    
    using Item = Iter::value_type const &;
    auto compareIters = [] (Item a, Item b) { return a.first < b.first; };
    auto mergeIters = [&] (Item a, Item b) {
        assert(a.first == b.first);
        string const& word = a.first; // for my convenience
                                      // we found words that are the same!
        auto atf = distance(a.second, decltype(a.second)());
        auto btf = distance(b.second, decltype(b.second)());
        auto df = documentFrequency(word);
        auto idf = log(static_cast<double>(collectionSize())/df);
        auto a_curr_coord = atf*idf;
        auto b_curr_coord = btf*idf;
        a_length += square(a_curr_coord);
        b_length += square(b_curr_coord);
        res += a_curr_coord*b_curr_coord;
        return a; // whatever
    };
    set_merge(d1iter, end, d2iter, end, NullOutputIterator(), compareIters, mergeIters);
    res = sqrt(res);
    res /= sqrt(a_length)*sqrt(b_length);
    
    return res;
}

int Index::indexOfWordInPositions(size_t indexes_idx, std::string word) const {
    using namespace std;
    string const& index = indexes_[indexes_idx];
    auto const& positions = positions_[indexes_idx];
    return binarySearch(positions.begin(), positions.end(),
    [&] (int i) {
        try {
            auto res = getWordFromIndex(index, i);
//            cout << res.size() << "[res_start]" << res << "[res_end]\n";
            return res.compare(word);
        } catch (...) {
            cout << "positions:\n" << positions << '\n'
                << "positions_size: " << positions.size() << '\n'
                << "position index: " << i << '\n'
                << "position at that index: " << positions[i] << '\n';
            throw ;
        }
    });
}


Index::document_freq_t Index::documentFrequency(std::string word) const {
    using namespace std;
    document_freq_t res (0);
    for (size_t i = 0; i != indexes_.size(); ++i) {
        try {
            if (indexes_[i].size() > 0) {
                auto idx = indexOfWordInPositions(i, word);
                if (idx != -1) {
                    if (documentFrequencies_.size() <= i) throw 1;
                    if (idx >= documentFrequencies_[i].size() or idx < 0) throw 1;
                    res += documentFrequencies_[i][idx];
                }
            }
        } catch (...) {
            cout << "documentFrequencies_.size()" << documentFrequencies_.size() << '\n';
            cout << documentFrequencies_ << '\n';
            cout << "indexOfWordInPositions(i, word) " << indexOfWordInPositions(i, word) << '\n';
            throw ;
        }
    }
    return res;
}
size_t Index::collectionSize() const {
    using namespace std;
    return accumulate(
        documentFrequencies_.begin(), documentFrequencies_.end(),
        0,
        [] (size_t acc, vector<document_freq_t> const& frequencies) {
            return accumulate(frequencies.begin(), frequencies.end(), 0);
        });
}

std::string Index::getWordFromIndex(std::string const& index, word_pos_idx_t wordIdx) {
    using namespace std;
    string res;
    if (wordIdx >= index.size()) throw std::runtime_error("index out of bounds");
    size_t wordLen = static_cast<unsigned char>(index[wordIdx]);
    res.resize(wordLen); // differentiate between resize and reserve
    copy(index.begin() + wordIdx + 1, index.begin() + wordIdx + wordLen + 1, res.begin());
    return move(res);
}

void Index::getWordPositionsInIndexString(
                                    std::string const& idx,
                                    std::vector<word_pos_idx_t>& positionsInIndex,
                                    std::vector<document_freq_t>& documentFrequencies) {
    using namespace std;
//    vector<size_t> positionsInIndex;
//    vector<size_t> documentFrequencies;
    positionsInIndex.push_back(0);
    
    string::const_iterator it = idx.begin();
    // & so that string::const_iterator is passed by
    // reference and changed, rather than copied as usual
    IndexIterator<decltype(it)&, docid_t> iter(it);
    while (not iter.end()) {
        documentFrequencies.push_back(distance(iter->second, decltype(iter->second)()));
        positionsInIndex.push_back(it - idx.begin());
        ++iter;
    }
}

void Index::addStringIndex(std::string first_index) {
    using namespace std;
    string idx = first_index;
    cout << "length of first_idx:" << idx.size() << '\n';
    for (auto i: indexes()) cout << i.size() << '\t';
    cout << '\n';
    for (size_t i = 0; ; ++i) {
        if (indexExists(i)) {
            // merge _i, i to _i+1.worddocss
            cout << "length of indexes()[i]: " << indexes_[i].size() << '\n';
            idx = getMergedIndex(idx, indexes_[i]);
            indexes_[i].clear();
            positions_[i].clear();
            cout << "length after merging:" << idx.size() << '\n';
            if (idx.size() < 10) cout << idx << '\n';
        } else {
            cout << "length on adding:" << idx.size() << '\n';
            
            addIndex(move(idx), i);
            break;
        }
    }
}


void Index::saveIndexes() const {
    using namespace std;
    for (unsigned n = 0; n != indexes_.size(); ++n) {
        string const& idx = indexes_[n];
        if (idx.size() > 0) {
            string indexPath = _databasePath + "/idx/" + to_string(n) + ".index";
            ofstream(indexPath) << idx;
            ofstream(indexPath + "_readable.txt") << idxStr2ReadableIdxStr(idx);
        }
    }
}


void Index::loadIndexes() {
    using namespace std;
    static const size_t MAX_NUMBER_OF_INDEXES = 64;
    for (unsigned n = 0; n != MAX_NUMBER_OF_INDEXES; ++n) {
        string indexPath = _databasePath + "/idx/" + to_string(n) + ".index";
        if (/* file exitst */ ifstream(indexPath)) {
            addIndex(copyFileIntoString(indexPath), n);
        }
    }
}

void Index::addIndex(std::string&& idx, size_t idxidx) {
    if (indexes_.size() < (idxidx + 1))
        indexes_.resize(idxidx + 1);
    
    if (positions_.size() < (idxidx + 1))
        positions_.resize(idxidx + 1);
    
    if (documentFrequencies_.size() < (idxidx + 1))
        documentFrequencies_.resize(idxidx + 1);
    
    
    indexes_[idxidx] = std::move(idx);
    getWordPositionsInIndexString(indexes_[idxidx], positions_[idxidx], documentFrequencies_[idxidx]);
}