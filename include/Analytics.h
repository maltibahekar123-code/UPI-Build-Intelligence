#pragma once

#include "Transaction.h"
#include <vector>
#include <chrono>
#include <string>

struct AnalyticsReport {
    bool impulseSpikeDetected{false};
    double windowSpend{0.0};
    int daysUntilZero{0}; // -1 if not decreasing
};

class Analytics {
public:
    Analytics(double startingBalance);

    // Processes a new transaction, updating internal sliding window and balance
    void processTransaction(const Transaction& tx);

    // Get real-time spend analytics
    AnalyticsReport getReport() const;

    // Helper: Sliding window algorithm for impulse detection
    bool detectImpulseSpiking(double threshold, int hoursWindow) const;

    double getCurrentBalance() const { return currentBalance; }

private:
    double currentBalance;
    std::vector<std::pair<std::chrono::system_clock::time_point, double>> spendHistory;

    // Parses a simple date/time structure "DD/MM/YYYY HH:MM:SS" into time_point
    std::chrono::system_clock::time_point parseTimestamp(const std::string& dateStr) const;
};
