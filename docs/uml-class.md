# 클래스 다이어그램 (설계 UML)

```mermaid
classDiagram
    %% ── Domain Models ──────────────────────────────────────
    class OrderStatus {
        <<enumeration>>
        RESERVED
        REJECTED
        PRODUCING
        CONFIRMED
        RELEASE
    }

    class Sample {
        +string sampleId
        +string name
        +double avgProductionTime
        +double yield
        +int stock
        +validate()
    }

    class Order {
        +string orderId
        +string sampleId
        +string customerName
        +int quantity
        +OrderStatus status
        +string createdAt
        +validate()
    }

    class ProductionJob {
        +string orderId
        +string sampleId
        +int shortage
        +int actualProduction
        +double totalTime
        +string enqueuedAt
    }

    Order --> OrderStatus

    %% ── Repository Layer ────────────────────────────────────
    class IRepository~T~ {
        <<interface>>
        +save(T)
        +findById(string) optional~T~
        +findAll() vector~T~
        +update(T)
        +remove(string)
    }

    class SampleRepository {
        -string filePath
        -load() vector~Sample~
        -persist(vector~Sample~)
    }

    class OrderRepository {
        -string filePath
        +findByStatus(OrderStatus) vector~Order~
        -load() vector~Order~
        -persist(vector~Order~)
    }

    class ProductionRepository {
        -string filePath
        +front() optional~ProductionJob~
        +getQueue() vector~ProductionJob~
        -load() vector~ProductionJob~
        -persist(vector~ProductionJob~)
    }

    IRepository~T~ <|-- SampleRepository : implements
    IRepository~T~ <|-- OrderRepository : implements
    IRepository~T~ <|-- ProductionRepository : implements

    SampleRepository ..> Sample : CRUD
    OrderRepository ..> Order : CRUD
    ProductionRepository ..> ProductionJob : CRUD

    %% ── Service Layer ───────────────────────────────────────
    class SampleService {
        +registerSample(Sample)
        +getAllSamples() vector~Sample~
        +findById(string) optional~Sample~
        +searchByName(string) vector~Sample~
    }

    class OrderService {
        +placeOrder(string, string, int) string
        +approveOrder(string)
        +rejectOrder(string)
        +getReservedOrders() vector~Order~
    }

    class ProductionService {
        +getCurrentJob() optional~ProductionJob~
        +getQueue() vector~ProductionJob~
        +completeCurrentJob()
    }

    class ReleaseService {
        +releaseOrder(string)
        +getConfirmedOrders() vector~Order~
    }

    SampleService --> SampleRepository
    OrderService --> SampleRepository
    OrderService --> OrderRepository
    OrderService --> ProductionRepository
    ProductionService --> SampleRepository
    ProductionService --> OrderRepository
    ProductionService --> ProductionRepository
    ReleaseService --> OrderRepository

    %% ── View Layer ──────────────────────────────────────────
    class ConsoleView {
        <<static>>
        +print(string)
        +printHeader(string)
        +prompt(string) string
        +promptInt(string) int
        +printSuccess(string)
        +printError(string)
        +clearScreen()
    }

    class TableFormatter {
        <<static>>
        +printSampleTable(vector~Sample~)
        +printOrderTable(vector~Order~)
        +printProductionQueue(vector~ProductionJob~)
        +printStockStatus(vector~Sample~, vector~Order~)
    }

    %% ── Controller Layer ────────────────────────────────────
    class MainController {
        +run()
    }

    class SampleController {
        +run()
    }

    class OrderController {
        +run()
    }

    class MonitorController {
        +run()
    }

    class ProductionController {
        +run()
    }

    class ReleaseController {
        +run()
    }

    MainController --> SampleController
    MainController --> OrderController
    MainController --> MonitorController
    MainController --> ProductionController
    MainController --> ReleaseController
    MainController --> ConsoleView

    SampleController --> SampleService
    SampleController --> ConsoleView
    SampleController --> TableFormatter

    OrderController --> OrderService
    OrderController --> ConsoleView
    OrderController --> TableFormatter

    MonitorController --> SampleService
    MonitorController --> OrderService
    MonitorController --> TableFormatter

    ProductionController --> ProductionService
    ProductionController --> ConsoleView
    ProductionController --> TableFormatter

    ReleaseController --> ReleaseService
    ReleaseController --> ConsoleView
    ReleaseController --> TableFormatter
```

## 레이어 의존 방향 요약

```
View (ConsoleView, TableFormatter)
         ↑
Controller (Main / Sample / Order / Monitor / Production / Release)
         ↓
Service (SampleService / OrderService / ProductionService / ReleaseService)
         ↓
Repository (SampleRepository / OrderRepository / ProductionRepository)
         ↓
JSON 파일 (data/samples.json, orders.json, production_jobs.json)
```

> 모든 의존은 상위 → 하위 단방향. 하위 레이어는 상위를 알지 못합니다.
