#include "ProductionService.h"
#include <ctime>

ProductionService::ProductionService(SampleRepository& sampleRepo,
                                     OrderRepository& orderRepo,
                                     ProductionRepository& productionRepo)
    : sampleRepo_(sampleRepo), orderRepo_(orderRepo), productionRepo_(productionRepo) {}

std::optional<ProductionJob> ProductionService::getCurrentJob() const {
    return productionRepo_.front();
}

std::vector<ProductionJob> ProductionService::getQueue() const {
    return productionRepo_.getQueue();
}

void ProductionService::checkAndAutoComplete() {
    long long now = (long long)std::time(nullptr);

    // FIFO 순으로 완료 조건 충족 Job을 모두 처리
    while (true) {
        auto job = productionRepo_.front();
        if (!job.has_value()) break;
        if ((now - job->enqueuedAt) < (long long)job->totalTime) break;

        // 재고 반영: stock += actualProduction - quantity
        auto sample = sampleRepo_.findById(job->sampleId).value();
        auto order  = orderRepo_.findById(job->orderId).value();
        sample.stock += job->actualProduction - order.quantity;
        sampleRepo_.update(sample);

        // 주문 상태 CONFIRMED 전환
        order.status = OrderStatus::CONFIRMED;
        orderRepo_.update(order);

        // 큐에서 Job 제거
        productionRepo_.remove(job->orderId);
    }
}
