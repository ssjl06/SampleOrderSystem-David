#include <gtest/gtest.h>
#include "service/ReleaseService.h"
#include <filesystem>

class ReleaseServiceTest : public ::testing::Test {
protected:
    std::string path = "test_release_orders_tmp.json";
    void TearDown() override { std::filesystem::remove(path); }
    Order makeOrder(const std::string& id, OrderStatus status) {
        return {id, "S-001", "삼성전자", 100, status, ""};
    }
};

TEST_F(ReleaseServiceTest, ReleaseConfirmedOrderSucceeds) {
    OrderRepository repo(path);
    repo.save(makeOrder("ORD-001", OrderStatus::CONFIRMED));
    ReleaseService svc(repo);
    svc.releaseOrder("ORD-001");
    EXPECT_EQ(repo.findById("ORD-001")->status, OrderStatus::RELEASE);
}

TEST_F(ReleaseServiceTest, ReleaseNonConfirmedOrderThrows) {
    OrderRepository repo(path);
    ReleaseService svc(repo);
    for (auto status : {OrderStatus::RESERVED, OrderStatus::PRODUCING,
                        OrderStatus::REJECTED, OrderStatus::RELEASE}) {
        repo.save(makeOrder("ORD-TMP", status));
        EXPECT_THROW(svc.releaseOrder("ORD-TMP"), std::invalid_argument);
        repo.remove("ORD-TMP");
    }
}

TEST_F(ReleaseServiceTest, ReleaseNonExistentOrderThrows) {
    OrderRepository repo(path);
    ReleaseService svc(repo);
    EXPECT_THROW(svc.releaseOrder("ORD-NOTEXIST"), std::invalid_argument);
}

TEST_F(ReleaseServiceTest, GetConfirmedOrdersReturnsOnlyConfirmed) {
    OrderRepository repo(path);
    repo.save(makeOrder("ORD-001", OrderStatus::RESERVED));
    repo.save(makeOrder("ORD-002", OrderStatus::CONFIRMED));
    repo.save(makeOrder("ORD-003", OrderStatus::CONFIRMED));
    ReleaseService svc(repo);
    auto result = svc.getConfirmedOrders();
    ASSERT_EQ(result.size(), 2u);
    for (const auto& o : result)
        EXPECT_EQ(o.status, OrderStatus::CONFIRMED);
}
