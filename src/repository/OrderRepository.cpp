#include "OrderRepository.h"
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

OrderRepository::OrderRepository(const std::string& filePath) : filePath_(filePath) {}

std::vector<Order> OrderRepository::load() const {
    std::ifstream f(filePath_);
    if (!f.is_open()) return {};
    return nlohmann::json::parse(f).get<std::vector<Order>>();
}

void OrderRepository::persist(const std::vector<Order>& items) const {
    std::ofstream(filePath_) << nlohmann::json(items).dump(2);
}

void OrderRepository::save(const Order& item) {
    auto items = load();
    items.push_back(item);
    persist(items);
}

std::optional<Order> OrderRepository::findById(const std::string& id) const {
    for (const auto& o : load())
        if (o.orderId == id) return o;
    return std::nullopt;
}

std::vector<Order> OrderRepository::findAll() const {
    return load();
}

void OrderRepository::update(const Order& item) {
    auto items = load();
    for (auto& o : items)
        if (o.orderId == item.orderId) { o = item; break; }
    persist(items);
}

void OrderRepository::remove(const std::string& id) {
    auto items = load();
    items.erase(std::remove_if(items.begin(), items.end(),
        [&](const Order& o) { return o.orderId == id; }), items.end());
    persist(items);
}

std::vector<Order> OrderRepository::findByStatus(OrderStatus status) const {
    std::vector<Order> result;
    for (const auto& o : load())
        if (o.status == status) result.push_back(o);
    return result;
}
