#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
class Queue {
private:
    struct Node {
        T vertex;
        int dist;
        Node* next;
        Node(T v, int d) : vertex(v), dist(d), next(nullptr) {}
    };

    Node* head = nullptr;

public:
    Queue() = default;

    ~Queue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push(T vertex, int dist) {
        Node* node = new Node(vertex, dist);
        if (!head || dist < head->dist) {
            node->next = head;
            head = node;
            return;
        }
        Node* temp = head;
        while (temp->next && temp->next->dist <= dist) {
            temp = temp->next;
        }
        node->next = temp->next;
        temp->next = node;
    }

    bool empty() const {
        return head == nullptr;
    }

    std::pair<int, int> popMin() {
        if (!head) return {-1, -1};
        Node* minNode = head;
        head = head->next;
        auto result = std::make_pair(minNode->vertex, minNode->dist);
        delete minNode;
        return result;
    }
};

#endif //QUEUE_H
