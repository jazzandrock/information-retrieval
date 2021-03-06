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
#include <boost/filesystem.hpp>


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
    [&] (position_t i) {
//        try {
            auto res = getWordFromIndex(index, i);
//            cout << res.size() << "[res_start]" << res << "[res_end]\n";
            return res.compare(word);
//        } catch (...) {
//            cout << "positions:\n" << positions << '\n'
//                << "positions_size: " << positions.size() << '\n'
//                << "position index: " << i << '\n'
//                << "position at that index: " << positions[i] << '\n';
//            throw ;
//        }
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
    size_t ans;
    ifstream(_databasePath + "/lastLine.txt") >> ans;
    return ans;
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
    string idx = move(first_index);
    if (idx.size() == 0) return;
    for (size_t i = 0; ; ++i) {
        if (indexExists(i)) {
            idx = getMergedIndex(idx, indexes(i));
            boost::filesystem::remove(indexPathByIndex(i));
        } else {
            cout << "length on adding:" << idx.size() << '\n';
            ofstream(indexPathByIndex(i)) << idx;
            break;
        }
    }
}


void Index::saveReadableIndexes() const {
    using namespace std;
    static const size_t MAX_NUMBER_OF_INDEXES = 64;
    for (unsigned n = 0; n != MAX_NUMBER_OF_INDEXES; ++n) {
        string indexPath = _databasePath + "/idx/" + to_string(n) + ".index";
        if (/* file exitst */ ifstream(indexPath)) {
            writeReadableIndexToFile(readIndexFromFile(indexPath), indexPath + "_readable.txt");
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
    using namespace std;

    if (indexes_.size() < (idxidx + 1))
        indexes_.resize(idxidx + 1);
    
    if (positions_.size() < (idxidx + 1))
        positions_.resize(idxidx + 1);
    
    if (documentFrequencies_.size() < (idxidx + 1))
        documentFrequencies_.resize(idxidx + 1);

    indexes_[idxidx] = move(idx);
    getWordPositionsInIndexString(indexes_[idxidx], positions_[idxidx], documentFrequencies_[idxidx]);
}

position_t Index::documentLength(docid_t d) const {
    // TODO: reading from hardcoded path
    auto wordPositions = readIndexFromFile(documentWordPositionsPath(d));
    return accumulate(
          wordPositions.begin(), wordPositions.end(), 0,
          [] (size_t acc, decltype(wordPositions)::const_reference p) {
              return acc + p.second.size();
          });
}

size_t Index::termFreq(std::string const& w, docid_t d) const {
    auto wordPositions = readIndexFromFile(documentWordPositionsPath(d));
    return wordPositions[w].size();
}