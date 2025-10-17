#include <gtest/gtest.h>
#include "Nebula.h"

TEST(NebulaTest, Initialization) {
    Nebula nebula("Orion", 5.0, Nebula::nebulaType::Emission);

    EXPECT_EQ(nebula.getName(), "Orion") << "Name should be correctly assigned";
    EXPECT_DOUBLE_EQ(nebula.getMass(), 5.0) << "Mass should match constructor parameter";
    EXPECT_EQ(nebula.nebula_type, Nebula::nebulaType::Emission)
        << "Nebula type should match constructor parameter";
}

TEST(NebulaTest, GetType) {
    Nebula nebula("Crab", 2.5, Nebula::nebulaType::Supernova);
    std::string type = nebula.getType();

    EXPECT_EQ(type, "Nebula") << "getType() should return correct nebula type string";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
