#pragma once
#include "service/SampleService.h"
#include "service/OrderService.h"
#include "view/ConsoleView.h"
#include "view/TableFormatter.h"

class MonitorController {
public:
    MonitorController(SampleService& sampleSvc, OrderService& orderSvc);
    void run();
private:
    SampleService& sampleSvc_;
    OrderService&  orderSvc_;
};
