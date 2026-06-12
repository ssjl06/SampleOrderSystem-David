#include "SampleController.h"

SampleController::SampleController(SampleService& svc) : svc_(svc) {}

void SampleController::run() {
    while (true) {
        ConsoleView::printHeader("시료 관리");
        ConsoleView::printLine("[1] 시료 등록  [2] 목록 조회  [3] 이름 검색  [0] 돌아가기");
        int choice = ConsoleView::promptInt("선택");
        if (choice == 0) break;
        if (choice == 1) handleRegister();
        else if (choice == 2) handleList();
        else if (choice == 3) handleSearch();
    }
}

void SampleController::handleRegister() {
    ConsoleView::printHeader("시료 등록");
    Sample s;
    s.sampleId          = ConsoleView::prompt("시료 ID (예: S-001)");
    s.name              = ConsoleView::prompt("시료명");
    s.avgProductionTime = std::stod(ConsoleView::prompt("평균 생산시간 (sec/ea)"));
    s.yield             = std::stod(ConsoleView::prompt("수율 (0.0~1.0)"));
    s.stock             = std::stoi(ConsoleView::prompt("초기 재고"));
    try {
        s.validate();
        svc_.registerSample(s);
        ConsoleView::printSuccess("시료 등록 완료: " + s.sampleId);
    } catch (const std::exception& e) {
        ConsoleView::printError(e.what());
    }
}

void SampleController::handleList() {
    ConsoleView::printHeader("시료 목록");
    TableFormatter::printSampleTable(svc_.getAllSamples());
}

void SampleController::handleSearch() {
    ConsoleView::printHeader("시료 검색");
    std::string kw = ConsoleView::prompt("검색 키워드");
    auto result = svc_.searchByName(kw);
    ConsoleView::printLine("검색 결과: " + std::to_string(result.size()) + "건");
    TableFormatter::printSampleTable(result);
}
