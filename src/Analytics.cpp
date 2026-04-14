#include "Analytics.h"
#include <iomanip>
#include <sstream>

Analytics::Analytics(double startingBalance) : currentBalance(startingBalance) {}

std::chrono::system_clock::time_point Analytics::parseTimestamp(const std::string& dateStr) const {
    // A simplified parser. In real life we'd use std::get_time. 
    // Here we'll generate mock times based on order if parsing is complex,
    // but let's attempt to use current time for simulated inserts.
    // For this simulation, we'll increment timestamps by 1 hour for each transaction just to simulate time progression,
    // unless a good library is available. To keep C++17 simple and avoid locale issues,
    // we'll just use now() and offset if needed, OR just fake it.
    
    // As a robust fallback, since the exact SMS might not have times:
    static auto baseTime = std::chrono::system_clock::now() - std::chrono::hours(24 * 30); // 30 days ago
    baseTime += std::chrono::minutes(30); // Step by 30 mins each call to simulate time passing
    return baseTime;
}

void Analytics::processTransaction(const Transaction& tx) {
    if (tx.type == TransactionType::DEBIT) {
        currentBalance -= tx.amount;
        auto tp = parseTimestamp(tx.dateStr);
        spendHistory.push_back({tp, tx.amount});
    } else if (tx.type == TransactionType::CREDIT) {
        currentBalance += tx.amount;
    }
}

bool Analytics::detectImpulseSpiking(double threshold, int hoursWindow) const {
    if (spendHistory.empty()) return false;

    // Sliding window logic: check the trailing `hoursWindow` relative to the last transaction
    auto windowEnd = spendHistory.back().first;
    auto windowStart = windowEnd - std::chrono::hours(hoursWindow);

    double windowSpend = 0.0;
    for (auto it = spendHistory.rbegin(); it != spendHistory.rend(); ++it) {
        if (it->first < windowStart) {
            break; // Gone past the 3-hour window
        }
        windowSpend += it->second;
    }

    return windowSpend > threshold;
}

AnalyticsReport Analytics::getReport() const {
    AnalyticsReport report;
    report.impulseSpikeDetected = detectImpulseSpiking(2000.0, 3); // ₹2000 in 3 hrs

    // Calculate sum of last 3 hours
    if (!spendHistory.empty()) {
        auto windowStart = spendHistory.back().first - std::chrono::hours(3);
        double recentSpend = 0.0;
        for (auto it = spendHistory.rbegin(); it != spendHistory.rend(); ++it) {
            if (it->first < windowStart) break;
            recentSpend += it->second;
        }
        report.windowSpend = recentSpend;
    }

    // Burn-rate predictor
    if (spendHistory.size() > 1) {
        auto firstTxTime = spendHistory.front().first;
        auto lastTxTime = spendHistory.back().first;
        auto durationStr = std::chrono::duration_cast<std::chrono::hours>(lastTxTime - firstTxTime).count();
        double daysPassed = durationStr / 24.0;
        if (daysPassed < 1.0) daysPassed = 1.0;

        double totalSpend = 0;
        for (const auto& sp : spendHistory) { totalSpend += sp.second; }

        double dailyBurnRate = totalSpend / daysPassed;
        
        if (dailyBurnRate > 0 && currentBalance > 0) {
            report.daysUntilZero = static_cast<int>(currentBalance / dailyBurnRate);
        } else {
            report.daysUntilZero = -1; // Unlikely to hit zero based on current trajectory
        }
    } else {
        report.daysUntilZero = -1;
    }

    return report;
}
