# CLAUDE.md — SemiOrderSystem

## 프로젝트 개요
반도체 시료 생산주문관리 시스템 (S-Semi 사 콘솔 기반 주문 관리)

## 핵심 문서
- **요구사항**: `PRD.md`
- **설계**: `docs/design.md`
- **구현 계획**: `docs/plan.md` (작성 예정)

## 기술 스택
- 언어: C++ (C++17)
- IDE: Visual Studio (.vcxproj)
- 영속성: nlohmann/json → `data/*.json`
- 테스트: GoogleTest / 목표 커버리지 90%+

## 아키텍처
MVC + Repository + Service 레이어드 구조
```
View → Controller → Service → Repository → JSON
```
레이어 상세는 `docs/design.md` 참조

## 코딩 규칙
- 네이밍: PascalCase (클래스), camelCase (메서드/변수)
- 레이어 간 의존 방향: 단방향 (상위 → 하위만 허용)
- 테스트 없는 Service 코드 작성 금지 (TDD)
- 커밋: 기능 단위로, 테스트 통과 후에만

## 주요 도메인 규칙
- 등록된 시료만 주문 가능
- 승인 시 재고 자동 확인 → CONFIRMED 또는 PRODUCING 분기
- 생산라인 스케줄링: FIFO
- 실생산량: `ceil(부족분 / (수율 × 0.9))`
- REJECTED 주문은 모니터링에서 제외

## 디렉토리 구조
```
src/model/          도메인 객체
src/repository/     JSON CRUD 추상화
src/service/        비즈니스 로직
src/controller/     입력 처리 및 흐름 제어
src/view/           콘솔 출력
tests/              GoogleTest 단위 테스트
data/               JSON 영속성 파일
docs/               설계·계획 문서
```
