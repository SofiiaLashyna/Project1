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

    DFSListAlgorithm<std::string> dfsListAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dfsListAlg.run(gList, 0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "DFS (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    DijkstraListAlgorithm<std::string> dijkstraListAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dijkstraListAlg.run(gList, 0, 99);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Dijkstra (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    std::cout << "\n==================================================" << std::endl;
    std::cout << "             GRAPH MATRIX (Strategy)              " << std::endl;
    std::cout << "==================================================" << std::endl;

    BFSMatrixAlgorithm<std::string> bfsMatrixAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) bfsMatrixAlg.run(gMatrix, 0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BFS (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    DFSMatrixAlgorithm<std::string> dfsMatrixAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dfsMatrixAlg.run(gMatrix, 0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "DFS (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    DijkstraMatrixAlgorithm<std::string> dijkstraMatrixAlg;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) dijkstraMatrixAlg.run(gMatrix, 0, 99);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Dijkstra (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
    std::cout << "==================================================" << std::endl;
}

/**
* The following code demonstrates how algorithms were executed before applying
* the Strategy Pattern. It used a monolithic "Algorithms" God Object.
* This is kept for historical documentation and performance comparison.
*
*
#include <iostream>
#include <chrono>
#include <string>

#include "GraphList.h"
#include <Algorithm.h>
#include <GraphMatrix.h>


void runPerformanceTest() {
    GraphList<std::string> gList;
    GraphMatrix<std::string> gMatrix;

    for (int i = 0; i < 100; ++i) {
        std::string vName = "Vertex_" + std::to_string(i);
        gList.addVertex(i, vName);
        gMatrix.addVertex(i, vName);
    }

    // З'єднуємо їх однаково (додаємо ваги для Дейкстри)
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
    Algorithms<GraphList<std::string>, std::string> algList(gList);
    Algorithms<GraphMatrix<std::string>, std::string> algMatrix(gMatrix);

    std::cout << "==================================================" << std::endl;
    std::cout << "               GRAPH LIST (Legacy)                " << std::endl;
    std::cout << "==================================================" << std::endl;

    // --- LIST: BFS, DFS, Dijkstra ---
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) algList.BFS_list(0);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "BFS (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) algList.DFS_list(0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "DFS (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) algList.Dijkstra_list(0, 99);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Dijkstra (List): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    std::cout << "\n==================================================" << std::endl;
    std::cout << "              GRAPH MATRIX (Legacy)               " << std::endl;
    std::cout << "==================================================" << std::endl;

    // --- MATRIX: BFS, DFS, Dijkstra ---
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) algMatrix.BFS_matrix(0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BFS (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) algMatrix.DFS_matrix(0);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "DFS (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) algMatrix.Dijkstra_matrix(0, 99);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Dijkstra (Matrix): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
}
*/

