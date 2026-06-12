#pragma once
#include "IRepository.h"
#include "model/Sample.h"

class SampleRepository : public IRepository<Sample> {
public:
    explicit SampleRepository(const std::string& filePath = "data/samples.json");
    void save(const Sample& item) override;
    std::optional<Sample> findById(const std::string& id) const override;
    std::vector<Sample> findAll() const override;
    void update(const Sample& item) override;
    void remove(const std::string& id) override;
private:
    std::string filePath_;
    std::vector<Sample> load() const;
    void persist(const std::vector<Sample>& items) const;
};
