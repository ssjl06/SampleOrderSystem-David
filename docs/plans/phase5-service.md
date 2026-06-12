# Phase 5: 메인 — Service 레이어 TDD

**Goal:** 핵심 비즈니스 로직 4개 Service 구현 (커버리지 90%+ 달성 주력)

**TDD 사이클:** 테스트 먼저 → 구현 → PASSED → Commit

---

## Task 5-1: SampleService

**Files:**
- Create: `src\service\SampleService.h / .cpp`
- Create: `tests\service\SampleServiceTest.cpp`

**Signatures:**
```cpp
SampleService(SampleRepository& repo);
void registerSample(const Sample& sample);
std::vector<Sample> getAllSamples() const;
std::optional<Sample> findById(const std::string& id) const;
std::vector<Sample> searchByName(const std::string& keyword) const;
```

**테스트 케이스:**
| 테스트명 | 검증 내용 |
|---------|----------|
| `RegisterSampleSucceeds` | 정상 시료 등록 후 findAll 결과에 포함되는지 |
| `DuplicateSampleIdThrows` | 동일 ID로 재등록 시 invalid_argument 발생 |
| `SearchByNameFindsMatch` | 키워드가 이름에 포함된 시료만 반환 |
| `SearchByNameReturnsEmptyWhenNoMatch` | 없는 키워드로 검색 시 빈 벡터 반환 |
| `FindByIdReturnsCorrectSample` | 등록된 ID로 조회 시 해당 시료 반환 |

---

## Task 5-2: OrderService

**Files:**
- Create: `src\service\OrderService.h / .cpp`
- Create: `tests\service\OrderServiceTest.cpp`

**Signatures:**
```cpp
OrderService(SampleRepository&, OrderRepository&, ProductionRepository&);
std::string placeOrder(const std::string& sampleId,
                       const std::string& customerName, int quantity);
void approveOrder(const std::string& orderId);
void rejectOrder(const std::string& orderId);
std::vector<Order> getReservedOrders() const;
std::vector<Order> getAllOrders() const;  // MonitorController 용도로 추가
```

**승인 로직:**
```
재고 >= 주문량  →  stock 차감, 상태 CONFIRMED
재고 < 주문량   →  shortage = quantity - stock
                  actualProduction = ceil(shortage / (yield × 0.9))
                  totalTime = avgProductionTime × actualProduction
                  ProductionJob 저장, 상태 PRODUCING
```

**테스트 케이스:**
| 테스트명 | 검증 내용 |
|---------|----------|
| `PlaceOrderCreatesReservedOrder` | 주문 생성 후 상태가 RESERVED이고 orderId가 반환되는지 |
| `PlaceOrderForUnknownSampleThrows` | 등록되지 않은 sampleId → invalid_argument |
| `PlaceOrderWithZeroQuantityThrows` | quantity=0 → invalid_argument |
| `ApproveWithSufficientStockConfirms` | 재고 충분 시 상태가 CONFIRMED으로 변경되는지 |
| `ApproveWithSufficientStockReducesStock` | 승인 후 시료 재고가 주문량만큼 감소하는지 |
| `ApproveWithInsufficientStockProduces` | 재고 부족 시 상태가 PRODUCING으로 변경되는지 |
| `ApproveCreatesCorrectProductionJob` | 재고 부족 시 shortage/actualProd/totalTime 계산이 공식과 일치하는지 (테스트 픽스처: yield=0.92, avgProdTime=0.8, stock=30, quantity=200 → shortage=170, actualProd=206, totalTime=164.8) |
| `RejectOrderSetsRejectedStatus` | 거절 후 상태가 REJECTED인지 |
| `ApproveNonReservedOrderThrows` | 이미 CONFIRMED된 주문 재승인 시 invalid_argument |
| `RejectNonReservedOrderThrows` | CONFIRMED 주문 거절 시도 시 invalid_argument |
| `ApproveWithExactStockConfirms` | stock == quantity 경계값 → CONFIRMED + 재고 0 (Phase 7 추가) |
| `ApproveNonExistentOrderThrows` | 존재하지 않는 orderId 승인 시도 → invalid_argument (Phase 7 추가) |
| `RejectNonExistentOrderThrows` | 존재하지 않는 orderId 거절 시도 → invalid_argument (Phase 7 추가) |
| `GetReservedOrdersReturnsOnlyReserved` | 혼재 상태 주문 중 RESERVED만 반환 (Phase 7 추가) |
| `GetAllOrdersReturnsAll` | 전체 주문 수 일치 확인 (Phase 7 추가) |

