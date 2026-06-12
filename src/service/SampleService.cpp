#include "SampleService.h"

SampleService::SampleService(SampleRepository& repo) : repo_(repo) {}

void SampleService::registerSample(const Sample& sample) {
    if (repo_.findById(sample.sampleId).has_value())
        throw std::invalid_argument("Duplicate sampleId: " + sample.sampleId);
    repo_.save(sample);
}

std::vector<Sample> SampleService::getAllSamples() const {
    return repo_.findAll();
}

std::optional<Sample> SampleService::findById(const std::string& id) const {
    return repo_.findById(id);
}

std::vector<Sample> SampleService::searchByName(const std::string& keyword) const {
    std::vector<Sample> result;
    for (const auto& s : repo_.findAll())
        if (s.name.find(keyword) != std::string::npos)
            result.push_back(s);
    return result;
}
