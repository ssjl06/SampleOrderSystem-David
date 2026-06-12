#pragma once
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "repository/ProductionRepository.h"

class ProductionService {
public:
    ProductionService(SampleRepository& sampleRepo,
                      OrderRepository& orderRepo,
                      ProductionRepository& productionRepo);
    std::optional<ProductionJob> getCurrentJob() const;
    std::vector<ProductionJob> getQueue() const;
    void checkAndAutoComplete();  // 경과 시간 확인 후 완료 조건 충족 Job 자동 처리
private:
    SampleRepository&     sampleRepo_;
    OrderRepository&      orderRepo_;
    ProductionRepository& productionRepo_;
};
