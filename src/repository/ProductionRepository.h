#pragma once
#include "IRepository.h"
#include "model/ProductionJob.h"

class ProductionRepository : public IRepository<ProductionJob> {
public:
    explicit ProductionRepository(const std::string& filePath = "data/production_jobs.json");
    void save(const ProductionJob& item) override;
    std::optional<ProductionJob> findById(const std::string& id) const override;
    std::vector<ProductionJob> findAll() const override;
    void update(const ProductionJob& item) override;
    void remove(const std::string& id) override;
    std::optional<ProductionJob> front() const;   // enqueuedAt 기준 FIFO 첫 번째
    std::vector<ProductionJob> getQueue() const;  // 전체 대기 목록
private:
    std::string filePath_;
    std::vector<ProductionJob> load() const;
    void persist(const std::vector<ProductionJob>& items) const;
};
