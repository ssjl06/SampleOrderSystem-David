# 반도체 시료 생산주문관리 시스템 설계 문서

**날짜:** 2026-06-12  
**프로젝트:** SemiOrderSystem  
**언어:** C++ / Visual Studio (.vcxproj)  
**영속성:** nlohmann/json (JSON 파일)  
**테스트:** GoogleTest / 목표 커버리지 90%+

---

## 1. 프로젝트 구조

```
C:\reviewer\
├── ConsoleMVC\              # PoC 1: MVC 스켈레톤
├── DataPersistence\         # PoC 2: JSON CRUD
├── DataMonitor\             # PoC 3: 데이터 모니터링 Tool
├── DummyDataGenerator\      # PoC 4: 더미데이터 생성 Tool
└── SemiOrderSystem\         # 메인 프로젝트
```

### 메인 프로젝트 디렉토리

```
SemiOrderSystem\
├── src\
│   ├── model\
│   │   ├── Sample.h / Sample.cpp
│   │   ├── Order.h / Order.cpp
│   │   └── ProductionJob.h / ProductionJob.cpp
│   ├── repository\
│   │   ├── IRepository.h
│   │   ├── SampleRepository.h / .cpp
│   │   ├── OrderRepository.h / .cpp
│   │   └── ProductionRepository.h / .cpp
│   ├── service\
│   │   ├── SampleService.h / .cpp
│   │   ├── OrderService.h / .cpp
│   │   ├── ProductionService.h / .cpp
│   │   └── ReleaseService.h / .cpp
│   ├── controller\
│   │   ├── MainController.h / .cpp
│   │   ├── SampleController.h / .cpp
│   │   ├── OrderController.h / .cpp
│   │   ├── MonitorController.h / .cpp
│   │   ├── ProductionController.h / .cpp
│   │   └── ReleaseController.h / .cpp
│   ├── view\
│   │   ├── ConsoleView.h / .cpp
│   │   └── TableFormatter.h / .cpp
│   └── main.cpp
├── tests\
│   ├── service\
│   ├── repository\
│   └── model\
├── data\                    # JSON 영속성 파일
│   ├── samples.json
│   ├── orders.json
│   └── production_jobs.json
└── docs\
```

---

## 2. 도메인 모델

### Sample (시료)

| 필드 | 타입 | 설명 |
|------|------|------|
| sampleId | string | 고유 ID (S-001 형식) |
| name | string | 시료명 |
| avgProductionTime | double | 평균 생산시간 (분/ea) |
| yield | double | 수율 (0.0 ~ 1.0) |
| stock | int | 현재 재고 수량 |

### Order (주문)

| 필드 | 타입 | 설명 |
|------|------|------|
| orderId | string | 주문번호 (ORD-YYYYMMDD-NNN 형식) |
| sampleId | string | 주문 시료 ID |
| customerName | string | 고객명 |
| quantity | int | 주문 수량 |
| status | OrderStatus | 현재 상태 |
| createdAt | string | 주문 접수 시각 (주문번호 날짜 생성 및 표시용) |

### ProductionJob (생산 작업)

| 필드 | 타입 | 설명 |
|------|------|------|
| orderId | string | 연결된 주문 ID |
| sampleId | string | 생산할 시료 ID |
| shortage | int | 부족분 (주문량 - 재고) |
| actualProduction | int | 실생산량 = ceil(부족분 / (수율 × 0.9)) |
| totalTime | double | 총생산시간 = avgProductionTime × actualProduction |
| enqueuedAt | string | 큐 등록 시각 (FIFO 순서 기준) |

---

## 3. 주문 상태 전이

```
RESERVED ──[승인 + 재고 충분]──→ CONFIRMED ──[출고]──→ RELEASE
         ──[승인 + 재고 부족]──→ PRODUCING ──[생산완료]──→ CONFIRMED
         ──[거절]─────────────→ REJECTED
```

| 상태 | 의미 |
|------|------|
| RESERVED | 주문 접수 |
| REJECTED | 주문 거절 (모니터링 제외) |
| PRODUCING | 승인 완료, 재고 부족으로 생산 중 |
| CONFIRMED | 승인 완료, 출고 대기 |
| RELEASE | 출고 완료 |

---

## 4. 핵심 비즈니스 로직

### 생산량 계산

```
부족분        = 주문량 - 현재 재고
실생산량      = ceil(부족분 / (수율 × 0.9))
총생산시간    = avgProductionTime × 실생산량
```

예시: 주문 200ea, 재고 30ea, 수율 0.92, 평균생산시간 0.8min
```
부족분     = 200 - 30 = 170
실생산량   = ceil(170 / (0.92 × 0.9)) = ceil(170 / 0.828) = ceil(205.3) = 206
총생산시간 = 0.8 × 206 = 164.8 min
```

