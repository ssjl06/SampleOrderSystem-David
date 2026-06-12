#include "OrderController.h"

OrderController::OrderController(OrderService& svc) : svc_(svc) {}

void OrderController::runPlace() {
    ConsoleView::printHeader("주문 접수");
    std::string sampleId     = ConsoleView::prompt("시료 ID");
    std::string customerName = ConsoleView::prompt("고객명");
    int quantity             = ConsoleView::promptInt("주문 수량");
    try {
        std::string id = svc_.placeOrder(sampleId, customerName, quantity);
        ConsoleView::printSuccess("주문 접수 완료: " + id);
    } catch (const std::exception& e) {
        ConsoleView::printError(e.what());
    }
}

void OrderController::runApprove() {
    ConsoleView::printHeader("주문 승인/거절");
    auto reserved = svc_.getReservedOrders();
    if (reserved.empty()) { ConsoleView::printLine("승인 대기 주문 없음"); return; }
    TableFormatter::printOrderTable(reserved);

    std::string orderId = ConsoleView::prompt("주문번호");
    ConsoleView::printLine("[1] 승인  [2] 거절");
    int choice = ConsoleView::promptInt("선택");
    try {
        if (choice == 1) {
            svc_.approveOrder(orderId);
            ConsoleView::printSuccess("승인 완료: " + orderId);
        } else if (choice == 2) {
            svc_.rejectOrder(orderId);
            ConsoleView::printSuccess("거절 완료: " + orderId);
        }
    } catch (const std::exception& e) {
        ConsoleView::printError(e.what());
    }
}
