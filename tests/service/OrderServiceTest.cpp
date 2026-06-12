#include <gtest/gtest.h>
#include "service/OrderService.h"
#include <filesystem>
#include <ctime>

class OrderServiceTest : public ::testing::Test {
protected:
    std::string sPath = "test_svc_samples2_tmp.json";
    std::string oPath = "test_svc_orders_tmp.json";
    std::string jPath = "test_svc_jobs_tmp.json";
    void TearDown() override {
        std::filesystem::remove(sPath);
        std::filesystem::remove(oPath);
        std::filesystem::remove(jPath);
    }
    // 픽스처: yield=0.92, avgProdTime=0.8sec/ea
    Sample makeSample(const std::string& id, int stock = 200) {
        return {id, "테스트 시료", 0.8, 0.92, stock};
    }
};

TEST_F(OrderServiceTest, PlaceOrderCreatesReservedOrder) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001"));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 100);
    EXPECT_FALSE(id.empty());
    auto order = oRepo.findById(id);
    ASSERT_TRUE(order.has_value());
    EXPECT_EQ(order->status, OrderStatus::RESERVED);
}

TEST_F(OrderServiceTest, PlaceOrderForUnknownSampleThrows) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    OrderService svc(sRepo, oRepo, jRepo);
    EXPECT_THROW(svc.placeOrder("S-999", "삼성전자", 100), std::invalid_argument);
}

TEST_F(OrderServiceTest, PlaceOrderWithZeroQuantityThrows) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001"));
    OrderService svc(sRepo, oRepo, jRepo);
    EXPECT_THROW(svc.placeOrder("S-001", "삼성전자", 0), std::invalid_argument);
}

TEST_F(OrderServiceTest, ApproveWithSufficientStockConfirms) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001", 200));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 100);
    svc.approveOrder(id);
    EXPECT_EQ(oRepo.findById(id)->status, OrderStatus::CONFIRMED);
}

TEST_F(OrderServiceTest, ApproveWithSufficientStockReducesStock) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001", 200));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 100);
    svc.approveOrder(id);
    EXPECT_EQ(sRepo.findById("S-001")->stock, 100);
}

TEST_F(OrderServiceTest, ApproveWithInsufficientStockProduces) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001", 30));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 200);
    svc.approveOrder(id);
    EXPECT_EQ(oRepo.findById(id)->status, OrderStatus::PRODUCING);
}

TEST_F(OrderServiceTest, ApproveCreatesCorrectProductionJob) {
    // 픽스처: stock=30, quantity=200 → shortage=170
    // actualProd = ceil(170 / (0.92 * 0.9)) = ceil(205.31) = 206
    // totalTime  = 0.8 * 206 = 164.8 sec
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001", 30));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 200);
    svc.approveOrder(id);
    auto job = jRepo.findById(id);
    ASSERT_TRUE(job.has_value());
    EXPECT_EQ(job->shortage,         170);
    EXPECT_EQ(job->actualProduction, 206);
    EXPECT_DOUBLE_EQ(job->totalTime, 164.8);
    EXPECT_GT(job->enqueuedAt, 0LL);  // 유효한 epoch 값인지
}

TEST_F(OrderServiceTest, RejectOrderSetsRejectedStatus) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001"));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 100);
    svc.rejectOrder(id);
    EXPECT_EQ(oRepo.findById(id)->status, OrderStatus::REJECTED);
}

TEST_F(OrderServiceTest, ApproveNonReservedOrderThrows) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001", 200));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 100);
    svc.approveOrder(id);
    EXPECT_THROW(svc.approveOrder(id), std::invalid_argument);
}

TEST_F(OrderServiceTest, RejectNonReservedOrderThrows) {
    SampleRepository sRepo(sPath); OrderRepository oRepo(oPath); ProductionRepository jRepo(jPath);
    sRepo.save(makeSample("S-001", 200));
    OrderService svc(sRepo, oRepo, jRepo);
    std::string id = svc.placeOrder("S-001", "삼성전자", 100);
    svc.approveOrder(id);
    EXPECT_THROW(svc.rejectOrder(id), std::invalid_argument);
}
