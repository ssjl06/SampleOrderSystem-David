# Phase 0: 환경 설정

**Goal:** vcpkg 연동, GoogleTest 연결, 각 PoC CLAUDE.md 작성, git 초기화

---

## Task 0-1: SemiOrderSystem 의존성 설정

**Files:**
- Create: `C:\reviewer\SemiOrderSystem\vcpkg.json`

**Steps:**
- [ ] vcpkg.json 작성 — dependencies: `nlohmann-json`, `gtest`
- [ ] `vcpkg\bootstrap-vcpkg.bat` 실행
- [ ] `vcpkg integrate install` (PowerShell 관리자 권한)
- [ ] VS에서 SemiOrderSystem.slnx 열기
- [ ] Solution에 테스트 프로젝트 추가 (Console App C++, 이름: `SemiOrderSystemTests`)
- [ ] 테스트 프로젝트 속성: Include Dirs에 `$(SolutionDir)src`, Linker에 gtestd.lib/gtest_maind.lib
- [ ] `tests\TestMain.cpp` 작성 — gtest + nlohmann 연동 확인용 smoke test
- [ ] Ctrl+Shift+B → Test Explorer → Run All → 1 test PASSED 확인
- [ ] git init + 초기 커밋 (vcpkg.json, CLAUDE.md, PRD.md, docs/)

---

## Task 0-2: PoC 프로젝트 CLAUDE.md 작성

**Files:**
- Create: `C:\reviewer\ConsoleMVC\CLAUDE.md`
- Create: `C:\reviewer\DataPersistence\CLAUDE.md`
- Create: `C:\reviewer\DataMonitor\CLAUDE.md`
- Create: `C:\reviewer\DummyDataGenerator\CLAUDE.md`

**각 CLAUDE.md 포함 내용:** 목적 / 기술스택 / 디렉토리 구조 / 메인 프로젝트 연관 관계

**Steps:**
- [ ] 4개 파일 작성
- [ ] 각 PoC 디렉토리 git init + `docs: add CLAUDE.md` 커밋
