# Phase 2: PoC 2 — DataPersistence JSON CRUD

**Goal:** nlohmann/json으로 Sample을 JSON 파일에 CRUD, 앱 재시작 후 데이터 유지 확인

**경로:** `C:\reviewer\DataPersistence\`

---

## Task 2-1: Sample 모델 (JSON 직렬화 포함)

**Files:**
- Create: `src\model\Sample.h`

**핵심:** `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Sample, sampleId, name, avgProductionTime, yield, stock)`

---

## Task 2-2: JsonSampleRepository

**Files:**
- Create: `src\repository\JsonSampleRepository.h / .cpp`

**Signatures:**
```cpp
JsonSampleRepository(const std::string& filePath);
void save(const Sample& sample);
std::optional<Sample> findById(const std::string& id) const;
std::vector<Sample> findAll() const;
void update(const Sample& sample);
void remove(const std::string& id);
private:
std::vector<Sample> load() const;        // ifstream → json → vector
void persist(const std::vector<Sample>&) const; // vector → json → ofstream
```

---

## Task 2-3: main.cpp — CRUD 시나리오 검증

**Files:**
- Create: `src\main.cpp`

**검증 시나리오:**
1. CREATE — S-001, S-002 저장
2. READ ALL — 목록 출력
3. UPDATE — S-001 stock 변경
4. DELETE — S-002 제거
5. 앱 재실행 → `data/samples.json` 데이터 유지 확인

**Steps:**
- [ ] 빌드 → 실행 → `data/samples.json` 생성 확인
- [ ] 앱 재실행 → 동일 데이터 로드 확인 (영속성)
- [ ] Commit: `feat: implement JSON CRUD persistence`
