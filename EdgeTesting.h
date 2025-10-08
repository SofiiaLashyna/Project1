#ifndef EDGETESTING_H
#define EDGETESTING_H

#include <cassert>
#include <iostream>
#include "Edge.h"

class EdgeTesting {
public:
    static void runAll() {
        testInitialState();
        testConnect();
        testChangeWeight();
        testDisconnect();
    }

private:
    static void testInitialState() {
        Edge<int> e;
        assert(!e.isActive() && "New edge should be inactive");
        assert(!e.connects(1, 2) && "Inactive edge should not connect any vertices");
        std::cout << "1/4 passed\n";
    }

    static void testConnect() {
        Edge<int> e;
        e.connect(1, 2);
        assert(e.isActive() && "Edge should be active after connect()");
        assert(e.connects(1, 2) && "Edge should connect (1,2)");
        assert(e.connects(2, 1) && "Edge should connect (2,1) for undirected graphs");
        std::cout << "2/4 passed\n";
    }

    static void testChangeWeight() {
        Edge<int> e;
        e.connect(1, 2);
        e.changeWeight(5);
        assert(e.weight == 5 && "Edge weight should change correctly");
        std::cout << "3/4 passed\n";
    }

    static void testDisconnect() {
        Edge<int> e;
        e.connect(1, 2);
        e.changeWeight(10);
        e.disconnect();
        assert(!e.isActive() && "Edge should be inactive after disconnect()");
        assert(e.weight == 0 && "Weight should reset to 0 after disconnect()");
        assert(!e.connects(1, 2) && "Disconnected edge should not connect vertices");
        std::cout << "4/4 passed\n";
    }
};

#endif //EDGETESTING_H
