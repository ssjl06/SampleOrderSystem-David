#include "ProductionRepository.h"
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

ProductionRepository::ProductionRepository(const std::string& filePath) : filePath_(filePath) {}

std::vector<ProductionJob> ProductionRepository::load() const {
    std::ifstream f(filePath_);
    if (!f.is_open()) return {};
    return nlohmann::json::parse(f).get<std::vector<ProductionJob>>();
}

void ProductionRepository::persist(const std::vector<ProductionJob>& items) const {
    std::ofstream(filePath_) << nlohmann::json(items).dump(2);
}

void ProductionRepository::save(const ProductionJob& item) {
    auto items = load();
    items.push_back(item);
    persist(items);
}

std::optional<ProductionJob> ProductionRepository::findById(const std::string& id) const {
    for (const auto& j : load())
        if (j.orderId == id) return j;
    return std::nullopt;
}

std::vector<ProductionJob> ProductionRepository::findAll() const {
    return load();
}

void ProductionRepository::update(const ProductionJob& item) {
    auto items = load();
    for (auto& j : items)
        if (j.orderId == item.orderId) { j = item; break; }
    persist(items);
}

void ProductionRepository::remove(const std::string& id) {
    auto items = load();
    items.erase(std::remove_if(items.begin(), items.end(),
        [&](const ProductionJob& j) { return j.orderId == id; }), items.end());
    persist(items);
}

std::optional<ProductionJob> ProductionRepository::front() const {
    auto items = load();
    if (items.empty()) return std::nullopt;
    // enqueuedAt 문자열 기준 최솟값 = 가장 먼저 등록된 Job
    return *std::min_element(items.begin(), items.end(),
        [](const ProductionJob& a, const ProductionJob& b) {
            return a.enqueuedAt < b.enqueuedAt;  // epoch 숫자 비교 → 먼저 등록된 Job
        });
}

std::vector<ProductionJob> ProductionRepository::getQueue() const {
    auto items = load();
    std::sort(items.begin(), items.end(),
        [](const ProductionJob& a, const ProductionJob& b) {
            return a.enqueuedAt < b.enqueuedAt;
        });
    return items;
}
