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

**생성 데이터 (buildScenario 기준):**

| 파일 | 내용 |
|------|------|
| samples.json | 시료 12종 (S-001~S-012, 랜덤 yield/avgProdTime/stock) |
| orders.json | 주문 36건 (RESERVED 3, CONFIRMED 3, PRODUCING 3, RELEASE 26, REJECTED 1) |
| production_jobs.json | 생산작업 3건 (PRODUCING 주문 대응) |

> 모든 값은 랜덤 생성. Phase 5 서비스 테스트는 DummyDataGenerator 데이터에 의존하지 않고 테스트 내부 픽스처를 사용합니다.

**Steps:**
- [ ] 빌드 → 출력경로 `C:\reviewer\SemiOrderSystem\data` 입력 → 3개 JSON 파일 생성 확인
- [ ] DataMonitor로 주입된 데이터 확인 (12/36/3건)
- [ ] Commit: `feat: implement dummy data generator`
