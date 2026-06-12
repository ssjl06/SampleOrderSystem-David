#pragma once
#include <string>

class ConsoleView {
public:
    static void print(const std::string& msg);
    static void printLine(const std::string& msg = "");
    static void printHeader(const std::string& title);
    static void printDivider();
    static void printSuccess(const std::string& msg);
    static void printError(const std::string& msg);
    static void clearScreen();
    static std::string prompt(const std::string& label);
    static int promptInt(const std::string& label);  // 유효한 정수 입력까지 재시도
};
