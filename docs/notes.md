# 개발 노트 (트러블슈팅)

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
