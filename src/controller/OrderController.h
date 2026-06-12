#pragma once
#include "service/OrderService.h"
#include "view/ConsoleView.h"
#include "view/TableFormatter.h"

class OrderController {
public:
    explicit OrderController(OrderService& svc);
    void runPlace();     // [2] 주문 접수
    void runApprove();   // [3] 주문 승인/거절
private:
    OrderService& svc_;
};
