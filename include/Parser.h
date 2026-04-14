#pragma once

#include "Transaction.h"
#include <string>
#include <optional>
#include <vector>
#include <regex>

class Parser {
public:
    Parser();

    // Attempts to parse an SMS message to extract transaction details.
    // Returns a populated Transaction object if successful, std::nullopt otherwise.
    std::optional<Transaction> parseMessage(const std::string& message) const;

private:
    std::vector<std::regex> amountRegexes;
    std::vector<std::regex> merchantRegexes;
    std::regex debitRegex;
    std::regex creditRegex;
    std::regex dateRegex;

    std::optional<double> extractAmount(const std::string& msg) const;
    std::optional<std::string> extractMerchant(const std::string& msg) const;
    std::optional<std::string> extractDate(const std::string& msg) const;
    TransactionType determineType(const std::string& msg) const;
};
