#include <windows.h>
#include <filesystem>
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "repository/ProductionRepository.h"
#include "service/SampleService.h"
#include "service/OrderService.h"
#include "service/ProductionService.h"
#include "service/ReleaseService.h"
#include "controller/MainController.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::filesystem::create_directories("data");  // data/ 폴더 없으면 생성

    // Repository 생성
    SampleRepository     sampleRepo("data/samples.json");
    OrderRepository      orderRepo("data/orders.json");
    ProductionRepository prodRepo("data/production_jobs.json");

    // Service 생성 (Repository 주입)
    SampleService     sampleSvc(sampleRepo);
    OrderService      orderSvc(sampleRepo, orderRepo, prodRepo);
    ProductionService prodSvc(sampleRepo, orderRepo, prodRepo);
    ReleaseService    releaseSvc(orderRepo);

    // Controller 생성 (Service 주입)
    SampleController     sampleCtrl(sampleSvc);
    OrderController      orderCtrl(orderSvc);
    MonitorController    monitorCtrl(sampleSvc, orderSvc);
    ProductionController prodCtrl(prodSvc);
    ReleaseController    releaseCtrl(releaseSvc);

    MainController main(sampleCtrl, orderCtrl, monitorCtrl, prodCtrl, releaseCtrl, prodSvc);
    main.run();
}
