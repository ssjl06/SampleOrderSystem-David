# PRD.md — 반도체 시료 생산주문관리 시스템

## 배경
가상의 반도체 회사 S-Semi는 주문량 급증으로 엑셀/메모장 기반 관리의 한계에 도달.
주문 처리 현황, 공정 예약, 재고 파악을 통합하는 콘솔 기반 시스템이 필요.

## 역할
| 역할 | 책임 |
|------|------|
| 주문 담당자 | 고객 요청 접수 → 주문서 생성 |
| 생산 담당자 | 주문 승인/거절, 시료 등록, 생산 관리 |

## 주문 상태 흐름
```
RESERVED → CONFIRMED → RELEASE   (재고 충분)
RESERVED → PRODUCING → CONFIRMED → RELEASE   (재고 부족)
RESERVED → REJECTED   (거절)
```

## 기능 요구사항

### 1. 시료 관리
- 시료 등록: ID, 이름, 평균생산시간(min/ea), 수율(0~1), 초기재고
- 시료 목록 조회: 재고 수량 포함
- 시료 검색: 이름 등 속성으로 검색

### 2. 시료 주문 (접수)
- 입력: 시료 ID, 고객명, 주문 수량
- 접수 즉시 상태 RESERVED, 주문번호 발급 (ORD-YYYYMMDD-NNN)

### 3. 주문 승인/거절
- RESERVED 목록 표시
- 승인: 재고 자동 확인
  - 재고 충분 → CONFIRMED (즉시 출고 대기)
  - 재고 부족 → PRODUCING + 생산라인 자동 등록
- 거절: 즉시 REJECTED

### 4. 모니터링
- 상태별 주문 수 (RESERVED / CONFIRMED / PRODUCING / RELEASE)
- 시료별 재고 현황 + 상태 표기
  - 여유: 주문 대비 재고 충분
  - 부족: 주문 대비 재고 부족
  - 고갈: 재고 0

### 5. 생산라인
- 현재 생산 중인 작업 정보 표시
- 대기 큐 목록 (FIFO 순)
- 실생산량: `ceil(부족분 / (수율 × 0.9))`
- 총생산시간: `avgProductionTime × 실생산량`
- 생산 완료 시 PRODUCING → CONFIRMED 자동 전환

### 6. 출고 처리
- CONFIRMED 주문 목록 표시
- 선택한 주문 출고 실행 → RELEASE 전환

## 비기능 요구사항
- 콘솔 기반 인터페이스
- 데이터 영속성: 재시작 후에도 데이터 유지 (JSON 파일)
- 단위 테스트 커버리지 90% 이상

## PoC 항목
| PoC | 경로 | 목적 |
|-----|------|------|
| ConsoleMVC | C:\reviewer\ConsoleMVC\ | MVC 레이어 분리 검증 |
| DataPersistence | C:\reviewer\DataPersistence\ | JSON CRUD 검증 |
| DataMonitor | C:\reviewer\DataMonitor\ | 저장 데이터 실시간 조회 도구 |
| DummyDataGenerator | C:\reviewer\DummyDataGenerator\ | 테스트용 더미 데이터 생성 |

## 제출 요건
- 모든 Repository Public
- Agentic Engineering 기법 적용 (CLAUDE.md, PRD.md, Harness, TDD, 커밋 이력)
- 사용 모델: Sonnet / Effort: Medium
