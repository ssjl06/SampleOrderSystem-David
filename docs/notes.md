# 개발 노트

## PoC 코드 작성 규칙

**원칙:** PoC는 "이렇게 동작하는구나"를 한눈에 파악할 수 있는 수준으로 간소화

**주석 규칙:** 외부 라이브러리 사용 부분과 레이어 역할은 반드시 주석으로 설명

```cpp
// [Repository 역할] 저장소 - 데이터를 어떻게 저장/조회할지만 담당
// [Service 역할] 비즈니스 규칙 담당 - "이 작업을 해도 되는가"를 판단
// [Controller 역할] 사용자 입력 → 적절한 Service 호출 → View에 결과 전달
// [View 역할] 출력과 입력만 담당 - 데이터가 어디서 왔는지 모름
// [조립 지점] 각 레이어 객체를 생성하고 의존성을 주입

// nlohmann 매크로: Sample ↔ JSON 변환 코드를 자동 생성 (to_json / from_json)
// nlohmann::json(data).dump(2)  → vector<T>를 JSON 문자열로 직렬화
// nlohmann::json::parse(f).get<vector<T>>()  → JSON 파일을 vector<T>로 역직렬화
```

**간소화 기준:**
- 각 파일 30줄 이내 유지
- 예외 처리, 입력 유효성 검사, 포매팅은 생략
- 핵심 흐름(레이어 연결, 라이브러리 동작)만 코드로 표현

---

## 트러블슈팅

## [Phase 1] Windows 콘솔 한글 깨짐

**증상:** `SetConsoleOutputCP(CP_UTF8)` 추가 후에도 한글 문자열이 `?록`, `?택` 형태로 출력됨

**원인:** MSVC가 소스 파일을 시스템 인코딩(CP949)으로 읽어 컴파일 시점에 한글 리터럴이 깨짐.  
런타임 코드페이지 설정만으로는 이미 깨진 바이트를 복구할 수 없음.

**해결:** vcxproj의 모든 구성(Debug/Release × Win32/x64)에 `/utf-8` 컴파일러 플래그 추가

```xml
<AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>
```

**적용 대상:** ConsoleMVC, DataPersistence, DataMonitor, DummyDataGenerator, SemiOrderSystem  
→ 모든 프로젝트 vcxproj에 동일하게 적용할 것

**main.cpp 세트:**
```cpp
#include <windows.h>
// main() 첫 줄
SetConsoleOutputCP(CP_UTF8);
SetConsoleCP(CP_UTF8);
```
