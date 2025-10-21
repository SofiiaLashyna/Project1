#include <gtest/gtest.h>
#include "Vertex.h"


TEST(VertexTest, InitialState) {
    Vertex<std::string> v;
    v.id = 1;
    v.data = "A";

    EXPECT_FALSE(v.visited) << "Vertex should not be visited initially";
    EXPECT_EQ(v.id, 1) << "Vertex id should be 1";
    EXPECT_EQ(v.data, "A") << "Vertex data should be 'A'";
}

TEST(VertexTest, MarkVisited) {
    Vertex<int> v;
    v.markVisited();

    EXPECT_TRUE(v.visited) << "Vertex should be marked as visited after markVisited()";
}

TEST(VertexTest, ResetVisited) {
    Vertex<int> v;
    v.markVisited();
    v.resetVisited();

    EXPECT_FALSE(v.visited) << "Vertex should not be visited after resetVisited()";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

