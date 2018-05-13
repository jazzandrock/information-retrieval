//
//  IndexMerging.cpp
//  IR
//
//  Created by User on 4/5/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include "IndexMerging.h"
#include "defs.h"
#include "VBUtils.h"
using namespace std;

template <class InputIterator, class OutputIterator>
void saveWordsFromIteratorToIterator(
                                     InputIterator start, InputIterator & end,
                                     OutputIterator & out) {
    using namespace std;
    for_each(
        start, end,
        [&] (typename InputIterator::reference string_pair_VBGapList) {
        *out++ = static_cast<char>(string_pair_VBGapList.first.length());
        copy(string_pair_VBGapList.first.begin(), string_pair_VBGapList.first.end(), out);
        VB::Utils::copyInnerData(string_pair_VBGapList.second.inner_data().begin(), out);
    });
    *out++ = 0; // end of the index is denoted by zero
}


template <class OutputIterator>
void saveWordsToIterator(WordToIDsMap const & wordToIdMap, OutputIterator & out) {
    auto s = wordToIdMap.begin(), e = wordToIdMap.end();
    saveWordsFromIteratorToIterator(s, e, out);
}

void saveWordIdsToFile(WordToIDsMap const & wordToIdMap, std::string const & filePath) {
    std::ofstream file_ofstream(filePath);
    std::ostreambuf_iterator<char> file_charout (file_ofstream);
    saveWordsToIterator(wordToIdMap, file_charout);
}

void mergeExistingIDsWithNew(VBGapInputIterator<std::vector<char>::iterator, docid_t> iter, vector<docid_t>& existing) {
    vector<docid_t> dest;
    dest.reserve(existing.capacity()); // microoptimization
    // turns out std::merge doesn't delete duplicates
    set_union(
          existing.begin(), existing.end(),
          iter, decltype(iter)(),
          back_inserter(dest));
    existing = move(dest);
}

void writePairToStringIndex(pair<string, vector<docid_t>>& current_least, back_insert_iterator<string>& dest) {
    dest = current_least.first.length();
    copy(current_least.first.begin(), current_least.first.end(), dest);
    VBGapList<docid_t> l;
    copy(current_least.second.begin(), current_least.second.end(), back_inserter(l));
    VB::Utils::copyInnerData(l.inner_data().begin(), dest);
}

void fillCurrentLeastFromIter(
                              pair<string, vector<docid_t>> & current_least,
                              IndexIterator<string::const_iterator, docid_t> & idx1_iter)
{
    current_least.first = idx1_iter->first;
    copy(idx1_iter->second, decltype(idx1_iter->second)(), back_inserter(current_least.second));
}

// requires that both iterators are dereferenceable
IndexIterator<string::const_iterator, docid_t> &
chooseLeastIter(
                IndexIterator<string::const_iterator, docid_t> & idx1_iter,
                IndexIterator<string::const_iterator, docid_t> & idx2_iter)
{
    if (not idx1_iter.end() and not idx2_iter.end()) {
        return idx1_iter->first.compare(idx2_iter->first) <= 0 ? idx1_iter : idx2_iter;
    }
    if (not idx1_iter.end()) return idx1_iter;
    if (not idx2_iter.end()) return idx2_iter;
    
    throw std::runtime_error("both iterators are end");
}

