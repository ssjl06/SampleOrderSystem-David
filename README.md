# SemiOrderSystem

S-Semi 사 반도체 시료 생산주문관리 시스템 — C++ 콘솔 애플리케이션

## 개요

시료 등록 → 주문 접수 → 승인/거절 → 생산라인 스케줄링 → 출고 처리까지의 전체 흐름을 관리합니다.
재고 부족 시 생산잡을 자동 등록하고, 생산 완료 시 실시간으로 주문 상태를 CONFIRMED로 전환합니다.

## 기술 스택

| 항목 | 내용 |
|------|------|
| 언어 | C++17 |
| IDE | Visual Studio 2022 (.vcxproj) |
| 패키지 관리 | vcpkg (manifest mode) |
| JSON 영속성 | nlohmann/json |
| 테스트 | GoogleTest (목표 커버리지 90%+) |

## 아키텍처

MVC + Repository + Service 레이어드 구조

```
View → Controller → Service → Repository → JSON 파일
```

```
src/
├── model/          # 도메인 객체 (Sample, Order, ProductionJob, OrderStatus)
├── repository/     # JSON CRUD 추상화 (IRepository<T> 기반)
├── service/        # 비즈니스 로직 (SampleService, OrderService, ProductionService, ReleaseService)
├── controller/     # 입력 처리 및 흐름 제어
└── view/           # 콘솔 출력 (ConsoleView, TableFormatter)
tests/              # GoogleTest 단위 테스트
data/               # JSON 영속성 파일 (samples.json, orders.json, production_jobs.json)
docs/               # 설계 및 구현 계획 문서
```

## 핵심 도메인 규칙

- 등록된 시료만 주문 가능
- 승인 시 재고 확인 → 충분하면 `CONFIRMED`, 부족하면 `PRODUCING` + 생산잡 등록
- 생산라인 스케줄링: FIFO (enqueuedAt epoch 기준)
- 실생산량: `ceil(부족분 / (수율 × 0.9))`
- 생산 자동완료: 매 루프 `checkAndAutoComplete()` 호출 → 경과시간 >= totalTime 이면 `CONFIRMED` 전환
- `REJECTED` 주문은 모니터링 제외

## 주문 상태 흐름

```
RESERVED → CONFIRMED  (재고 충분)
RESERVED → PRODUCING  (재고 부족 → 생산 완료 후 CONFIRMED)
RESERVED → REJECTED
CONFIRMED → RELEASE
```

## 빌드 및 실행

### 사전 조건

- Visual Studio 2022 (v145 toolset)
- vcpkg 설치 및 통합 (`vcpkg integrate install`)
- `vcpkg.json`에 nlohmann-json, gtest 의존성 포함됨

### 빌드

1. `SemiOrderSystem.sln` 열기
2. 구성: `Debug | x64`
3. `Ctrl+Shift+B` 빌드

### 실행

```
SemiOrderSystem 프로젝트를 시작 프로젝트로 설정 후 F5
```

실행 시 `data/` 폴더가 없으면 자동 생성됩니다.

### 테스트 실행

```
SemiOrderSystemTests 프로젝트를 시작 프로젝트로 설정 후 F5
```

또는 Test Explorer에서 Run All.

## 테스트 구성

| 분류 | 파일 | 테스트 수 |
|------|------|-----------|
| Model | OrderStatusTest, SampleTest, OrderTest | 12 |
| Repository | SampleRepositoryTest, OrderRepositoryTest, ProductionRepositoryTest | 14 |
| Service | SampleServiceTest, OrderServiceTest, ProductionServiceTest, ReleaseServiceTest | 35 |
| **합계** | | **61** |

## 관련 도구 (PoC)

| 도구 | 위치 | 용도 |
|------|------|------|
| DataMonitor | `C:\reviewer\DataMonitor\` | `data/*.json` 실시간 모니터링 |
| DummyDataGenerator | `C:\reviewer\DummyDataGenerator\` | 테스트용 더미 데이터 생성 |
