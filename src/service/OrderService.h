#pragma once
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "repository/ProductionRepository.h"

class OrderService {
public:
    OrderService(SampleRepository& sampleRepo,
                 OrderRepository& orderRepo,
                 ProductionRepository& productionRepo);
    std::string placeOrder(const std::string& sampleId,
                           const std::string& customerName, int quantity);
    void approveOrder(const std::string& orderId);
    void rejectOrder(const std::string& orderId);
    std::vector<Order> getReservedOrders() const;
    std::vector<Order> getAllOrders() const;
private:
    SampleRepository&    sampleRepo_;
    OrderRepository&     orderRepo_;
    ProductionRepository& productionRepo_;
};
