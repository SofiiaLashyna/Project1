#include "gtest/gtest.h"
#include "Star.h"

TEST(StarTest, Initialization) {
    Star* star = new Star("Sun", 1.0, 5778.0, Star::starType::Main_sequence_Star);

    EXPECT_EQ(star->getName(), "Sun") << "Name should be correctly assigned";
    EXPECT_DOUBLE_EQ(star->getMass(), 1.0) << "Mass should match constructor parameter";
    EXPECT_EQ(star->getStarType(), Star::starType::Main_sequence_Star)
        << "Star type should match constructor parameter";
    delete star;
}

TEST(StarTest, GetType) {
    Star* star = new Star("Betelgeuse", 20.0, 3500.0, Star::starType::Red_Giant);
    std::string type = star->getType();

    EXPECT_EQ(type, "Star") << "getType() should return correct star type string";
    delete star;
}

TEST(StarTest, InvalidMass) {
    EXPECT_THROW({
        Star* star = new Star("Invalid", -1.0, 100.0, Star::starType::Brown_Dwarf);
    }, std::invalid_argument);
}