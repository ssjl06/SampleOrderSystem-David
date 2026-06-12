#pragma once
#include "repository/SampleRepository.h"

class SampleService {
public:
    explicit SampleService(SampleRepository& repo);
    void registerSample(const Sample& sample);
    std::vector<Sample> getAllSamples() const;
    std::optional<Sample> findById(const std::string& id) const;
    std::vector<Sample> searchByName(const std::string& keyword) const;
private:
    SampleRepository& repo_;
};
