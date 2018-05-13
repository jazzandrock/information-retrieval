//
//  utils.h
//  IR
//
//  Created by User on 4/5/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef __IR__utils__
#define __IR__utils__

#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

namespace misc {
std::string copyFileIntoString(std::string const& filePath);
std::string charVectorToBinaryString(std::vector<char> const& chv);
}
#endif /* defined(__IR__utils__) */
