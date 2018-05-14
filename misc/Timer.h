//
//  Timer.h
//  IR
//
//  Created by User on 5/14/18.
//  Copyright (c) 2018 Oleg. All rights reserved.
//

#ifndef IR_Timer_h
#define IR_Timer_h
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

struct Timer {
    using t_t = std::chrono::milliseconds;
    t_t start;
    std::string filePath;
    Timer(std::string path): filePath(path) {
        using namespace std;
        using namespace chrono;
        start = duration_cast<t_t>(system_clock::now().time_since_epoch());
    }
    
    ~Timer() {
        using namespace std;
        using namespace chrono;
        t_t end = duration_cast<t_t>(system_clock::now().time_since_epoch());
        cout << "\ntime: " << (end - start).count() << endl;
        if (filePath.size() > 0) {
            size_t prevDuration = 0;
            {
                ifstream fin(filePath);
                if (fin) fin >> prevDuration;
            }
            ofstream(filePath) << (prevDuration + (end - start).count()) << endl << endl;
        }
    }
};

#endif
