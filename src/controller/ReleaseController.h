#pragma once
#include "service/ReleaseService.h"
#include "view/ConsoleView.h"
#include "view/TableFormatter.h"

class ReleaseController {
public:
    explicit ReleaseController(ReleaseService& svc);
    void run();
private:
    ReleaseService& svc_;
};
