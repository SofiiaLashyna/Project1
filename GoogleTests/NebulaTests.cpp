#include "gtest/gtest.h"
#include "Nebula.h"

TEST(NebulaTest, Initialization) {
    Nebula* nebula = new Nebula("Orion", 5.0, Nebula::nebulaType::Emission);

    EXPECT_EQ(nebula->getName(), "Orion") << "Name should be correctly assigned";
    EXPECT_DOUBLE_EQ(nebula->getMass(), 5.0) << "Mass should match constructor parameter";
    EXPECT_EQ(nebula->getNebulaType(), Nebula::nebulaType::Emission)
        << "Nebula type should match constructor parameter";
delete nebula;
}

TEST(NebulaTest, GetType) {
    Nebula* nebula = new Nebula("Crab", 2.5, Nebula::nebulaType::Supernova);
    std::string type = nebula->getType();

    EXPECT_EQ(type, "Nebula") << "getType() should return correct nebula type string";
delete nebula;
}


TEST(NebulaTest, InvalidMass) {
    EXPECT_THROW({
        Nebula* s = new Nebula("Invalid", -1.0, Nebula::nebulaType::Emission);
    }, std::invalid_argument);
}