# Phase 1: PoC 1 — ConsoleMVC 스켈레톤

**Goal:** MVC 레이어 분리 패턴 검증 — Sample 등록/조회로 각 레이어 역할 시연

**경로:** `C:\reviewer\ConsoleMVC\`

---

## Task 1-1: Model + Repository (인메모리)

**Files:**
- Create: `src\model\Sample.h` — sampleId, name, avgProductionTime, yield, stock
- Create: `src\repository\SampleRepository.h / .cpp` — save / findById / findAll (인메모리 vector)

---

## Task 1-2: Service

**Files:**
- Create: `src\service\SampleService.h / .cpp`

**Signatures:**
```cpp
SampleService(SampleRepository& repo);
void registerSample(const Sample& sample);
std::vector<Sample> getAllSamples() const;
std::optional<Sample> findById(const std::string& id) const;
```

---

## Task 1-3: View + Controller + main.cpp

**Files:**
- Create: `src\view\ConsoleView.h` — showSampleList / showMessage / prompt (static)
- Create: `src\controller\SampleController.h / .cpp` — run / handleRegister / handleList
- Create: `src\main.cpp` — Repository → Service → Controller 조립 후 run()

**Steps:**
- [ ] 빌드 → 실행 → `[1]` 시료 등록, `[2]` 목록 조회 동작 확인
- [ ] Commit: `feat: implement ConsoleMVC skeleton`