### 재고 상태 (모니터링)

| 상태 | 조건 |
|------|------|
| 여유 | 재고 >= 주문대비 충분 |
| 부족 | 0 < 재고 < 주문 수량 |
| 고갈 | 재고 == 0 |

### 생산라인 스케줄링

- 전략: **FIFO** (`std::queue<ProductionJob>`)
- 하나의 생산라인, 한 번에 하나의 작업 처리
- 생산 완료 시 해당 주문 PRODUCING → CONFIRMED 전환

---

## 5. 아키텍처 레이어

```
[View]       콘솔 출력, 사용자 입력 수신
    ↓
[Controller] 입력 파싱, 흐름 제어, View 호출
    ↓
[Service]    비즈니스 로직 (상태 전이, 재고 계산, 생산량 계산)
    ↓
[Repository] JSON 파일 CRUD 추상화
    ↓
[JSON 파일]  data/*.json 영속성
```

### 적용 디자인 패턴

| 패턴 | 적용 위치 | 목적 |
|------|-----------|------|
| Repository Pattern | repository 레이어 | JSON I/O 추상화, Service 격리 |
| Service Layer | service 레이어 | 비즈니스 로직 집중 |
| Template Method | IRepository\<T\> | 공통 CRUD 인터페이스 |
| FIFO Queue | ProductionService | 생산 스케줄링 |
| MVC | 전체 구조 | 관심사 분리 |

---

## 6. 메인 메뉴 기능

| 번호 | 메뉴 | 주요 기능 |
|------|------|-----------|
| 1 | 시료 관리 | 시료 등록, 목록 조회, 이름 검색 |
| 2 | 시료 주문 | 주문 접수 (RESERVED) |
| 3 | 주문 승인/거절 | RESERVED 목록 조회, 승인/거절 처리 |
| 4 | 모니터링 | 상태별 주문 수, 시료별 재고 현황 |
| 5 | 생산라인 조회 | 현재 생산 중 + 대기 큐 (FIFO) |
| 6 | 출고 처리 | CONFIRMED 주문 출고 (→ RELEASE) |
| 0 | 종료 | |

---

## 7. PoC 구성

| PoC | 경로 | 목적 | 메인 프로젝트 기여 |
|-----|------|------|-------------------|
| ConsoleMVC | C:\reviewer\ConsoleMVC\ | MVC 레이어 분리 검증 | 전체 구조 기반 |
| DataPersistence | C:\reviewer\DataPersistence\ | nlohmann/json CRUD 검증 | Repository 레이어 이식 |
| DataMonitor | C:\reviewer\DataMonitor\ | JSON 데이터 실시간 조회 도구 | 개발 중 디버깅 보조 |
| DummyDataGenerator | C:\reviewer\DummyDataGenerator\ | 테스트용 더미 데이터 생성 | data/ 폴더 주입 |

---

## 8. 테스트 전략

**목표:** 90% 이상 커버리지  
**프레임워크:** GoogleTest  
**측정:** OpenCppCoverage 또는 VS 코드 커버리지

### 레이어별 테스트 범위

**Service 레이어 (핵심)**
- OrderService: 승인(재고충분/부족), 거절, 중복처리 방지
- ProductionService: 실생산량 계산, FIFO 순서, 완료 처리
- SampleService: 등록 유효성, 중복 방지
- ReleaseService: CONFIRMED 상태만 출고 허용

**Repository 레이어**
- 저장 → 재로드 → 동일 데이터 보장
- 존재하지 않는 ID 조회 처리

**Model 레이어**
- 수율 범위 0.0~1.0 유효성
- 주문 수량 양수 유효성
- 상태 전이 규칙

### TDD 사이클

```
Red   → 실패하는 테스트 먼저 작성
Green → 테스트 통과하는 최소 코드 구현
Blue  → 리팩토링 후 커버리지 확인
Commit
```

---

## 9. 구현 Phase 계획 (개요)

| Phase | 내용 |
|-------|------|
| Phase 0 | 환경 설정 (vcpkg, nlohmann/json, GoogleTest) + CLAUDE.md 작성 |
| Phase 1 | PoC 1 — ConsoleMVC 스켈레톤 |
| Phase 2 | PoC 2 — DataPersistence JSON CRUD |
| Phase 3 | PoC 3,4 — DataMonitor + DummyDataGenerator |
| Phase 4 | 메인: Model + Repository + 기본 테스트 |
| Phase 5 | 메인: Service 레이어 + TDD |
| Phase 6 | 메인: Controller + View + 통합 |
| Phase 7 | 커버리지 90% 달성, 리팩토링, 최종 커밋 |
