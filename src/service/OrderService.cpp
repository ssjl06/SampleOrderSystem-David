#include "OrderService.h"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>

OrderService::OrderService(SampleRepository& sampleRepo,
                           OrderRepository& orderRepo,
                           ProductionRepository& productionRepo)
    : sampleRepo_(sampleRepo), orderRepo_(orderRepo), productionRepo_(productionRepo) {}

std::string OrderService::placeOrder(const std::string& sampleId,
                                     const std::string& customerName, int quantity) {
    if (!sampleRepo_.findById(sampleId).has_value())
        throw std::invalid_argument("Unknown sampleId: " + sampleId);
    if (quantity <= 0)
        throw std::invalid_argument("quantity must be > 0");

    // orderId: ORD-YYYYMMDD-NNN (당일 기준 순번)
    auto now = std::time(nullptr);
    std::tm t{};
    localtime_s(&t, &now);
    std::ostringstream oss;
    oss << "ORD-" << std::put_time(&t, "%Y%m%d") << "-"
        << std::setw(3) << std::setfill('0')
        << (orderRepo_.findAll().size() + 1);

    Order order{oss.str(), sampleId, customerName, quantity, OrderStatus::RESERVED, ""};
    orderRepo_.save(order);
    return order.orderId;
}

void OrderService::approveOrder(const std::string& orderId) {
    auto order = orderRepo_.findById(orderId);
    if (!order.has_value() || order->status != OrderStatus::RESERVED)
        throw std::invalid_argument("Order is not in RESERVED state: " + orderId);

    auto sample = sampleRepo_.findById(order->sampleId).value();

    if (sample.stock >= order->quantity) {
        // 재고 충분 → CONFIRMED
        sample.stock -= order->quantity;
        sampleRepo_.update(sample);
        order->status = OrderStatus::CONFIRMED;
        orderRepo_.update(*order);
    } else {
        // 재고 부족 → PRODUCING + 생산잡 등록
        int    shortage   = order->quantity - sample.stock;
        int    actualProd = (int)std::ceil(shortage / (sample.yield * 0.9));
        double totalTime  = sample.avgProductionTime * actualProd;

        ProductionJob job{
            orderId, order->sampleId, shortage, actualProd, totalTime,
            (long long)std::time(nullptr)  // enqueuedAt: 현재 epoch
        };
        productionRepo_.save(job);
        order->status = OrderStatus::PRODUCING;
        orderRepo_.update(*order);
    }
}

void OrderService::rejectOrder(const std::string& orderId) {
    auto order = orderRepo_.findById(orderId);
    if (!order.has_value() || order->status != OrderStatus::RESERVED)
        throw std::invalid_argument("Order is not in RESERVED state: " + orderId);
    order->status = OrderStatus::REJECTED;
    orderRepo_.update(*order);
}

std::vector<Order> OrderService::getReservedOrders() const {
    return orderRepo_.findByStatus(OrderStatus::RESERVED);
}
