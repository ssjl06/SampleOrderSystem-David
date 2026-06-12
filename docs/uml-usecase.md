# 유스케이스 다이어그램

```mermaid
flowchart LR
    %% ── Actors ──────────────────────────────────────────────
    A1(["👤 시스템 담당자"])

    %% ── System Boundary ─────────────────────────────────────
    subgraph SYS["S-Semi 반도체 시료 생산주문관리 시스템"]

        subgraph UC1["[1] 시료 관리"]
            U1_1["시료 등록"]
            U1_2["시료 목록 조회"]
            U1_3["시료 이름 검색"]
        end

        subgraph UC2["[2] 시료 주문"]
            U2_1["주문 접수\n(RESERVED 생성)"]
        end

        subgraph UC3["[3] 주문 승인/거절"]
            U3_1["RESERVED 주문 목록 조회"]
            U3_2["주문 승인\n(재고 충분 → CONFIRMED)"]
            U3_3["주문 승인\n(재고 부족 → PRODUCING + 생산Job 생성)"]
            U3_4["주문 거절\n(→ REJECTED)"]
        end

        subgraph UC4["[4] 모니터링"]
            U4_1["상태별 주문 수 조회\n(RESERVED/CONFIRMED/PRODUCING/RELEASE)"]
            U4_2["시료별 재고 현황 조회\n(여유/부족/고갈)"]
        end

        subgraph UC5["[5] 생산라인 조회"]
            U5_1["현재 생산 중 작업 확인"]
            U5_2["생산 대기 큐 조회 (FIFO)"]
            U5_3["생산 완료 처리\n(재고 증가, → CONFIRMED)"]
        end

        subgraph UC6["[6] 출고 처리"]
            U6_1["CONFIRMED 주문 목록 조회"]
            U6_2["출고 실행\n(→ RELEASE)"]
        end

    end

    %% ── Actor → UseCase 연결 ────────────────────────────────
    A1 --> U1_1
    A1 --> U1_2
    A1 --> U1_3
    A1 --> U2_1
    A1 --> U3_1
    A1 --> U3_2
    A1 --> U3_3
    A1 --> U3_4
    A1 --> U4_1
    A1 --> U4_2
    A1 --> U5_1
    A1 --> U5_2
    A1 --> U5_3
    A1 --> U6_1
    A1 --> U6_2
```

## 주문 상태 전이 흐름

```mermaid
stateDiagram-v2
    [*] --> RESERVED : 주문 접수 (UC2)

    RESERVED --> CONFIRMED : 승인 + 재고 충분 (UC3)
    RESERVED --> PRODUCING : 승인 + 재고 부족 (UC3)
    RESERVED --> REJECTED  : 거절 (UC3)

    PRODUCING --> CONFIRMED : 생산 완료 처리 (UC5)

    CONFIRMED --> RELEASE : 출고 실행 (UC6)

    REJECTED --> [*]
    RELEASE  --> [*]
```

## 주요 비즈니스 규칙 요약

| 유스케이스 | 사전조건 | 핵심 규칙 |
|-----------|---------|---------|
| 주문 접수 | 시료가 등록되어 있어야 함 | 미등록 시료 → 오류 |
| 주문 승인 | 상태가 RESERVED여야 함 | 재고 ≥ 주문량 → CONFIRMED, 재고 < 주문량 → PRODUCING |
| 주문 거절 | 상태가 RESERVED여야 함 | CONFIRMED/PRODUCING 거절 불가 |
| 생산 완료 | 생산 큐가 비어있지 않아야 함 | FIFO 순서, 실생산량 = ⌈부족분 / (수율 × 0.9)⌉ |
| 출고 실행 | 상태가 CONFIRMED여야 함 | 다른 상태 → 오류 |
| 모니터링 | - | REJECTED 주문은 집계에서 제외 |
