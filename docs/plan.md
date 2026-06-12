# 구현 계획 인덱스

> **For agentic workers:** 각 페이즈 파일을 순서대로 실행. superpowers:executing-plans 또는 superpowers:subagent-driven-development 사용.

**Goal:** 반도체 시료 생산주문관리 시스템 TDD 기반 단계적 구현

## 페이즈 목록

| 페이즈 | 파일 | 내용 |
|--------|------|------|
| Phase 0 | [phase0-setup.md](plans/phase0-setup.md) | 환경 설정, vcpkg, GoogleTest, PoC CLAUDE.md |
| Phase 1 | [phase1-poc-mvc.md](plans/phase1-poc-mvc.md) | PoC1 ConsoleMVC 스켈레톤 |
| Phase 2 | [phase2-poc-persistence.md](plans/phase2-poc-persistence.md) | PoC2 DataPersistence JSON CRUD |
| Phase 3 | [phase3-poc-tools.md](plans/phase3-poc-tools.md) | PoC3 DataMonitor + PoC4 DummyDataGenerator |
| Phase 4a | [phase4-model.md](plans/phase4-model.md) | 메인: Model (도메인 객체 + IRepository 인터페이스) |
| Phase 4b | [phase4-repository.md](plans/phase4-repository.md) | 메인: Repository 3개 구현 + 테스트 |
| Phase 5 | [phase5-service.md](plans/phase5-service.md) | 메인: Service 레이어 TDD (핵심 비즈니스 로직) |
| Phase 6 | [phase6-controller-view.md](plans/phase6-controller-view.md) | 메인: Controller + View + 통합 |
| Phase 7 | [phase7-final.md](plans/phase7-final.md) | 커버리지 90% 달성, 리팩토링, 최종 커밋 |

## 핵심 원칙
- 각 Phase는 TDD (Red → Green → Blue) 사이클로 진행
- 테스트 통과 후에만 커밋
- 레이어 의존 방향: View → Controller → Service → Repository → JSON (단방향)
- 생산량 공식: `actualProduction = ceil(shortage / (yield × 0.9))`
