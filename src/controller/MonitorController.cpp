#include "MonitorController.h"
#include <map>
#include <iostream>

MonitorController::MonitorController(SampleService& sampleSvc, OrderService& orderSvc)
    : sampleSvc_(sampleSvc), orderSvc_(orderSvc) {}

void MonitorController::run() {
    ConsoleView::printHeader("모니터링");

    // 상태별 주문 수 집계 (REJECTED 제외)
    auto orders = orderSvc_.getAllOrders();
    std::map<std::string, int> counts;
    for (const auto& o : orders) {
        if (o.status != OrderStatus::REJECTED)
            counts[toString(o.status)]++;
    }
    ConsoleView::printLine("[주문 현황]");
    for (const auto& [status, count] : counts)
        std::cout << "  " << status << ": " << count << "건\n";

    // 시료별 재고 현황
    ConsoleView::printLine("\n[재고 현황]");
    std::vector<Order> activeOrders;
    for (const auto& o : orders)
        if (o.status == OrderStatus::RESERVED || o.status == OrderStatus::PRODUCING)
            activeOrders.push_back(o);
    TableFormatter::printStockStatus(sampleSvc_.getAllSamples(), activeOrders);
}
