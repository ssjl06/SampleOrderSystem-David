#include <gtest/gtest.h>
#include "model/Order.h"

TEST(OrderTest, ValidatePassesForPositiveQuantity) {
    Order o{"ORD-001", "S-001", "삼성전자", 100, OrderStatus::RESERVED, "2026-06-12T09:00:00"};
    EXPECT_NO_THROW(o.validate());
}

TEST(OrderTest, ZeroQuantityThrows) {
    Order o{"ORD-001", "S-001", "삼성전자", 0, OrderStatus::RESERVED, "2026-06-12T09:00:00"};
    EXPECT_THROW(o.validate(), std::invalid_argument);
}

TEST(OrderTest, NegativeQuantityThrows) {
    Order o{"ORD-001", "S-001", "삼성전자", -1, OrderStatus::RESERVED, "2026-06-12T09:00:00"};
    EXPECT_THROW(o.validate(), std::invalid_argument);
}

TEST(OrderTest, JsonRoundTrip) {
    Order o{"ORD-001", "S-001", "삼성전자", 100, OrderStatus::CONFIRMED, "2026-06-12T09:00:00"};
    nlohmann::json j = o;
    Order o2 = j.get<Order>();
    EXPECT_EQ(o2.orderId,      o.orderId);
    EXPECT_EQ(o2.sampleId,     o.sampleId);
    EXPECT_EQ(o2.customerName, o.customerName);
    EXPECT_EQ(o2.quantity,     o.quantity);
    EXPECT_EQ(o2.status,       o.status);
    EXPECT_EQ(o2.createdAt,    o.createdAt);
}
