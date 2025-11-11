#include "gtest/gtest.h"
#include "RandomUtilities.h"
#include <limits>
#include "TestFixtures.h"

TEST_F(RandomGeneratorTest, GetIntRange) {
    int minVal = 10;
    int maxVal = 20;
    
    for (int i = 0; i < 100; ++i) {
        int result = rng.getInt(minVal, maxVal);
        EXPECT_GE(result, minVal) << "Generated int should be >= minVal";
        EXPECT_LE(result, maxVal) << "Generated int should be <= maxVal";
    }
}

TEST_F(RandomGeneratorTest, GetDoubleRange) {
    double minVal = 0.5;
    double maxVal = 1.5;

    for (int i = 0; i < 100; ++i) {
        double result = rng.getDouble(minVal, maxVal);
        EXPECT_GE(result, minVal) << "Generated double should be >= minVal";
        EXPECT_LE(result, maxVal) << "Generated double should be <= maxVal";
    }
}

TEST_F(RandomGeneratorTest, GetChoiceThrowsOnEmptyVector) {
    std::vector<int> emptyVec;
    EXPECT_THROW({
        rng.getChoice(emptyVec);
    }, std::runtime_error);
}
TEST_F(RandomGeneratorTest, GetChoicePicksElement) {
    std::vector<std::string> items = {"Apple", "Banana", "Cherry"};
    bool found[3] = {false, false, false};
    
    for (int i = 0; i < 100; ++i) {
        std::string result = rng.getChoice(items);
        if (result == "Apple") found[0] = true;
        if (result == "Banana") found[1] = true;
        if (result == "Cherry") found[2] = true;
        
        bool isKnown = (result == "Apple" || result == "Banana" || result == "Cherry");
        EXPECT_TRUE(isKnown) << "getChoice should only return elements from the vector";
    }
    
    EXPECT_TRUE(found[0] && found[1] && found[2]) << "Should eventually pick all choices (probabilistically)";
}