# Phase 3: PoC 3 & 4 — DataMonitor + DummyDataGenerator

---

## Task 3-1: DataMonitor

**경로:** `C:\reviewer\DataMonitor\`

**Files:**
- Create: `src\main.cpp`

**동작:** data 폴더 경로를 입력받아 samples.json / orders.json / production_jobs.json 을 테이블 형식으로 콘솔 출력

**Steps:**
- [ ] 빌드 → `C:\reviewer\SemiOrderSystem\data` 경로 입력 → 데이터 출력 확인
- [ ] Commit: `feat: implement data monitoring tool`

---

## Task 3-2: DummyDataGenerator

**경로:** `C:\reviewer\DummyDataGenerator\`

**Files:**
- Create: `src\main.cpp`

**생성 데이터:**

| 파일 | 내용 |
|------|------|
| samples.json | 시료 12종 (S-001~S-012, 다양한 yield/stock) |
| orders.json | 주문 36건 (RESERVED 3, CONFIRMED 3, PRODUCING 3, RELEASE 26, REJECTED 1) |
| production_jobs.json | 생산작업 3건 (PRODUCING 주문 대응) |

**phase5 테스트 기준값을 위한 S-003 고정 수치:**

| 필드 | 값 | 비고 |
|-----|---|-----|
| sampleId | S-003 | |
| yield | 0.92 | |
| avgProductionTime | 0.8 | min/ea |
| stock | 30 | |
| 연결 주문 (RESERVED) | ORD-20260612-003, 삼성전자, 200ea | 승인 시 부족분=170 → actualProd=206, totalTime=164.8 |

> 이 값은 `ApproveCreatesCorrectProductionJob` 테스트의 기대값과 반드시 일치해야 합니다.

**Steps:**
- [ ] 빌드 → 출력경로 `C:\reviewer\SemiOrderSystem\data` 입력 → 3개 JSON 파일 생성 확인
- [ ] DataMonitor로 주입된 데이터 확인 (12/36/3건)
- [ ] Commit: `feat: implement dummy data generator`
