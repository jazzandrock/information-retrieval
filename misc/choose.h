
//
//  choose.h
//  IR
//
//  Created by User on 3/6/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_choose_h
#define IR_choose_h

template <bool flag, class IsTrue, class IsFalse>
struct choose;

template <class IsTrue, class IsFalse>
struct choose<true, IsTrue, IsFalse> {
   typedef IsTrue type;
};

template <class IsTrue, class IsFalse>
struct choose<false, IsTrue, IsFalse> {
   typedef IsFalse type;
};

#endif