---

## Task 5-3: ProductionService

**Files:**
- Create: `src\service\ProductionService.h / .cpp`
- Create: `tests\service\ProductionServiceTest.cpp`

**Signatures:**
```cpp
ProductionService(SampleRepository&, OrderRepository&, ProductionRepository&);
std::optional<ProductionJob> getCurrentJob() const;
std::vector<ProductionJob> getQueue() const;
void checkAndAutoComplete();  // now - enqueuedAt >= totalTime(초) 인 Job 자동 완료
```

**자동 완료 흐름:**
```
checkAndAutoComplete() 호출 시:
  front() Job의 (now - enqueuedAt) >= totalTime 이면:
    실생산량 재고 추가 → 주문량 차감 → 주문 PRODUCING→CONFIRMED → Job 제거
    (연속 완료 가능: 다음 Job도 조건 충족이면 반복)
```

**테스트 케이스:**
| 테스트명 | 검증 내용 |
|---------|----------|
| `FrontReturnsFirstEnqueuedJob` | 먼저 등록된 Job이 getCurrentJob()으로 반환되는지 (FIFO) |
| `GetQueueReturnsFifoOrder` | 여러 Job 등록 시 enqueuedAt 순으로 반환되는지 |
| `AutoCompleteWhenTimeElapsed` | enqueuedAt=과거, totalTime=1초 → checkAndAutoComplete() 후 CONFIRMED 전환되는지 |
| `AutoCompleteAddsStockMinusOrderQty` | 자동 완료 후 재고 = 실생산량 - 주문량 잔여분인지 (예: 실생산 121 - 주문 100 = 재고 21) |
| `AutoCompleteRemovesJobFromQueue` | 자동 완료 후 해당 Job이 큐에서 제거되는지 |
| `NoAutoCompleteWhenTimeNotElapsed` | enqueuedAt=현재, totalTime=9999초 → checkAndAutoComplete() 후 여전히 PRODUCING인지 |
| `GetQueueReturnsEmptyWhenNoJobs` | 큐가 비어있을 때 getQueue() 빈 벡터 반환 |
| `MultipleJobsAutoCompleteInFifoOrder` | 조건 충족된 Job 2개 있을 때 enqueuedAt 순으로 모두 완료되는지 |

---

## Task 5-4: ReleaseService

**Files:**
- Create: `src\service\ReleaseService.h / .cpp`
- Create: `tests\service\ReleaseServiceTest.cpp`

**Signatures:**
```cpp
ReleaseService(OrderRepository& orderRepo);
void releaseOrder(const std::string& orderId);
std::vector<Order> getConfirmedOrders() const;
```

**테스트 케이스:**
| 테스트명 | 검증 내용 |
|---------|----------|
| `ReleaseConfirmedOrderSucceeds` | CONFIRMED 주문 출고 후 상태가 RELEASE인지 |
| `ReleaseNonConfirmedOrderThrows` | RESERVED/PRODUCING/REJECTED/RELEASE 상태 주문 출고 시도 → invalid_argument (CONFIRMED만 출고 허용) |
| `GetConfirmedOrdersReturnsOnlyConfirmed` | RESERVED/CONFIRMED 혼재 시 CONFIRMED만 반환하는지 |
