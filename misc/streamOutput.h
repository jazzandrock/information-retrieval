//
//  streamOutput.h
//  IR
//
//  Created by User on 4/9/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_streamOutput_h
#define IR_streamOutput_h

namespace misc {
template<class OStream, class T>
OStream& operator<< (OStream& os, std::vector<T> const& vec) {
    os << "[vector out start]\n";
    for (auto const& e: vec)
        os << e << '\t';
    return os << "[vector out end]\n";;
}
}

#endif
