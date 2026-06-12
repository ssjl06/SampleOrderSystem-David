#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct ProductionJob {
    std::string orderId;
    std::string sampleId;
    int         shortage;
    int         actualProduction;
    double      totalTime;    // 총생산시간 (초)
    long long   enqueuedAt;   // 큐 등록 Unix epoch (초) — 재시작 후에도 경과 시간 계산 가능
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProductionJob, orderId, sampleId, shortage, actualProduction, totalTime, enqueuedAt)
