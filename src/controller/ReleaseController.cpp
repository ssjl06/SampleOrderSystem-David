#include "ReleaseController.h"

ReleaseController::ReleaseController(ReleaseService& svc) : svc_(svc) {}

void ReleaseController::run() {
    ConsoleView::printHeader("출고 처리");
    auto confirmed = svc_.getConfirmedOrders();
    if (confirmed.empty()) { ConsoleView::printLine("출고 대기 주문 없음"); return; }
    TableFormatter::printOrderTable(confirmed);

    std::string orderId = ConsoleView::prompt("출고할 주문번호");
    try {
        svc_.releaseOrder(orderId);
        ConsoleView::printSuccess("출고 완료: " + orderId);
    } catch (const std::exception& e) {
        ConsoleView::printError(e.what());
    }
}
