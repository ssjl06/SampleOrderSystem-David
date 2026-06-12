#include <gtest/gtest.h>
#include "repository/OrderRepository.h"
#include <filesystem>

class OrderRepositoryTest : public ::testing::Test {
protected:
    std::string path = "test_orders_tmp.json";
    void TearDown() override { std::filesystem::remove(path); }
    Order makeOrder(const std::string& id, OrderStatus status) {
        return {id, "S-001", "삼성전자", 100, status, "2026-06-12T09:00:00"};
    }
};

TEST_F(OrderRepositoryTest, SaveAndFindByStatus) {
    OrderRepository repo(path);
    repo.save(makeOrder("ORD-001", OrderStatus::RESERVED));
    repo.save(makeOrder("ORD-002", OrderStatus::RESERVED));
    repo.save(makeOrder("ORD-003", OrderStatus::CONFIRMED));
    EXPECT_EQ(repo.findByStatus(OrderStatus::RESERVED).size(),  2u);
    EXPECT_EQ(repo.findByStatus(OrderStatus::CONFIRMED).size(), 1u);
}

TEST_F(OrderRepositoryTest, FindByStatusReturnsEmptyWhenNone) {
    OrderRepository repo(path);
    EXPECT_TRUE(repo.findByStatus(OrderStatus::RESERVED).empty());
}

TEST_F(OrderRepositoryTest, UpdateChangesStatus) {
    OrderRepository repo(path);
    repo.save(makeOrder("ORD-001", OrderStatus::RESERVED));
    Order updated = makeOrder("ORD-001", OrderStatus::CONFIRMED);
    repo.update(updated);
    EXPECT_TRUE(repo.findByStatus(OrderStatus::CONFIRMED).size() == 1u);
    EXPECT_TRUE(repo.findByStatus(OrderStatus::RESERVED).empty());
}
