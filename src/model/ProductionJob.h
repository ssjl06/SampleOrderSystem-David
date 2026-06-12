#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct ProductionJob {
    std::string orderId;
    std::string sampleId;
    int         shortage;
    int         actualProduction;
    double      totalTime;
    std::string enqueuedAt;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProductionJob, orderId, sampleId, shortage, actualProduction, totalTime, enqueuedAt)
