# Phase 4a: 메인 — Model

**Goal:** 도메인 객체 3개 + OrderStatus 열거형 구현, 단위 테스트 작성

**TDD 사이클:** 각 태스크마다 테스트 먼저 → 구현 → PASSED 확인 → Commit

---

## Task 4-1: OrderStatus

**Files:**
- Create: `src\model\OrderStatus.h`
- Create: `tests\model\OrderStatusTest.cpp`

**Signatures:**
```cpp
enum class OrderStatus { RESERVED, REJECTED, PRODUCING, CONFIRMED, RELEASE };
std::string toString(OrderStatus s);
OrderStatus fromString(const std::string& s);  // 미지 값 → invalid_argument
```

**테스트 케이스:**
- `ToStringAllValues` — 5개 enum 값 → 문자열 매핑
- `FromStringAllValues` — 5개 문자열 → enum 매핑
- `FromStringThrowsOnUnknown` — "INVALID", "" → invalid_argument

---

## Task 4-2: Sample

**Files:**
- Create: `src\model\Sample.h`
- Create: `tests\model\SampleTest.cpp`

**Signatures:**
```cpp
struct Sample { sampleId, name, avgProductionTime, yield, stock };
void validate() const;  // yield 범위 0~1, avgProductionTime > 0
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Sample, ...)
```

**테스트 케이스:**
- `ValidatePasses` — 정상 값
- `YieldAboveOneThrows` — yield=1.1 → invalid_argument
- `YieldBelowZeroThrows` — yield=-0.1 → invalid_argument
- `NegativeProductionTimeThrows`
- `JsonRoundTrip` — 직렬화 → 역직렬화 동일성

---

## Task 4-3: Order

**Files:**
- Create: `src\model\Order.h`
- Create: `tests\model\OrderTest.cpp`

**Signatures:**
```cpp
struct Order { orderId, sampleId, customerName, quantity, status(OrderStatus), createdAt };
void validate() const;  // quantity > 0
// to_json / from_json 직접 구현 (OrderStatus 문자열 변환 포함)
```

**테스트 케이스:**
- `ValidatePassesForPositiveQuantity`
- `ZeroQuantityThrows`
- `NegativeQuantityThrows`
- `JsonRoundTrip` — status 포함 직렬화/역직렬화

---

## Task 4-4: ProductionJob

**Files:**
- Create: `src\model\ProductionJob.h`

**Signatures:**
```cpp
struct ProductionJob {
    orderId, sampleId, shortage, actualProduction, totalTime, enqueuedAt
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProductionJob, ...)
```

> 별도 테스트 불필요 — Repository 테스트에서 직렬화 검증

---

## Task 4-5: IRepository 인터페이스

**Files:**
- Create: `src\repository\IRepository.h`

```cpp
template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual void save(const T&) = 0;
    virtual std::optional<T> findById(const std::string& id) const = 0;
    virtual std::vector<T> findAll() const = 0;
    virtual void update(const T&) = 0;
    virtual void remove(const std::string& id) = 0;
};
```
