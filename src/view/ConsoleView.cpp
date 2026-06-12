#include "ConsoleView.h"
#include <iostream>
#include <limits>

void ConsoleView::print(const std::string& msg)    { std::cout << msg; }
void ConsoleView::printLine(const std::string& msg) { std::cout << msg << "\n"; }
void ConsoleView::printDivider()                    { std::cout << std::string(50, '-') << "\n"; }
void ConsoleView::printSuccess(const std::string& msg) { std::cout << "[OK] " << msg << "\n"; }
void ConsoleView::printError(const std::string& msg)   { std::cout << "[오류] " << msg << "\n"; }

void ConsoleView::printHeader(const std::string& title) {
    std::cout << "\n+" << std::string(48, '-') << "+\n";
    std::cout << "|  " << title << "\n";
    std::cout << "+" << std::string(48, '-') << "+\n";
}

void ConsoleView::clearScreen() {
    system("cls");
}

std::string ConsoleView::prompt(const std::string& label) {
    std::cout << label << ": ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int ConsoleView::promptInt(const std::string& label) {
    while (true) {
        std::cout << label << ": ";
        int val;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[오류] 숫자를 입력해주세요.\n";
    }
}
