# Phase 4b: 메인 — Repository

**Goal:** JSON 기반 Repository 3개 구현 및 테스트

**TDD 사이클:** 테스트 먼저 → 구현 → PASSED → Commit

---

## Task 4-6: SampleRepository

**Files:**
- Create: `src\repository\SampleRepository.h / .cpp`
- Create: `tests\repository\SampleRepositoryTest.cpp`

**Signatures:**
```cpp
class SampleRepository : public IRepository<Sample> {
    SampleRepository(const std::string& filePath = "data/samples.json");
    // IRepository<Sample> 구현 (sampleId 기준)
private:
    std::vector<Sample> load() const;
    void persist(const std::vector<Sample>&) const;
};
```

**테스트 케이스:**

| 테스트명 | 검증 내용 |
|---------|----------|
| `SaveAndFindById` | 저장 후 동일 ID로 findById 시 같은 객체 반환 |
| `FindByIdMissingReturnsNullopt` | 존재하지 않는 ID 조회 시 nullopt 반환 |
| `UpdateModifiesExisting` | update 후 findById 결과가 변경된 값으로 일치 |
| `RemoveDeletesItem` | remove 후 findById가 nullopt, findAll에 미포함 |
| `PersistsAcrossReload` | Repository 소멸 후 재생성해도 저장 데이터 유지 (JSON 영속성) |

---

## Task 4-7: OrderRepository

**Files:**
- Create: `src\repository\OrderRepository.h / .cpp`
- Create: `tests\repository\OrderRepositoryTest.cpp`

**Signatures:**
```cpp
class OrderRepository : public IRepository<Order> {
    OrderRepository(const std::string& filePath = "data/orders.json");
    std::vector<Order> findByStatus(OrderStatus status) const;  // 추가 메서드
};
```

**테스트 케이스:**

| 테스트명 | 검증 내용 |
|---------|----------|
| `SaveAndFindByStatus` | RESERVED 2건·CONFIRMED 1건 저장 후 각 상태로 findByStatus 시 해당 건수만 반환 |
| `FindByStatusReturnsEmptyWhenNone` | 저장된 주문이 없는 상태로 findByStatus 시 빈 벡터 반환 |
| `UpdateChangesStatus` | RESERVED 주문을 CONFIRMED으로 update 후 findByStatus(CONFIRMED)에 포함되는지 |

---

## Task 4-8: ProductionRepository

**Files:**
- Create: `src\repository\ProductionRepository.h / .cpp`

**Signatures:**
```cpp
class ProductionRepository : public IRepository<ProductionJob> {
    ProductionRepository(const std::string& filePath = "data/production_jobs.json");
    std::optional<ProductionJob> front() const;   // enqueuedAt 기준 첫 번째 (FIFO)
    std::vector<ProductionJob> getQueue() const;  // 전체 대기 목록
};
```

**테스트 케이스:**

| 테스트명 | 검증 내용 |
|---------|----------|
| `FrontReturnsFifoOrder` | 저장 순서대로 front()가 먼저 등록된 Job을 반환 (FIFO) |
| `RemoveAdvancesFront` | front() Job을 remove 후 다음 Job이 front()로 이동 |
| `JsonRoundTripProductionJob` | ProductionJob 저장 후 재로드 시 모든 필드(orderId, shortage, actualProduction, totalTime, enqueuedAt) 동일 |
