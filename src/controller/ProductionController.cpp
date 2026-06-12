#include "ProductionController.h"
#include <iostream>
#include <ctime>

ProductionController::ProductionController(ProductionService& svc) : svc_(svc) {}

void ProductionController::run() {
    ConsoleView::printHeader("생산라인");
    svc_.checkAndAutoComplete();  // 화면 진입 시 자동 완료 체크

    auto current = svc_.getCurrentJob();
    if (current.has_value()) {
        long long now     = (long long)std::time(nullptr);
        long long elapsed = now - current->enqueuedAt;
        long long remain  = (long long)current->totalTime - elapsed;

        ConsoleView::printLine("[현재 생산 중]");
        std::cout << "  주문: " << current->orderId
                  << "  시료: " << current->sampleId
                  << "  실생산량: " << current->actualProduction << "ea\n";
        if (remain > 0)
            std::cout << "  완료까지 약 " << remain << "초 남음\n";
        else
            std::cout << "  완료 처리 중...\n";
    } else {
        ConsoleView::printLine("[현재 생산 중] 없음");
    }

    ConsoleView::printLine("\n[생산 대기 큐]");
    TableFormatter::printProductionQueue(svc_.getQueue());
}
