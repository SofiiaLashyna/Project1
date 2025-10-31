#include "gtest/gtest.h"
#include "Vertex.h"


TEST(VertexTest, InitialState) {
    Vertex<std::string> v;
    v.setId(1);
    v.setData("A");

    EXPECT_FALSE(v.isVisited()) << "Vertex should not be visited initially";
    EXPECT_EQ(v.getId(), 1) << "Vertex id should be 1";
    EXPECT_EQ(v.getData(), "A") << "Vertex data should be 'A'";
}

TEST(VertexTest, MarkVisited) {
    Vertex<int> v;
    v.markVisited();

    EXPECT_TRUE(v.isVisited()) << "Vertex should be marked as visited after markVisited()";
}

TEST(VertexTest, ResetVisited) {
    Vertex<int> v;
    v.markVisited();
    v.resetVisited();

    EXPECT_FALSE(v.isVisited()) << "Vertex should not be visited after resetVisited()";
}