void mergeIndexes(
                  string const& idx1_str,
                  string const& idx2_str,
                  back_insert_iterator<string> dest)
{
    IndexIterator<string::const_iterator, docid_t> idx1_iter(idx1_str.begin());
    IndexIterator<string::const_iterator, docid_t> idx2_iter(idx2_str.begin());
    assert(not idx1_iter.end());
    assert(not idx2_iter.end());
    // we assume that none of iterators is initially empty
    pair<string, vector<docid_t>> current_least;
    IndexIterator<string::const_iterator, docid_t> & less_iter = chooseLeastIter(idx1_iter, idx2_iter);
    fillCurrentLeastFromIter(current_least, less_iter);
    ++less_iter;
    
    while (not idx1_iter.end() or not idx2_iter.end()) {
        // less_iter = chooseLeastIter(idx1_iter, idx2_iter);
        // I used to assign to less_iter, thinking that I just
        // change to what object reference points,
        // but in reality I was assigning to the object
        // calling operator=, that wast tough.
//        IndexIterator<string::const_iterator, docid_t> * less_iter_p (0);
//        try {
//        less_iter_p = &chooseLeastIter(idx1_iter, idx2_iter);
//        } catch (std::runtime_error const & e) {
//            throw std::logic_error("i first check if some iterator is not end and then get exc that both are end. can't be so");
//        }
        auto & less_iter = chooseLeastIter(idx1_iter, idx2_iter);
        string const & word = less_iter->first;
        if (word.compare(current_least.first) == 0) {
            mergeExistingIDsWithNew(less_iter->second, current_least.second);
        } else {
            try {
                writePairToStringIndex(current_least, dest);
            } catch (invalid_argument const& e) {
                cout << "couldn't writePairToStringIndex\n";
                                static unsigned err_id = 0;
                std::cerr << "invalid_argument: " << e.what() << '\n';
        //            << idx1_str << '\n'
        //            << idx2_str << '\n';
                
                ++err_id;
                std::string idx1_fname = "mergeIndexes_err_" + to_string(err_id) + "_idx1_str";
                std::string idx2_fname = "mergeIndexes_err_" + to_string(err_id) + "_idx2_str";
                std::cerr << "we'll write indexes to " << idx1_fname << " and "
                        << idx2_fname << ".";
                { ofstream(idx1_fname) << idx1_str; }
                { ofstream(idx2_fname) << idx2_str; }
                std::string idx1_rfname = "readable_" + idx1_fname + ".txt";
                std::string idx2_rfname = "readable_" + idx2_fname + ".txt";
                std::cerr << "we will also write readable indexes to " << idx1_rfname
                    << " and " << idx2_rfname << '\n';
//                writeReadableIndexToFile(readIndexFromFile(idx1_fname), idx1_rfname);
//                writeReadableIndexToFile(readIndexFromFile(idx2_fname), idx2_rfname);
            }
            current_least.second.clear();
            fillCurrentLeastFromIter(current_least, less_iter);
        }
        
        ++less_iter;
    }

    writePairToStringIndex(current_least, dest);
    *dest = '\0';
}

template <class OutputStream>
void writeReadableIndexToOstream(WordToIDsMap const& wordToIDMap, OutputStream & readableidx) {
    for (auto p : wordToIDMap) {
        readableidx << p.first <<'\n';
        for (auto i : p.second) readableidx << i << ' ';
        readableidx << '\n';
    }
}


std::string idxMap2ReadableIdxStr(WordToIDsMap const& wordToIDMap) {
    ostringstream os;
    writeReadableIndexToOstream(wordToIDMap, os);
    return os.str();
}

std::string idxStr2ReadableIdxStr(std::string const& idx) {
    return idxMap2ReadableIdxStr(idxStr2idxMap(idx));
}
void writeReadableIndexToFile(WordToIDsMap const& wordToIDMap, std::string const& filename) {
    ofstream readableidx (filename);
    writeReadableIndexToOstream(wordToIDMap, readableidx);
}

template <class CharInputIterator>
WordToIDsMap
readIndexFromCharIterator(CharInputIterator in) {
    using namespace std;
    CharInputIterator in2 (in);
    map<string, VBGapList<docid_t> > wordToIdMap;
    IndexIterator<CharInputIterator, docid_t> iter (in);
    map<string, vector<docid_t>> insertHistory;
    try {
        while (not iter.end()) {
            string word = iter.operator*().first;
            // ???: operator*
            // (*iter).second works, but only with parentheses
            // somehow compiler messes up the operator precedence
            VBGapInputIterator<vector<char>::iterator, docid_t> numbersIn = iter->second;
            while (numbersIn != numbersIn) {
                docid_t num = *numbersIn;
                insertHistory[word].push_back(num);
                wordToIdMap[word].push_back(num);
                ++numbersIn;
            }
            ++iter;
        }
    } catch(...) {
        {
            ofstream of("insertHistory.txt");
            for (auto & p : insertHistory) {
                of << "[start of word]";
                of << p.first << "[end of word]\n";
                of << "[start of list]\n";
                for (auto n : p.second) {
                    of << "[s]" << n << "[e]" << '\n';
                }
                of << "[end of list]";
                of << '\n';
            }
        }
        
        
        IndexIterator<CharInputIterator, docid_t> iter2 (in2);
        
        throw ;
    }
    return move(wordToIdMap);
}

