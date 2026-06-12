#pragma once
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "OrderStatus.h"

struct Order {
    std::string orderId;
    std::string sampleId;
    std::string customerName;
    int         quantity;
    OrderStatus status;
    std::string createdAt;

    void validate() const {
        if (quantity <= 0)
            throw std::invalid_argument("quantity must be > 0");
    }
};

inline void to_json(nlohmann::json& j, const Order& o) {
    j = {
        {"orderId",      o.orderId},
        {"sampleId",     o.sampleId},
        {"customerName", o.customerName},
        {"quantity",     o.quantity},
        {"status",       toString(o.status)},
        {"createdAt",    o.createdAt}
    };
}

inline void from_json(const nlohmann::json& j, Order& o) {
    j.at("orderId").get_to(o.orderId);
    j.at("sampleId").get_to(o.sampleId);
    j.at("customerName").get_to(o.customerName);
    j.at("quantity").get_to(o.quantity);
    o.status = fromString(j.at("status").get<std::string>());
    j.at("createdAt").get_to(o.createdAt);
}
