#pragma once
#include "model/Sample.h"
#include "model/Order.h"
#include "model/ProductionJob.h"
#include <vector>

class TableFormatter {
public:
    static void printSampleTable(const std::vector<Sample>& samples);
    static void printOrderTable(const std::vector<Order>& orders);
    static void printProductionQueue(const std::vector<ProductionJob>& jobs);
    static void printStockStatus(const std::vector<Sample>& samples,
                                 const std::vector<Order>& activeOrders);
};
