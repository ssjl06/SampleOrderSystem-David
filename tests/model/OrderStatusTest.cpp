#include <gtest/gtest.h>
#include "model/OrderStatus.h"

TEST(OrderStatusTest, ToStringAllValues) {
    EXPECT_EQ(toString(OrderStatus::RESERVED),  "RESERVED");
    EXPECT_EQ(toString(OrderStatus::REJECTED),  "REJECTED");
    EXPECT_EQ(toString(OrderStatus::PRODUCING), "PRODUCING");
    EXPECT_EQ(toString(OrderStatus::CONFIRMED), "CONFIRMED");
    EXPECT_EQ(toString(OrderStatus::RELEASE),   "RELEASE");
}

TEST(OrderStatusTest, FromStringAllValues) {
    EXPECT_EQ(fromString("RESERVED"),  OrderStatus::RESERVED);
    EXPECT_EQ(fromString("REJECTED"),  OrderStatus::REJECTED);
    EXPECT_EQ(fromString("PRODUCING"), OrderStatus::PRODUCING);
    EXPECT_EQ(fromString("CONFIRMED"), OrderStatus::CONFIRMED);
    EXPECT_EQ(fromString("RELEASE"),   OrderStatus::RELEASE);
}

TEST(OrderStatusTest, FromStringThrowsOnUnknown) {
    EXPECT_THROW(fromString("INVALID"), std::invalid_argument);
    EXPECT_THROW(fromString(""),        std::invalid_argument);
    EXPECT_THROW(fromString("reserved"), std::invalid_argument); // 소문자
}
