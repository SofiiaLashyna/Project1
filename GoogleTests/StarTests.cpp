#include <gtest/gtest.h>
#include "Star.h"

// Тести для Star

TEST(StarTest, Initialization) {
    Star star("Sun", 1.0, 5778.0, Star::starType::Main_sequence_Star);

    EXPECT_EQ(star.getName(), "Sun") << "Name should be correctly assigned";
    EXPECT_DOUBLE_EQ(star.getMass(), 1.0) << "Mass should match constructor parameter";
    EXPECT_EQ(star.star_type, Star::starType::Main_sequence_Star)
        << "Star type should match constructor parameter";
}

TEST(StarTest, GetType) {
    Star star("Betelgeuse", 20.0, 3500.0, Star::starType::Red_Giant);
    std::string type = star.getType();

    EXPECT_EQ(type, "Star") << "getType() should return correct star type string";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
