#include <QtWidgets/QApplication>
#include "MainWindow/windowinterface.h"
#include <iostream>
#include <chrono>
#include <string>

#include "GraphList.h"
#include "BFSList.h"
#include "BFSMatrix.h"
#include "DFSList.h"
#include "DFSMatrix.h"
#include "DijkstraMatrix.h"
#include "DijkstraList.h"

/*
void runPerformanceTest() {
    GraphList<std::string> gList;
    GraphMatrix<std::string> gMatrix;

    for (int i = 0; i < 100; ++i) {
        std::string vName = "Vertex_" + std::to_string(i);
        gList.addVertex(i, vName);
        gMatrix.addVertex(i, vName);
    }

    for (int i = 0; i < 99; ++i) {
        gList.addEdge(i, i + 1, 1);
        gMatrix.addEdge(i, i + 1, 1);

        if (i + 2 < 100) {
            gList.addEdge(i, i + 2, 2);
            gMatrix.addEdge(i, i + 2, 2);
        }
        if (i + 5 < 100) {
            gList.addEdge(i, i + 5, 5);
            gMatrix.addEdge(i, i + 5, 5);
        }
    }

    const int ITERATIONS = 10000;
    std::cout << "Starting performance test (" << ITERATIONS << " iterations)..." << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "              GRAPH LIST (Strategy)               " << std::endl;
    std::cout << "==================================================" << std::endl;

    // --- LIST: BFS ---
    BFSListAlgorithm<std::string> bfsListAlg;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) bfsListAlg.run(gList, 0);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "BFS (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    // --- LIST: DFS ---
    DFSListAlgorithm<std::string> dfsListAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dfsListAlg.run(gList, 0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "DFS (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    // --- LIST: Dijkstra ---
    DijkstraListAlgorithm<std::string> dijkstraListAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dijkstraListAlg.run(gList, 0, 99);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Dijkstra (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    std::cout << "\n==================================================" << std::endl;
    std::cout << "             GRAPH MATRIX (Strategy)              " << std::endl;
    std::cout << "==================================================" << std::endl;

    // --- MATRIX: BFS ---
    BFSMatrixAlgorithm<std::string> bfsMatrixAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) bfsMatrixAlg.run(gMatrix, 0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BFS (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    // --- MATRIX: DFS ---
    DFSMatrixAlgorithm<std::string> dfsMatrixAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dfsMatrixAlg.run(gMatrix, 0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "DFS (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    // --- MATRIX: Dijkstra ---
    DijkstraMatrixAlgorithm<std::string> dijkstraMatrixAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dijkstraMatrixAlg.run(gMatrix, 0, 99);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Dijkstra (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
    std::cout << "==================================================" << std::endl;
}*/
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    WindowInterface w;
    w.show();
    return a.exec();
    // runPerformanceTest();
}

