#include "Parser.h"
#include <iostream>

Parser::Parser() {
    // Regex for amounts like Rs. 1000, INR 500.00, Rs 50.5
    amountRegexes = {
        std::regex(R"((?:Rs\.?|INR)\s*([\d,]+\.?\d*))", std::regex_constants::icase)
    };

    // Regex for merchant/VPA like "to VPA abc@upi", "Info: Zomato", "to Swiggy"
    merchantRegexes = {
        std::regex(R"(to\s+VPA\s+([A-Za-z0-9\.\-@]+))", std::regex_constants::icase),
        std::regex(R"(Info:\s*([A-Za-z0-9\s]+?)(?:\.|$))", std::regex_constants::icase),
        std::regex(R"(to\s+([A-Za-z0-9\s]+?)(?:\s+ref|\s+on|\.|$))", std::regex_constants::icase)
    };

    // Credit/Debit keywords
    debitRegex = std::regex(R"((debited|spent|deducted))", std::regex_constants::icase);
    creditRegex = std::regex(R"((credited|received|added))", std::regex_constants::icase);

    // Simple date regex for DD-MM-YYYY, DD/MM/YYYY, DD-MMM-YY
    dateRegex = std::regex(R"((\d{2}[-/]\d{2}[-/]\d{2,4}|\d{2}-[A-Za-z]{3}-\d{2,4}))");
}

std::optional<double> Parser::extractAmount(const std::string& msg) const {
    std::smatch match;
    for (const auto& r : amountRegexes) {
        if (std::regex_search(msg, match, r) && match.size() > 1) {
            std::string amountStr = match[1].str();
            // Remove commas
            amountStr.erase(std::remove(amountStr.begin(), amountStr.end(), ','), amountStr.end());
            try {
                return std::stod(amountStr);
            } catch (...) {
                return std::nullopt;
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string> Parser::extractMerchant(const std::string& msg) const {
    std::smatch match;
    for (const auto& r : merchantRegexes) {
        if (std::regex_search(msg, match, r) && match.size() > 1) {
            return match[1].str();
        }
    }
    return std::nullopt;
}

std::optional<std::string> Parser::extractDate(const std::string& msg) const {
    std::smatch match;
    if (std::regex_search(msg, match, dateRegex) && match.size() > 1) {
        return match[1].str();
    }
    return std::nullopt;
}

TransactionType Parser::determineType(const std::string& msg) const {
    if (std::regex_search(msg, debitRegex)) {
        return TransactionType::DEBIT;
    } else if (std::regex_search(msg, creditRegex)) {
        return TransactionType::CREDIT;
    }
    return TransactionType::UNKNOWN;
}

std::optional<Transaction> Parser::parseMessage(const std::string& message) const {
    Transaction tx;
    tx.rawMessage = message;

    auto amount = extractAmount(message);
    if (!amount) return std::nullopt;
    tx.amount = *amount;

    auto type = determineType(message);
    if (type == TransactionType::UNKNOWN) return std::nullopt;
    tx.type = type;

    auto date = extractDate(message);
    tx.dateStr = date ? *date : "Unknown Date";

    auto merchant = extractMerchant(message);
    tx.merchant = merchant ? *merchant : "Unknown Merchant";

    return tx;
}
