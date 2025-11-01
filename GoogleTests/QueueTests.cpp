#include "gtest/gtest.h"
#include "Queue.h"

TEST(QueueTest, PushPopOrder) {
    Queue<int> q;
    q.push(5, 10);
    q.push(1, 3);
    q.push(3, 7);

    auto [v1, d1] = q.popMin();
    auto [v2, d2] = q.popMin();
    auto [v3, d3] = q.popMin();

    EXPECT_EQ(v1, 1);
    EXPECT_EQ(d1, 3);
    EXPECT_EQ(v2, 3);
    EXPECT_EQ(d2, 7);
    EXPECT_EQ(v3, 5);
    EXPECT_EQ(d3, 10);
}

TEST(QueueTest, PopEmptyQueue) {
    Queue<int> q;
    auto [v, d] = q.popMin();

    EXPECT_EQ(v, -1) << "Popping from empty queue should return (-1, -1)";
    EXPECT_EQ(d, -1) << "Popping from empty queue should return (-1, -1)";
}
