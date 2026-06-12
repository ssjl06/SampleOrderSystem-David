#include "MainController.h"
#include "view/ConsoleView.h"
#include <iostream>

MainController::MainController(SampleController& sample, OrderController& order,
                               MonitorController& monitor, ProductionController& production,
                               ReleaseController& release, ProductionService& prodSvc)
    : sampleCtrl_(sample), orderCtrl_(order), monitorCtrl_(monitor),
      productionCtrl_(production), releaseCtrl_(release), prodSvc_(prodSvc) {}

void MainController::run() {
    while (true) {
        prodSvc_.checkAndAutoComplete();  // 메인 루프마다 자동 완료 체크

        ConsoleView::clearScreen();
        ConsoleView::printHeader("S-Semi 시료 생산주문관리 시스템");
        std::cout << "|  [1] 시료 관리          [2] 주문 접수\n"
                  << "|  [3] 주문 승인/거절      [4] 모니터링\n"
                  << "|  [5] 생산라인           [6] 출고 처리\n"
                  << "|  [0] 종료\n";
        std::cout << "+" << std::string(48, '-') << "+\n";

        int choice = ConsoleView::promptInt("메뉴 선택");
        switch (choice) {
            case 1: sampleCtrl_.run();     break;
            case 2: orderCtrl_.runPlace(); break;
            case 3: orderCtrl_.runApprove(); break;
            case 4: monitorCtrl_.run();    break;
            case 5: productionCtrl_.run(); break;
            case 6: releaseCtrl_.run();    break;
            case 0: ConsoleView::printLine("시스템을 종료합니다."); return;
            default: ConsoleView::printError("잘못된 입력입니다.");
        }
        ConsoleView::prompt("\n계속하려면 Enter 키를 누르세요");
    }
}
