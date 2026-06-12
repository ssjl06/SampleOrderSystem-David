#include "SampleRepository.h"
#include <fstream>
#include <nlohmann/json.hpp>

SampleRepository::SampleRepository(const std::string& filePath) : filePath_(filePath) {}

std::vector<Sample> SampleRepository::load() const {
    std::ifstream f(filePath_);
    if (!f.is_open()) return {};
    return nlohmann::json::parse(f).get<std::vector<Sample>>();
}

void SampleRepository::persist(const std::vector<Sample>& items) const {
    std::ofstream(filePath_) << nlohmann::json(items).dump(2);
}

void SampleRepository::save(const Sample& item) {
    auto items = load();
    items.push_back(item);
    persist(items);
}

std::optional<Sample> SampleRepository::findById(const std::string& id) const {
    for (const auto& s : load())
        if (s.sampleId == id) return s;
    return std::nullopt;
}

std::vector<Sample> SampleRepository::findAll() const {
    return load();
}

void SampleRepository::update(const Sample& item) {
    auto items = load();
    for (auto& s : items)
        if (s.sampleId == item.sampleId) { s = item; break; }
    persist(items);
}

void SampleRepository::remove(const std::string& id) {
    auto items = load();
    items.erase(std::remove_if(items.begin(), items.end(),
        [&](const Sample& s) { return s.sampleId == id; }), items.end());
    persist(items);
}
