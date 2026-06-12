#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

TEST(SmokeTest, GTestWorks) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(SmokeTest, NlohmannJsonWorks) {
    nlohmann::json j = { {"sampleId", "S-001"}, {"stock", 100} };
    EXPECT_EQ(j["sampleId"], "S-001");
    EXPECT_EQ(j["stock"], 100);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
