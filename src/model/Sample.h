#pragma once
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

struct Sample {
    std::string sampleId;
    std::string name;
    double      avgProductionTime;
    double      yield;
    int         stock;

    void validate() const {
        if (yield < 0.0 || yield > 1.0)
            throw std::invalid_argument("yield must be in [0, 1]");
        if (avgProductionTime <= 0.0)
            throw std::invalid_argument("avgProductionTime must be > 0");
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Sample, sampleId, name, avgProductionTime, yield, stock)
