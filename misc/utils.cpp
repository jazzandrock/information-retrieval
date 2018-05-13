//
//  utils.cpp
//  IR
//
//  Created by User on 4/5/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#include "utils.h"
#include <sstream>

std::string copyFileIntoString(std::string const& filePath) {
    using namespace std;
    ifstream t(filePath);
    if (not t) throw runtime_error("copyFileIntoString: no such file as " + filePath);
    string res;
    t.seekg(0, ios::end);
    res.reserve(t.tellg());
    t.seekg(0, ios::beg);
    // wtf, but parentheses are necessary, see "most vexing parse"
    res.assign( (istreambuf_iterator<char>(t)) , istreambuf_iterator<char>());
    return move(res);
}

std::string charVectorToBinaryString(std::vector<char> const& chv) {
    using namespace std;
    ostringstream os;
    for_each(chv.begin(), chv.end(), [&os] (char c) {
        if (static_cast<unsigned char>(c) < 16) os << '0';
        os << hex << int((unsigned char)(c));
    });
    return move(os.str());
}