#include "ReleaseService.h"

ReleaseService::ReleaseService(OrderRepository& orderRepo) : orderRepo_(orderRepo) {}

void ReleaseService::releaseOrder(const std::string& orderId) {
    auto order = orderRepo_.findById(orderId);
    if (!order.has_value() || order->status != OrderStatus::CONFIRMED)
        throw std::invalid_argument("Order is not in CONFIRMED state: " + orderId);
    order->status = OrderStatus::RELEASE;
    orderRepo_.update(*order);
}

std::vector<Order> ReleaseService::getConfirmedOrders() const {
    return orderRepo_.findByStatus(OrderStatus::CONFIRMED);
}
