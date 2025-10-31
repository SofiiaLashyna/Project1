#ifndef EDGE_H
#define EDGE_H

template<typename T>
class Edge {
public:
    int from;
    int to;
    int weight;

    Edge() : from(-1), to(-1), weight(0) {
    }

    void changeWeight(int w) { weight = w; }
    int getWeight() { return weight; }

    void connect(int u, int v) {
        from = u;
        to = v;
    }


    int getFrom(){return from;}

    int getTo(){return to;}

    void disconnect() {
        from = -1;
        to = -1;
        weight = 0;
    }

    bool connects(int u, int v) const { return (from == u && to == v) || (from == v && to == u); }
    bool isActive() const { return from != -1 && to != -1; }
};

#endif //EDGE_H
