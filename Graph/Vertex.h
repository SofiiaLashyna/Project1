#ifndef VERTEX_H
#define VERTEX_H

template <typename T>
class Vertex {
public:
    int id;
    T data;
    bool visited = false;
    void markVisited() { visited = true; }
    void resetVisited() { visited = false; }
    void printData() { std::cout << id << " " << data; }
};

#endif //VERTEX_H
