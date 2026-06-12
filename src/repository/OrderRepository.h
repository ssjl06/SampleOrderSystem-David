#pragma once
#include "IRepository.h"
#include "model/Order.h"

class OrderRepository : public IRepository<Order> {
public:
    explicit OrderRepository(const std::string& filePath = "data/orders.json");
    void save(const Order& item) override;
    std::optional<Order> findById(const std::string& id) const override;
    std::vector<Order> findAll() const override;
    void update(const Order& item) override;
    void remove(const std::string& id) override;
    std::vector<Order> findByStatus(OrderStatus status) const;
private:
    std::string filePath_;
    std::vector<Order> load() const;
    void persist(const std::vector<Order>& items) const;
};
