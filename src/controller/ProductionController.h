#pragma once
#include "service/ProductionService.h"
#include "view/ConsoleView.h"
#include "view/TableFormatter.h"

class ProductionController {
public:
    explicit ProductionController(ProductionService& svc);
    void run();
private:
    ProductionService& svc_;
};
