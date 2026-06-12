#pragma once
#include "SampleController.h"
#include "OrderController.h"
#include "MonitorController.h"
#include "ProductionController.h"
#include "ReleaseController.h"
#include "service/ProductionService.h"

class MainController {
public:
    MainController(SampleController& sample, OrderController& order,
                   MonitorController& monitor, ProductionController& production,
                   ReleaseController& release, ProductionService& prodSvc);
    void run();
private:
    SampleController&     sampleCtrl_;
    OrderController&      orderCtrl_;
    MonitorController&    monitorCtrl_;
    ProductionController& productionCtrl_;
    ReleaseController&    releaseCtrl_;
    ProductionService&    prodSvc_;
};
