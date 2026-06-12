#include <gtest/gtest.h>
#include "model/Sample.h"

TEST(SampleTest, ValidatePasses) {
    Sample s{"S-001", "실리콘 웨이퍼", 1.5, 0.85, 100};
    EXPECT_NO_THROW(s.validate());
}

TEST(SampleTest, YieldAboveOneThrows) {
    Sample s{"S-001", "실리콘 웨이퍼", 1.5, 1.1, 100};
    EXPECT_THROW(s.validate(), std::invalid_argument);
}

TEST(SampleTest, YieldBelowZeroThrows) {
    Sample s{"S-001", "실리콘 웨이퍼", 1.5, -0.1, 100};
    EXPECT_THROW(s.validate(), std::invalid_argument);
}

TEST(SampleTest, NegativeProductionTimeThrows) {
    Sample s{"S-001", "실리콘 웨이퍼", -1.0, 0.85, 100};
    EXPECT_THROW(s.validate(), std::invalid_argument);
}

TEST(SampleTest, JsonRoundTrip) {
    Sample s{"S-001", "실리콘 웨이퍼", 1.5, 0.85, 100};
    nlohmann::json j = s;
    Sample s2 = j.get<Sample>();
    EXPECT_EQ(s2.sampleId, s.sampleId);
    EXPECT_EQ(s2.name, s.name);
    EXPECT_DOUBLE_EQ(s2.avgProductionTime, s.avgProductionTime);
    EXPECT_DOUBLE_EQ(s2.yield, s.yield);
    EXPECT_EQ(s2.stock, s.stock);
}
