# SemiOrderSystem

S-Semi 반도체 시료 생산주문관리 시스템 — C++17 콘솔 애플리케이션

## 개요

시료 등록부터 출고까지 전체 주문 흐름을 관리하는 콘솔 기반 시스템입니다.

- 재고 충분 시 즉시 **CONFIRMED**, 부족 시 생산잡 자동 등록 → **PRODUCING**
- 생산 완료 시 실시간 재고 반영 + 주문 **CONFIRMED** 자동 전환
- `REJECTED` 주문은 모니터링 집계에서 제외

## 주문 상태 흐름

```
RESERVED ──재고 충분──▶ CONFIRMED ──▶ RELEASE
         └─재고 부족──▶ PRODUCING ─(자동완료)─▶ CONFIRMED
         └─거절───────▶ REJECTED
```

## 기술 스택

| 항목 | 내용 |
|------|------|
| 언어 | C++17 |
| IDE | Visual Studio 2022 (v143 toolset, x64) |
| 패키지 관리 | vcpkg (manifest mode) |
| JSON 영속성 | nlohmann/json |
| 테스트 프레임워크 | GoogleTest |
| 커버리지 측정 | OpenCppCoverage |

## 아키텍처

MVC + Repository + Service 레이어드 구조

```
View → Controller → Service → Repository → JSON 파일
```

```
src/
├── model/        도메인 객체 (Sample, Order, ProductionJob, OrderStatus)
├── repository/   JSON CRUD 추상화 (IRepository<T> 기반)
├── service/      비즈니스 로직
├── controller/   입력 처리 및 흐름 제어
└── view/         콘솔 출력 (ConsoleView, TableFormatter)
tests/            GoogleTest 단위 테스트
data/             JSON 영속성 파일
scripts/          E2E 시나리오 러너 및 시나리오 파일
docs/             설계 및 구현 계획 문서
```

## 빌드

### 사전 조건

- Visual Studio 2022
- vcpkg 설치 및 통합: `vcpkg integrate install`
- `vcpkg.json`에 nlohmann-json, gtest 의존성 선언됨

### 빌드 방법

1. `SemiOrderSystem.sln` 열기
2. 구성: `Debug | x64`
3. `Ctrl+Shift+B`

## 실행

```
SemiOrderSystem 프로젝트 → 시작 프로젝트 설정 → F5
```

`data/` 폴더가 없으면 자동 생성됩니다.

## 단위 테스트

```
SemiOrderSystemTests 프로젝트 → 시작 프로젝트 설정 → F5
```

또는 **Test Explorer → Run All**.

| 분류 | 파일 | 테스트 수 |
|------|------|:---------:|
| Model | OrderStatusTest, SampleTest, OrderTest | 12 |
| Repository | SampleRepositoryTest, OrderRepositoryTest, ProductionRepositoryTest | 14 |
| Service | SampleServiceTest, OrderServiceTest, ProductionServiceTest, ReleaseServiceTest | 35 |
| Smoke | SmokeTest | 2 |
| **합계** | | **65** |

커버리지: **95.9%** (src/ 기준, OpenCppCoverage) — 리포트: `coverage_report_src/index.html`

## E2E 시나리오 테스트

앱을 빌드한 후 PowerShell에서 시나리오 파일을 파이프로 실행하는 방식입니다.  
AI 없이 사람이 직접 시나리오를 확인·수정하여 재실행할 수 있습니다.

### 실행 방법

```powershell
# 데이터 초기화 후 실행 (두 번째 인수 1 = ResetData)
powershell.exe -File .\scripts\run_scenario.ps1 .\scripts\scenarios\02_order_confirm_release.txt 1

# 데이터 유지하며 실행
powershell.exe -File .\scripts\run_scenario.ps1 .\scripts\scenarios\01_sample_crud.txt
```

### 제공 시나리오

| 파일 | 검증 흐름 |
|------|-----------|
| `01_sample_crud.txt` | 시료 등록 → 목록 조회 → 이름 검색 |
| `02_order_confirm_release.txt` | 주문 접수 → 승인(재고 충분 → CONFIRMED) → 출고 → 모니터링 |
| `03_order_producing_autocomplete.txt` | 주문 접수 → 승인(재고 부족 → PRODUCING) → 2초 대기 → 자동완료(CONFIRMED) 확인 |
| `04_error_handling.txt` | 미등록 시료 주문 / 수량 0 주문 / 재승인 시도 오류 검증 |

### 시나리오 파일 형식

```
# '#' 으로 시작하는 줄 = 주석 (무시됨)
# 빈 줄 = Enter 입력 ("계속하려면 Enter" 프롬프트 응답)
# {TODAY} = 실행일 날짜 (yyyyMMdd) 로 자동 치환
# !sleep N = N초 대기 후 앱 재실행 (타이머 기반 테스트용)

1          # 메인 메뉴: 시료 관리
1          # 시료 등록 선택
S-DEMO
데모시료
5.0        # avgProductionTime (sec/ea)
0.90       # yield
100        # 초기 재고
0          # 돌아가기

           # (빈 줄 = "계속하려면 Enter" 응답)
0          # 메인 종료
```

> **주의:** 컨트롤러가 조기 return하는 경우 (예: 승인 대기 주문 없을 때 `runApprove()` 즉시 반환)  
> 해당 위치에 주문번호/선택 입력을 넣으면 다음 `promptInt`가 엉뚱한 값을 읽습니다.

## 핵심 도메인 규칙

| 규칙 | 내용 |
|------|------|
| 주문 가능 조건 | 등록된 시료만 주문 가능 |
| 승인 분기 | 재고 ≥ 주문량 → CONFIRMED / 재고 < 주문량 → PRODUCING |
| 실생산량 공식 | `ceil(부족분 / (수율 × 0.9))` |
| 생산 스케줄링 | FIFO (`enqueuedAt` Unix epoch 기준) |
| 자동완료 조건 | `now - enqueuedAt ≥ totalTime(초)` → PRODUCING → CONFIRMED |
| 생산 시간 단위 | 초(sec) — `avgProductionTime`(sec/ea), `totalTime`(sec) |

## 관련 도구 (PoC)

| 도구 | 위치 | 용도 |
|------|------|------|
| DataMonitor | `C:\reviewer\DataMonitor\` | `data/*.json` 실시간 모니터링 |
| DummyDataGenerator | `C:\reviewer\DummyDataGenerator\` | 테스트용 더미 데이터 대량 생성 |

## 커버리지 측정

```powershell
# src/ 절대경로 지정으로 시스템 파일 제외
& "C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe" `
    --sources "C:\reviewer\SemiOrderSystem\src" `
    --export_type "html:coverage_report_src" `
    -- "x64\Debug\SemiOrderSystemTests.exe"
```

리포트: `coverage_report_src/index.html`
