# Phase 6: 메인 — Controller + View + 통합

**Goal:** 콘솔 UI 구성 및 전체 흐름 조립, 실제 실행 동작 확인

---

## Task 6-1: ConsoleView + TableFormatter

**Files:**
- Create: `src\view\ConsoleView.h / .cpp`
- Create: `src\view\TableFormatter.h / .cpp`

**ConsoleView Signatures:**
```cpp
static void print(const std::string& msg);
static void printLine(const std::string& msg = "");
static void printHeader(const std::string& title);
static void printDivider();
static std::string prompt(const std::string& label);
static int promptInt(const std::string& label);  // 숫자 입력, 실패 시 재시도
static void printSuccess(const std::string& msg);
static void printError(const std::string& msg);
static void clearScreen();
```

**TableFormatter Signatures:**
```cpp
static void printSampleTable(const std::vector<Sample>& samples);
static void printOrderTable(const std::vector<Order>& orders);
static void printProductionQueue(const std::vector<ProductionJob>& jobs);
static void printStockStatus(const std::vector<Sample>& samples,
                              const std::vector<Order>& activeOrders);
// 재고 상태: 여유(재고>=주문) / 부족(0<재고<주문) / 고갈(재고==0)
```

---

## Task 6-2: 서브 컨트롤러 5개

**Files:**
- Create: `src\controller\SampleController.h / .cpp`
- Create: `src\controller\OrderController.h / .cpp`
- Create: `src\controller\MonitorController.h / .cpp`
- Create: `src\controller\ProductionController.h / .cpp`
- Create: `src\controller\ReleaseController.h / .cpp`

**각 Controller 역할:**

| 컨트롤러 | 메뉴 진입점 | 주요 동작 |
|---------|-----------|---------|
| SampleController | [1] | 시료 등록 / 목록 / 검색 |
| OrderController | [2][3] | 주문 접수 / 승인·거절 목록 처리 |
| MonitorController | [4] | 상태별 주문 수 + 재고 현황 출력 |
| ProductionController | [5] | 현재 생산 중 + 대기 큐 출력 + 완료 처리 |
| ReleaseController | [6] | CONFIRMED 목록 + 출고 실행 |

---

## Task 6-3: MainController + main.cpp 조립

**Files:**
- Create: `src\controller\MainController.h / .cpp`
- Modify: `src\main.cpp`

**MainController 동작:**
1. clearScreen() + 시스템 현황 헤더 출력 (등록 시료 수, 총 재고, 전체 주문, 생산 대기)
2. 메뉴 [1]~[6], [0] 표시
3. 선택 입력 → 해당 컨트롤러 run()
4. [0] → 종료

**main.cpp 조립 순서:**
```
SampleRepository → OrderRepository → ProductionRepository
→ SampleService → OrderService → ProductionService → ReleaseService
→ 각 Controller 생성 → MainController(모든 Controller 주입) → run()
```

---

## Task 6-4: 통합 시나리오 검증

**Steps:**
- [ ] DummyDataGenerator 실행 → data/ 주입
- [ ] 메인 시스템 실행 → 메인 메뉴 현황 확인 (시료 12종, 주문 36건, 생산대기 3건)
- [ ] [1] 시료 관리 → 목록 조회 → 12종 확인
- [ ] [4] 모니터링 → RESERVED:3 / CONFIRMED:3 / PRODUCING:3 / RELEASE:26 확인
- [ ] [3] 주문 승인 → ORD-20260612-001 (LG이노텍, S-001, 300ea) 승인 → 재고 충분 → CONFIRMED
- [ ] [3] 주문 승인 → ORD-20260612-003 (삼성전자, S-003, 200ea) 승인 → 재고 부족 → PRODUCING
- [ ] [5] 생산라인 → 대기 큐 확인
- [ ] [6] 출고 처리 → CONFIRMED 주문 출고 → RELEASE 확인
- [ ] Commit: `feat: add all controllers and complete main application`
