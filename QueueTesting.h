#ifndef QUEUETESTING_H
#define QUEUETESTING_H

#include "Queue.h"
#include <cassert>
#include <iostream>

class Queue_tests {
public:
    static void runAll() {
        testPushPopOrder();
        testPopEmptyQueue();
    }
private:
    static void testPushPopOrder() {
        Queue<int> q;
        q.push(5, 10);
        q.push(1, 3);
        q.push(3, 7);

        auto [v1, d1] = q.popMin();
        auto [v2, d2] = q.popMin();
        auto [v3, d3] = q.popMin();

        assert(v1 == 1 && d1 == 3);
        assert(v2 == 3 && d2 == 7);
        assert(v3 == 5 && d3 == 10);

        std::cout << "1/2 passed\n";
    }

    static void testPopEmptyQueue() {
        Queue<int> q;
        auto [v, d] = q.popMin();
        assert(v == -1 && d == -1);
        std::cout << "2/2 passed\n";
    }
};
#endif //QUEUETESTING_H
