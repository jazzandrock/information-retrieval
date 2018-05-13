//
//  IndexMerging.h
//  IR
//
//  Created by User on 4/5/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_IndexMerging_h
#define IR_IndexMerging_h

#include <algorithm>
#include <regex>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>
#include "IndexBuilder.h"
#include "Dedup.h"
#include "WordAndPositions.h"
#include "VBOutputIterator.h"
#include "VBGapList.h"
#include "IndexIterator.h"
#include <boost/range/algorithm/merge.hpp>

void saveWordIdsToFile(
    std::map<std::string, VBGapList<docid_t> > const & wordToIdMap,
    std::string const & filePath);

void mergeExistingIDsWithNew(
    VBGapInputIterator<std::vector<char>::iterator, docid_t> iter,
    std::vector<docid_t> & existing);

void writePairToStringIndex(
    std::pair<std::string, std::vector<docid_t>> & current_least,
    std::back_insert_iterator<std::string> & dest
    );

void fillCurrentLeastFromIter(
    std::pair<std::string, std::vector<docid_t>> & current_least,
    StringIndexIterator & idx1_iter);

// requires that both iterators are dereferenceable
StringIndexIterator &
chooseLeastIter(
    StringIndexIterator & idx1_iter,
    StringIndexIterator & idx2_iter);

void verifyIndexString(std::string const& idx);

void mergeIndexes(
    std::string const& idx1_str,
    std::string const& idx2_str,
    std::back_insert_iterator<std::string> dest);

void writeReadableIndexToFile(std::map<std::string, VBGapList<docid_t> > const& wordToIDMap, std::string const& filename);
std::string idxStr2ReadableIdxStr(std::string const& idx);

std::map<std::string, VBGapList<docid_t> > idxStr2idxMap(std::string const& idx);
std::map<std::string, VBGapList<docid_t> > readIndexFromFile(std::string const& filename);

std::string getMergedIndex(std::string const& idx1_str, std::string const& idx2_str);

std::string mapIndex2StringIndex(std::map<std::string, VBGapList<docid_t>> const& wordToIDMap);

bool testIndexMerging();

#endif
