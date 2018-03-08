//
//  defs.h
//  IR
//
//  Created by User on 2/16/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_defs_h
#define IR_defs_h

typedef unsigned long long docid_t;
typedef unsigned position_t;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(TypeName const&) = delete;              \
void operator=(TypeName const &) = delete;

#define NOTIFY_COPY_AND_ASSIGN(TypeName) \
TypeName(TypeName&) { std::cerr << "#TypeName was copied\n"; }              \
void operator=(TypeName) { std::cerr << "#TypeName was assigned\n"; }

//template <class T>
//struct equality {
//    friend bool operator!=(T const & lhs, T const & rhs) { return not (lhs == rhs); }
//};
//
//
//template <class T>
//struct comparison {
//    friend bool operator>(T const & lhs, T const & rhs) { return rhs < lhs; }
//    friend bool operator<=(T const & lhs, T const & rhs) { return not (lhs > rhs); }
//    friend bool operator>=(T const & lhs, T const & rhs) { return not (lhs < rhs); }
//};

struct I {
    I() {}
    I & operator=(I& other) { std::cerr << "i assigned\n"; return *this; }
    I(I & other) { std::cerr << "i copied\n"; }
    ~I() { std::cerr << "i deleted\n"; }
};

#endif
