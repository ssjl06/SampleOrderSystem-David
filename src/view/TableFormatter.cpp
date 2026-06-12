#include "TableFormatter.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>

// UTF-8 한글은 화면에서 2칸 차지 → displayWidth로 패딩 보정
static int displayWidth(const std::string& s) {
    int w = 0;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = (unsigned char)s[i];
        if      (c >= 0xE0) { w += 2; i += 3; }  // 한글 등 3바이트 UTF-8 → 2칸
        else if (c >= 0xC0) { w += 1; i += 2; }  // 2바이트 UTF-8
        else                { w += 1; i += 1; }  // ASCII
    }
    return w;
}

static std::string padRight(const std::string& s, int width) {
    int pad = width - displayWidth(s);
    return s + (pad > 0 ? std::string(pad, ' ') : "");
}

static std::string padLeft(const std::string& s, int width) {
    int pad = width - displayWidth(s);
    return (pad > 0 ? std::string(pad, ' ') : "") + s;
}

void TableFormatter::printSampleTable(const std::vector<Sample>& samples) {
    if (samples.empty()) { std::cout << "  (등록된 시료 없음)\n"; return; }
    const std::string sep = "+----------+----------------------+-------+--------+----------+";
    std::cout << sep << "\n"
              << "| " << padRight("ID",     8)  << " | "
              << padRight("이름",           20) << " | "
              << padLeft("재고",            5)  << " | "
              << padLeft("수율",            6)  << " | "
              << padLeft("sec/ea",          8)  << " |\n"
              << sep << "\n";
    for (const auto& s : samples) {
        std::cout << "| " << padRight(s.sampleId, 8) << " | "
                  << padRight(s.name,              20) << " | "
                  << padLeft(std::to_string(s.stock), 5) << " | "
                  << padLeft([&]{ std::ostringstream o; o << std::fixed << std::setprecision(2) << s.yield; return o.str(); }(), 6) << " | "
                  << padLeft([&]{ std::ostringstream o; o << std::fixed << std::setprecision(1) << s.avgProductionTime; return o.str(); }(), 8) << " |\n";
    }
    std::cout << sep << "\n";
}

void TableFormatter::printOrderTable(const std::vector<Order>& orders) {
    if (orders.empty()) { std::cout << "  (주문 없음)\n"; return; }
    const std::string sep = "+------------------------+----------+----------------+-------+-----------+";
    std::cout << sep << "\n"
              << "| " << padRight("주문번호",  22) << " | "
              << padRight("시료ID",             8) << " | "
              << padRight("고객명",            14) << " | "
              << padLeft("수량",               5)  << " | "
              << padRight("상태",              9)  << " |\n"
              << sep << "\n";
    for (const auto& o : orders) {
        std::cout << "| " << padRight(o.orderId,           22) << " | "
                  << padRight(o.sampleId,                   8) << " | "
                  << padRight(o.customerName,               14) << " | "
                  << padLeft(std::to_string(o.quantity),    5)  << " | "
                  << padRight(toString(o.status),           9)  << " |\n";
    }
    std::cout << sep << "\n";
}

void TableFormatter::printProductionQueue(const std::vector<ProductionJob>& jobs) {
    if (jobs.empty()) { std::cout << "  (생산 대기 없음)\n"; return; }
    const std::string sep = "+----+------------------------+----------+----------+------------+";
    std::cout << sep << "\n"
              << "| "  << padLeft("No",  2)  << " | "
              << padRight("주문번호",    22)  << " | "
              << padRight("시료ID",       8)  << " | "
              << padLeft("실생산량",     8)   << " | "
              << padLeft("총시간(sec)", 10)   << " |\n"
              << sep << "\n";
    for (size_t i = 0; i < jobs.size(); i++) {
        std::cout << "| "  << padLeft(std::to_string(i + 1), 2) << " | "
                  << padRight(jobs[i].orderId,                22) << " | "
                  << padRight(jobs[i].sampleId,                8) << " | "
                  << padLeft(std::to_string(jobs[i].actualProduction), 8) << " | "
                  << padLeft([&]{ std::ostringstream o; o << std::fixed << std::setprecision(1) << jobs[i].totalTime; return o.str(); }(), 10) << " |\n";
    }
    std::cout << sep << "\n";
}

void TableFormatter::printStockStatus(const std::vector<Sample>& samples,
                                      const std::vector<Order>& activeOrders) {
    std::map<std::string, int> demanded;
    for (const auto& o : activeOrders)
        demanded[o.sampleId] += o.quantity;

    const std::string sep = "+----------+----------------------+-------+------+";
    std::cout << sep << "\n"
              << "| " << padRight("ID",    8)  << " | "
              << padRight("이름",          20)  << " | "
              << padLeft("재고",            5)  << " | "
              << padRight("상태",           4)  << " |\n"
              << sep << "\n";
    for (const auto& s : samples) {
        int demand  = demanded.count(s.sampleId) ? demanded[s.sampleId] : 0;
        std::string status = s.stock == 0 ? "고갈" :
                             s.stock < demand ? "부족" : "여유";
        std::cout << "| " << padRight(s.sampleId, 8)              << " | "
                  << padRight(s.name,              20)             << " | "
                  << padLeft(std::to_string(s.stock), 5)          << " | "
                  << padRight(status,               4)             << " |\n";
    }
    std::cout << sep << "\n";
}
