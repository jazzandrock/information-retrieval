//
//  Index.h
//  IR
//
//  Created by User on 4/13/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_Index_h
#define IR_Index_h

#include <string>
#include <vector>
#include <algorithm>
#include "defs.h"
#include "BinarySearch.h"
#include "streamOutput.h"

class Index {
    public:
    using word_pos_idx_t = size_t;
    using document_freq_t = size_t;
    using score_t = double;
    
    score_t cosineSimilarity(docid_t d1, docid_t d2) const;
    

    Index(std::string dbp): _databasePath(dbp) {}
    document_freq_t documentFrequency(std::string word) const;
    size_t collectionSize() const;

    void addStringIndex(std::string newIndex);

	void getWordPositionsInIndexString( std::string const& idx,
                                        std::vector<word_pos_idx_t>& positions,
                                        std::vector<document_freq_t>& documentFrequencies);
    
    inline std::string documentWordPositionsPath(docid_t doc) const { return _databasePath + "/pos/" + std::to_string(doc) + ".wrdps"; }
    
    template <class UnaryFunction>
    void execForEveryDocId( std::string word, UnaryFunction f) const;
    
    static std::string getWordFromIndex(std::string const& index, word_pos_idx_t wordIdx);
    
    void loadIndexes();
    void saveReadableIndexes() const;
    
    inline std::vector<std::string> const&                  indexes                                     () const { return indexes_; };
    inline std::vector<std::vector<word_pos_idx_t>> const&  positions                                   () const { return positions_; };
    inline std::vector<std::vector<document_freq_t>> const& documentFrequencies                         () const { return documentFrequencies_; };
    inline size_t                                           indexInArray                                () const { return indexInArray_; }
    inline size_t                                           indexOfWordInPositionsAndFrequenciesVectors () const { return indexOfWordInPositionsAndFrequenciesVectors_; }
//    inline std::string databasePath() { return _databasePath; }
private:
    template <class UnaryFunction>
    void execForEveryDocIdInIndex(std::string word, UnaryFunction f) const;

    int indexOfWordInPositions(size_t indexes_idx, std::string word) const;
    
    std::string const _databasePath;
    std::vector<std::string> indexes_;
    std::vector<std::vector<word_pos_idx_t>> positions_;
    std::vector<std::vector<document_freq_t>> documentFrequencies_;
    mutable size_t indexInArray_;
    mutable size_t indexOfWordInPositionsAndFrequenciesVectors_;
    
    inline bool indexExists(size_t idxidx) { return static_cast<bool>(std::ifstream(indexPathByIndex(idxidx))); }
    void addIndex(std::string&& idx, size_t idxidx);
    void addNewIndexAndMerge(std::string const& first_index);
    inline std::string indexPathByIndex(size_t n) const { return _databasePath + "/idx/" + std::to_string(n) + ".index"; }
    std::string indexes(size_t i) { return copyFileIntoString(indexPathByIndex(i)); }
};




template <class UnaryFunction>
void Index::execForEveryDocId( std::string word, UnaryFunction f) const {
    using namespace std;
    cout << "indexes.size(): " << indexes_.size() << '\n';
    for (indexInArray_ = 0; indexInArray_ != indexes_.size(); ++indexInArray_) {
        string const& idx = indexes_[indexInArray_];
        if (idx.size() > 0) {
            // we need to pass df in our function somehow.
            // we can do it: make a method that computes/returns
            // df, but we need to specify the index we are currently
            // searching to get
//                vector<string> wordsPresentInIdx;
//                for (auto p: positions) wordsPresentInIdx.push_back(getWordFromIndex(idx, p));
//                cout << wordsPresentInIdx << '\n';
            execForEveryDocIdInIndex(word, f);
        }
    }
}
template <class UnaryFunction>
void Index::execForEveryDocIdInIndex(std::string word, UnaryFunction f) const {
    using namespace std;
    string const& index = indexes_[indexInArray_];
    vector<word_pos_idx_t> const& positions = positions_[indexInArray_];
    try {
        indexOfWordInPositionsAndFrequenciesVectors_ = indexOfWordInPositions(indexInArray_, word);
        // TODO:
        if (indexOfWordInPositionsAndFrequenciesVectors_ != -1) {
            // IT DOESN'T WORK THAT WAY
            // because StringIndexIterator will be immediately destroyed,
            // p will be a dangling reference!!!
            // auto& p = *StringIndexIterator(index.begin() + found_idx);
            auto innerDataStart = index.begin() + positions[indexOfWordInPositionsAndFrequenciesVectors_]; // point to length of the word
            innerDataStart += static_cast<unsigned char>(*innerDataStart) + 1; // point to the start of inner data
            VBGapInputIterator<decltype(innerDataStart), docid_t> docIdsStart (innerDataStart);
            VBGapInputIterator<decltype(innerDataStart), docid_t> docIdsEnd;
            for_each(docIdsStart, docIdsEnd, f);
        }
    } catch (...) {
        throw ;
    }
}

#endif
