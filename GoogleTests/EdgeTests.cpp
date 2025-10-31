#include "gtest/gtest.h"
#include "Edge.h"

TEST(EdgeTest, InitialState) {
    Edge<int> e;

    EXPECT_FALSE(e.isActive()) << "New edge should be inactive";
    EXPECT_FALSE(e.connects(1, 2)) << "Inactive edge should not connect any vertices";
}

TEST(EdgeTest, Connect) {
    Edge<int> e;
    e.connect(1, 2);

    EXPECT_TRUE(e.isActive()) << "Edge should be active after connect()";
    EXPECT_TRUE(e.connects(1, 2)) << "Edge should connect (1,2)";
    EXPECT_TRUE(e.connects(2, 1)) << "Edge should connect (2,1) for undirected graphs";
}

TEST(EdgeTest, ChangeWeight) {
    Edge<int> e;
    e.connect(1, 2);
    e.changeWeight(5);

    EXPECT_EQ(e.getWeight(), 5) << "Edge weight should change correctly";
}

TEST(EdgeTest, Disconnect) {
    Edge<int> e;
    e.connect(1, 2);
    e.disconnect();

    EXPECT_FALSE(e.isActive()) << "Edge should be inactive after disconnect()";
    EXPECT_EQ(e.getWeight(), 0) << "Weight should reset to 0 after disconnect()";
    EXPECT_FALSE(e.connects(1, 2)) << "Disconnected edge should not connect vertices";
}
