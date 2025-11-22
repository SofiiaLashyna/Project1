#ifndef QUEUE_H
#define QUEUE_H
/**
 * @file Queue.h
 * @brief Defines a custom priority queue (min-heap) implemented as a sorted linked list.
 */
/**
 * @class Queue
 * @brief A simple priority queue used for Dijkstra's algorithm.
 *
 * It maintains a sorted linked list where the head is always the
 * node with the minimum distance.
 * @tparam T The type of data to store (e.g., vertex ID).
 */
template <typename T>
class Queue {
private:
    /**
    * @struct Node
    * @brief A node in the linked list queue.
    */
    struct Node {
        T vertex; ///< The data (vertex ID).
        int dist; ///< The priority (distance).
        Node* next; ///< Pointer to the next node.
        Node(T v, int d) : vertex(v), dist(d), next(nullptr) {}
    };

    Node* head = nullptr; ///< The front of the queue (always the minimum element).

public:
    /**
     * @brief Default constructor.
     */
    Queue() = default;
    /**
             * @brief Destructor.
             * Cleans up all nodes in the linked list.
             */
    ~Queue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    /**
             * @brief Pushes an item onto the priority queue in sorted order.
             * @param vertex The vertex ID.
             * @param dist The distance (priority).
             */
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
    /**
             * @brief Checks if the queue is empty.
             * @return True if the queue is empty, false otherwise.
             */
    bool empty() const {
        return head == nullptr;
    }
    /**
             * @brief Pops the element with the minimum distance from the queue.
             * @return A std::pair of {vertex, dist}. Returns {-1, -1} if empty.
             */
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
