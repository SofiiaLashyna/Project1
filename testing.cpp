#include <iostream>
#include "BfsListTesting.h"
#include "BfsMatrixTesting.h"
#include "DfsListTesting.h"
#include "DfsMatrixTesting.h"
#include "DijkstraListTesting.h"
#include "DijkstraMatrixTesting.h"
#include "QueueTesting.h"
#include "IsConnectedListTesting.h"
#include "IsConnectedMatrixTesting.h"

int main() {
    std::cout << "1. BFS unit tests:\n";
    std::cout << "list:\n";
    BFS_tests_list bfs_list;
    bfs_list.testBFS_basic();
    bfs_list.testBFS_emptyGraph();
    bfs_list.testBFS_singleVertex();
    std::cout << "\nmatrix:\n";
    BFS_tests_matrix bfs_matrix;
    bfs_matrix.testBFS_basic();
    bfs_matrix.testBFS_emptyGraph();
    bfs_matrix.testBFS_singleVertex();

    std::cout << "\n2. DFS unit test:\n";
    std::cout << "list:\n";
    DFS_tests_list dfs_list;
    dfs_list.testDFS_basic();
    dfs_list.testDFS_emptyGraph();
    dfs_list.testDFS_singleVertex();
    std::cout << "\nmatrix:\n";
    DFS_tests_matrix dfs_matrix;
    dfs_matrix.testDFS_basic();
    dfs_matrix.testDFS_emptyGraph();
    dfs_matrix.testDFS_singleVertex();

    std::cout << "\n\n3. Dijkstra unit test:\n";
    std::cout << "list:\n";
    Dijkstra_tests_list dijkstra_list;
    dijkstra_list.testDijkstra_basic();
    dijkstra_list.testDijkstra_noPath();
    dijkstra_list.testDijkstra_multiplePaths();
    dijkstra_list.testDijkstra_invalidNodes();
    std::cout << "matrix:\n";
    Dijkstra_tests_matrix dijkstra_matrix;
    dijkstra_matrix.testDijkstra_basic();
    dijkstra_matrix.testDijkstra_noPath();
    dijkstra_matrix.testDijkstra_multiplePaths();
    dijkstra_matrix.testDijkstra_invalidNodes();

    std::cout << "\n4. Queue unit test:\n";
    Queue_tests queue_tests;
    queue_tests.testPopEmptyQueue();
    queue_tests.testPushPopOrder();

    std::cout << "\n5. Is Connected unit test:\n";
    std::cout << "list:\n";
    IsConnectedListTests is_connected_list_tests;
    is_connected_list_tests.testList_emptyGraph();
    is_connected_list_tests.testList_singleVertex();
    is_connected_list_tests.testList_connectedGraph();
    is_connected_list_tests.testList_disconnectedGraph();
    std::cout << "matrix:\n";
    IsConnectedMatrixTests is_connected_matrix_tests;
    is_connected_matrix_tests.testMatrix_emptyGraph();
    is_connected_matrix_tests.testMatrix_singleVertex();
    is_connected_matrix_tests.testMatrix_connectedGraph();
    is_connected_matrix_tests.testMatrix_disconnectedGraph();
    return 0;
}
