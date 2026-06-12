#include <gtest/gtest.h>
#include "repository/ProductionRepository.h"
#include <filesystem>

class ProductionRepositoryTest : public ::testing::Test {
protected:
    std::string path = "test_jobs_tmp.json";
    void TearDown() override { std::filesystem::remove(path); }
    ProductionJob makeJob(const std::string& orderId, const std::string& enqueuedAt) {
        return {orderId, "S-001", 50, 100, 80.0, enqueuedAt};
    }
};

TEST_F(ProductionRepositoryTest, FrontReturnsFifoOrder) {
    ProductionRepository repo(path);
    repo.save(makeJob("ORD-001", "2026-06-10T08:00:00"));
    repo.save(makeJob("ORD-002", "2026-06-10T09:00:00"));
    ASSERT_TRUE(repo.front().has_value());
    EXPECT_EQ(repo.front()->orderId, "ORD-001");
}

TEST_F(ProductionRepositoryTest, RemoveAdvancesFront) {
    ProductionRepository repo(path);
    repo.save(makeJob("ORD-001", "2026-06-10T08:00:00"));
    repo.save(makeJob("ORD-002", "2026-06-10T09:00:00"));
    repo.remove("ORD-001");
    ASSERT_TRUE(repo.front().has_value());
    EXPECT_EQ(repo.front()->orderId, "ORD-002");
}

TEST_F(ProductionRepositoryTest, JsonRoundTripProductionJob) {
    ProductionRepository repo(path);
    repo.save(makeJob("ORD-001", "2026-06-10T08:00:00"));
    ProductionRepository repo2(path);
    auto job = repo2.findById("ORD-001");
    ASSERT_TRUE(job.has_value());
    EXPECT_EQ(job->orderId,          "ORD-001");
    EXPECT_EQ(job->sampleId,         "S-001");
    EXPECT_EQ(job->shortage,         50);
    EXPECT_EQ(job->actualProduction, 100);
    EXPECT_DOUBLE_EQ(job->totalTime, 80.0);
    EXPECT_EQ(job->enqueuedAt,       "2026-06-10T08:00:00");
}
