# Phase 7: 커버리지 90% 달성 + 최종 마무리

**Goal:** 커버리지 측정 → 미달 보완 → 더미 데이터 검증 → 최종 커밋

---

## Task 7-1: 커버리지 측정 환경 구성

**Tool:** OpenCppCoverage (Windows) + VS Test Explorer

**설치 확인:**
```powershell
OpenCppCoverage --version
```

**실행 명령 (SemiOrderSystemTests 기준):**
```powershell
OpenCppCoverage --sources src\ --export-type html:coverage_report `
  -- "x64\Debug\SemiOrderSystemTests.exe"
```

**결과 확인:** `coverage_report\index.html` → 파일별 커버리지 % 확인

---

## Task 7-2: 미달 케이스 보완

**기준:** 파일별 라인 커버리지 90% 미달 파일 대상으로 추가 테스트 작성

**예상 취약 지점:**

| 대상 | 추가 테스트 포인트 |
|-----|------------------|
| `OrderService` | placeOrder ID 형식 검증 (orderId 패턴 확인) |
| `ProductionService` | 큐 비어있을 때 getQueue() 빈 벡터 반환 |
| `SampleRepository` | filePath 없을 때 findAll() 빈 벡터 반환 |
| `TableFormatter` | 빈 벡터 입력 시 헤더만 출력되는지 확인 |
| `ConsoleView` | printError/printSuccess 정상 출력 확인 (cout 리다이렉션) |

**추가 테스트 파일:**
- Modify: `tests\service\OrderServiceTest.cpp`
- Modify: `tests\service\ProductionServiceTest.cpp`
- Modify: `tests\repository\SampleRepositoryTest.cpp`

---

## Task 7-3: 경계값 + 예외 경로 커버리지

**추가 테스트 케이스:**

| 테스트명 | 검증 내용 |
|---------|----------|
| `PlaceOrderIdFormatCheck` | orderId가 "ORD-YYYYMMDD-NNN" 형식인지 |
| `ApproveOrderWithExactStockConfirms` | stock == quantity 정확히 일치 시 CONFIRMED (경계값) |
| `ProductionJobEnqueuedAtOrdering` | 동일 시간 Job 여러 개 등록 시 저장 순서대로 큐 유지 |
| `SampleSearchCaseInsensitive` | 대소문자 무관 검색 (설계상 case-sensitive면 skip) |
| `ReleaseAlreadyReleasedThrows` | RELEASE 상태 주문 재출고 → invalid_argument |

---

## Task 7-4: 전체 시나리오 최종 검증

**DummyDataGenerator 실행 → 메인 시스템 수동 검증 체크리스트:**

- [ ] 시료 12종 등록 확인 ([1] 시료 관리 → 목록)
- [ ] 주문 36건 확인 ([4] 모니터링)
- [ ] RESERVED 주문 접수 1건 추가 ([2] 주문 접수 → S-001, 100ea)
- [ ] 신규 RESERVED 주문 승인 → 재고 확인 후 CONFIRMED 또는 PRODUCING 분기 확인
- [ ] 생산라인 현황 ([5]) → FIFO 순서 확인
- [ ] 생산 완료 처리 → 재고 증가 + 주문 CONFIRMED 전환 확인
- [ ] CONFIRMED 주문 출고 ([6]) → RELEASE 전환 확인
- [ ] [4] 모니터링 → 최종 상태 수치 일치 확인

---

## Task 7-5: 문서 정리 + 최종 커밋

**체크리스트:**
- [ ] `README.md` 작성: 빌드 방법, 실행 방법, 테스트 실행 방법
- [ ] `CLAUDE.md` 최종 점검: 실제 구조와 일치하는지 확인
- [ ] `data/*.json` 초기화 (빈 배열 `[]`) → 배포용 초기 상태
- [ ] 최종 커버리지 리포트 스크린샷 저장 (docs/coverage_screenshot.png)

**최종 커밋:**
```
feat: complete SemiOrderSystem with 90%+ test coverage
```

---

## 커버리지 목표 요약

| 레이어 | 목표 | 주요 테스트 파일 |
|-------|------|----------------|
| Model | 95%+ | model/*Test.cpp |
| Repository | 90%+ | repository/*Test.cpp |
| Service | 95%+ | service/*Test.cpp |
| Controller/View | 80%+ | 통합 시나리오 |
| **전체** | **90%+** | |
