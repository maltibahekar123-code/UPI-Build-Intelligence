#pragma once

#include <string>
#include <chrono>

enum class TransactionType {
    DEBIT,
    CREDIT,
    UNKNOWN
};

struct Transaction {
    std::string   rawMessage;
    double        amount{0.0};
    std::string   merchant;
    std::string   dateStr;
    TransactionType type{TransactionType::UNKNOWN};
    std::string   category{"Uncategorized"}; // To be populated by MerchantTrie
};
