#pragma once
#include "service/SampleService.h"
#include "view/ConsoleView.h"
#include "view/TableFormatter.h"

class SampleController {
public:
    explicit SampleController(SampleService& svc);
    void run();
private:
    SampleService& svc_;
    void handleRegister();
    void handleList();
    void handleSearch();
};
