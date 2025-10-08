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
#include "EdgeTesting.h"
#include "VertexTesting.h"
#include "StarSystemTesting.h"
#include "NebulaTesting.h"
#include "PlanetTesting.h"
#include "StarTesting.h"
#include "GraphTesting.h"
#include "GalaxyTesting.h"
#include "GraphListMatrixTesting.h"

int main() {
    std::cout << "1. BFS unit tests:\n";
    std::cout << "list:\n";
    BFS_tests_list bfs_list;
    bfs_list.runAll();
    std::cout << "\nmatrix:\n";
    BFS_tests_matrix bfs_matrix;
    bfs_matrix.runAll();

    std::cout << "\n2. DFS unit test:\n";
    std::cout << "list:\n";
    DFS_tests_list dfs_list;
    dfs_list.runAll();
    std::cout << "\nmatrix:\n";
    DFS_tests_matrix dfs_matrix;
    dfs_matrix.runAll();

    std::cout << "\n\n3. Dijkstra unit test:\n";
    std::cout << "list:\n";
    Dijkstra_tests_list dijkstra_list;
    dijkstra_list.runAll();
    std::cout << "matrix:\n";
    Dijkstra_tests_matrix dijkstra_matrix;
    dijkstra_matrix.runAll();

    std::cout << "\n4. Queue unit test:\n";
    Queue_tests queue_tests;
    queue_tests.runAll();

    std::cout << "\n5. Is Connected unit test:\n";
    std::cout << "list:\n";
    IsConnectedListTests is_connected_list_tests;
    is_connected_list_tests.runAll();
    std::cout << "matrix:\n";
    IsConnectedMatrixTests is_connected_matrix_tests;
    is_connected_matrix_tests.runAll();

    std::cout << "\n6. Edge unit test:\n";
    EdgeTesting edge_testing;
    edge_testing.runAll();

    std::cout << "\n7. Vertex unit test:\n";
    VertexTesting vertex_testing;
    vertex_testing.runAll();

    std::cout<<"\n8. Star system unit test:\n";
    StarSystemTesting star_system_testing;
    star_system_testing.runAll();

    std::cout<<"\n9. Nebula unit test:\n";
    NebulaTesting nebula_testing;
    nebula_testing.runAll();

    std::cout<<"\n9. Planet unit test:\n";
    PlanetTesting planet_testing;
    planet_testing.runAll();

    std::cout<<"\n10. Star unit test:\n";
    StarTesting star_testing;
    star_testing.runAll();


    std::cout<<"\n11. Graph unit test:\n";
    std::cout << "list:\n";
    GraphTesting<Graph_list<std::string>> graph_testing1;
    graph_testing1.runAll();
    std::cout << "matrix:\n";
    GraphTesting<Graph_matrix<std::string>> graph_testing2;
    graph_testing2.runAll();

    std::cout << "\n12. Galaxy unit test:\n";
    std::cout << "list:\n";
    GalaxyTesting<Graph_list<CelestialObject*>>::runAll();

    std::cout << "matrix:\n";
    GalaxyTesting<Graph_matrix<CelestialObject*>>::runAll();

    std::cout<<"\n13. Graph list and matrix unit test:\n";
    std::cout << "list:\n";
    GraphUnitTest<Graph_list<std::string>> graph_unit_test_list;
    graph_unit_test_list.runAll();

    std::cout << "matrix:\n";
    GraphUnitTest<Graph_matrix<std::string>> graph_unit_test_matrix;
    graph_unit_test_matrix.runAll();

    return 0;
}
