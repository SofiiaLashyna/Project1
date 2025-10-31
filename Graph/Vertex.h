#ifndef VERTEX_H
#define VERTEX_H

template<typename T>
class Vertex {
private:
    int id;
    T data;
    bool visited = false;

public:
    void markVisited() { visited = true; }
    void resetVisited() { visited = false; }
    bool isVisited() const { return visited; }
    int getId() const { return id; }
    void setId(int index) { id = index; }
    T getData() const { return data; }
    void setData(T d) { data = d; }
    void printData() const { std::cout << id << " " << data; }
};

#endif //VERTEX_H
