#pragma once
#include "repository/OrderRepository.h"

class ReleaseService {
public:
    explicit ReleaseService(OrderRepository& orderRepo);
    void releaseOrder(const std::string& orderId);
    std::vector<Order> getConfirmedOrders() const;
private:
    OrderRepository& orderRepo_;
};
