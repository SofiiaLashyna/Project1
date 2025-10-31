#ifndef RAIIGUARD_H
#define RAIIGUARD_H
#include <iostream>
#include <sstream>
struct CoutGuard {
    std::streambuf* old_buf;
    std::stringstream& ss;

    CoutGuard(std::stringstream& new_output) : ss(new_output) {
        old_buf = std::cout.rdbuf(ss.rdbuf());
    }

    ~CoutGuard() {
        std::cout.rdbuf(old_buf);
    }
};
#endif //RAIIGUARD_H
