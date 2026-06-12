#include <gtest/gtest.h>
#include "service/ProductionService.h"
#include <filesystem>
#include <ctime>

class ProductionServiceTest : public ::testing::Test {
protected:
    std::string sPath = "test_prod_samples_tmp.json";
    std::string oPath = "test_prod_orders_tmp.json";
    std::string jPath = "test_prod_jobs_tmp.json";
    void TearDown() override {
        std::filesystem::remove(sPath);
        std::filesystem::remove(oPath);
        std::filesystem::remove(jPath);
    }

    // PRODUCING 상태 주문 + 생산잡을 직접 레포에 주입
    void injectProducingJob(SampleRepository& sRepo, OrderRepository& oRepo,
                            ProductionRepository& jRepo,
                            const std::string& orderId, const std::string& sampleId,
                            int stock, int quantity, int actualProd,
                            long long enqueuedAt) {
        sRepo.save({sampleId, "테스트 시료", 1.0, 0.9, stock});
        oRepo.save({orderId, sampleId, "삼성전자", quantity, OrderStatus::PRODUCING, ""});
        int shortage = quantity - stock;
        jRepo.save({orderId, sampleId, shortage, actualProd, 1.0, enqueuedAt});
    }
};

TEST_F(ProductionServiceTest, FrontReturnsFirstEnqueuedJob) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-001", "S-001", 0, 100, 121, 1000);
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-002", "S-002", 0, 100, 121, 2000);
    ProductionService svc(sRepo, oRepo, jRepo);
    ASSERT_TRUE(svc.getCurrentJob().has_value());
    EXPECT_EQ(svc.getCurrentJob()->orderId, "ORD-001");
}

TEST_F(ProductionServiceTest, GetQueueReturnsFifoOrder) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-001", "S-001", 0, 100, 121, 1000);
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-002", "S-002", 0, 100, 121, 2000);
    ProductionService svc(sRepo, oRepo, jRepo);
    auto queue = svc.getQueue();
    ASSERT_EQ(queue.size(), 2u);
    EXPECT_EQ(queue[0].orderId, "ORD-001");
    EXPECT_EQ(queue[1].orderId, "ORD-002");
}

TEST_F(ProductionServiceTest, GetQueueReturnsEmptyWhenNoJobs) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    ProductionService svc(sRepo, oRepo, jRepo);
    EXPECT_TRUE(svc.getQueue().empty());
}

TEST_F(ProductionServiceTest, AutoCompleteWhenTimeElapsed) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    // enqueuedAt = 10초 전, totalTime = 1초 → 경과 조건 충족
    long long past = (long long)time(nullptr) - 10;
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-001", "S-001", 0, 100, 121, past);
    ProductionService svc(sRepo, oRepo, jRepo);
    svc.checkAndAutoComplete();
    EXPECT_EQ(oRepo.findById("ORD-001")->status, OrderStatus::CONFIRMED);
}

TEST_F(ProductionServiceTest, AutoCompleteAddsStockMinusOrderQty) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    // stock=0, actualProd=121, quantity=100 → 완료 후 재고 = 0 + 121 - 100 = 21
    long long past = (long long)time(nullptr) - 10;
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-001", "S-001", 0, 100, 121, past);
    ProductionService svc(sRepo, oRepo, jRepo);
    svc.checkAndAutoComplete();
    EXPECT_EQ(sRepo.findById("S-001")->stock, 21);
}

TEST_F(ProductionServiceTest, AutoCompleteRemovesJobFromQueue) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    long long past = (long long)time(nullptr) - 10;
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-001", "S-001", 0, 100, 121, past);
    ProductionService svc(sRepo, oRepo, jRepo);
    svc.checkAndAutoComplete();
    EXPECT_TRUE(svc.getQueue().empty());
}

TEST_F(ProductionServiceTest, NoAutoCompleteWhenTimeNotElapsed) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    // enqueuedAt = 현재, totalTime = 9999초 → 경과 조건 미충족
    long long now = (long long)time(nullptr);
    sRepo.save({"S-001", "테스트 시료", 9999.0, 0.9, 0});
    oRepo.save({"ORD-001", "S-001", "삼성전자", 100, OrderStatus::PRODUCING, ""});
    jRepo.save({"ORD-001", "S-001", 100, 121, 9999.0, now});
    ProductionService svc(sRepo, oRepo, jRepo);
    svc.checkAndAutoComplete();
    EXPECT_EQ(oRepo.findById("ORD-001")->status, OrderStatus::PRODUCING);
}

TEST_F(ProductionServiceTest, GetCurrentJobReturnsNulloptWhenEmpty) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    ProductionService svc(sRepo, oRepo, jRepo);
    EXPECT_FALSE(svc.getCurrentJob().has_value());
}

TEST_F(ProductionServiceTest, MultipleJobsAutoCompleteInFifoOrder) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    long long past1 = (long long)time(nullptr) - 20;
    long long past2 = (long long)time(nullptr) - 10;
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-001", "S-001", 0, 100, 121, past1);
    injectProducingJob(sRepo, oRepo, jRepo, "ORD-002", "S-002", 0, 100, 121, past2);
    ProductionService svc(sRepo, oRepo, jRepo);
    svc.checkAndAutoComplete();
    EXPECT_EQ(oRepo.findById("ORD-001")->status, OrderStatus::CONFIRMED);
    EXPECT_EQ(oRepo.findById("ORD-002")->status, OrderStatus::CONFIRMED);
    EXPECT_TRUE(svc.getQueue().empty());
}