WordToIDsMap idxStr2idxMap(std::string const& idx) {
    try {
        auto i = idx.c_str();
        { ofstream("c_str") << i; }
        { ofstream ("idx.begin()") << *idx.begin(); }
        { ofstream ("string_that_was_passed_to_idxStr2IdxPam") << idx; }
        return readIndexFromCharIterator(i);
    } catch (...) {
        { ofstream ("idx.begin()_after_throw") << *idx.begin(); }
        { ofstream ("string_that_was_passed_to_idxStr2IdxPam_after_throw") << idx; }
        throw ;
    }
}
WordToIDsMap readIndexFromFile(std::string const& filename) {
    using namespace std;
    string idx = copyFileIntoString(filename);
    return readIndexFromCharIterator(idx.begin());
}


std::string getMergedIndex(std::string const& idx1_str, std::string const& idx2_str) {
    using namespace std;
    if (idx1_str.size() == 0) return idx2_str;
    if (idx2_str.size() == 0) return idx1_str;
    
    {
        static unsigned n = 1;
        ofstream("merging" + to_string(n) + "first.txt") << idxStr2ReadableIdxStr(idx1_str);
        ofstream("merging" + to_string(n) + "second.txt") << idxStr2ReadableIdxStr(idx2_str);
        ++n;
    }
    
    string res;
    mergeIndexes(idx1_str, idx2_str, back_inserter(res));
    assert(res.size() > min(idx1_str.size(), idx2_str.size()));
    
    {
        static unsigned n = 1;
        ofstream("merged" + to_string(n) + ".txt") << idxStr2ReadableIdxStr(res);
        ++n;
    }
    return std::move(res);
}
//std::string getMergedIndex(std::string const& idx1_str, std::string const& idx2_str) {
//    std::string res;
//    assert(res.size() == 0);
//    auto ins = back_inserter(res);
//    try {
//        mergeIndexes(idx1_str, idx2_str, ins);
//    } catch (std::runtime_error const & e) {
//        static unsigned err_id = 0;
//        std::cerr << "runtime_error: " << e.what() << '\n';
////            << idx1_str << '\n'
////            << idx2_str << '\n';
//        
//        ++err_id;
//        std::string idx1_fname = "err_" + to_string(err_id) + "_idx1_str";
//        std::string idx2_fname = "err_" + to_string(err_id) + "_idx2_str";
//        std::cerr << "we'll write indexes to " << idx1_fname << " and "
//                << idx2_fname << ".";
//        { ofstream(idx1_fname) << idx1_str; }
//        { ofstream(idx2_fname) << idx2_str; }
//        std::string idx1_rfname = "readable_" + idx1_fname + ".txt";
//        std::string idx2_rfname = "readable_" + idx2_fname + ".txt";
//        std::cerr << "we will also write readable indexes to " << idx1_rfname
//            << " and " << idx2_rfname << '\n';
//        writeReadableIndexToFile(readIndexFromFile(idx1_fname), idx1_rfname);
//        writeReadableIndexToFile(readIndexFromFile(idx2_fname), idx2_rfname);
//    }
//    assert(res.size() > min(idx1_str.size(), idx2_str.size()));
//    return std::move(res);
//}

std::string mapIndex2StringIndex(WordToIDsMap const& wordToIDMap) {
    using namespace std;
    string first_index;
    back_insert_iterator<string> first_index_out_iter (first_index);
    saveWordsToIterator(wordToIDMap, first_index_out_iter);
    first_index.shrink_to_fit();
    return move(first_index);
}

bool indexValid(std::string const& idx, WordToIDsMap const& wordToIdMap) {
    try {
        return idxMap2ReadableIdxStr(wordToIdMap) == idxStr2ReadableIdxStr(idx);
    } catch (...) {
        return false;
    }
}