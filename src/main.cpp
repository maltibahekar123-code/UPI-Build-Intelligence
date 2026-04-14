#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "SecurityLayer.h"
#include "Parser.h"
#include "MerchantTrie.h"
#include "Analytics.h"

int main() {
    std::cout << "Starting Vortex-FinShield Offline Processing Engine..." << std::endl;

    // 1. Initialize Components
    SecurityLayer security;
    Parser parser;
    MerchantTrie trie;
    Analytics analytics(50000.0); // Assuming 50k initial balance as a default

    // Populate Merchant Trie
    trie.insert("Zomato", "Food");
    trie.insert("Swiggy", "Food");
    trie.insert("Domino", "Food");
    trie.insert("Uber", "Travel");
    trie.insert("Ola", "Travel");
    trie.insert("Amazon", "Shopping");
    trie.insert("Flipkart", "Shopping");
    trie.insert("Myntra", "Shopping");
    trie.insert("Netflix", "Entertainment");

    // 2. Read Offline Data
    std::string filename = "../data/sms_inbox.txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        // Try fallback for different execution directory
        file.open("data/sms_inbox.txt");
        if (!file.is_open()) {
            std::cerr << "Error: Could not open data/sms_inbox.txt" << std::endl;
            return 1;
        }
    }

    std::vector<Transaction> validTransactions;
    std::string line;
    int totalProcessed = 0;
    int droppedOTP = 0;

    std::cout << "\nScanning SMS Inbox...\n" << std::endl;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        totalProcessed++;

        // 3. Security Layer
        if (!security.isMessageSafe(line)) {
            droppedOTP++;
            continue;
        }

        // 4. Extraction Layer
        auto optTx = parser.parseMessage(line);
        if (optTx) {
            Transaction tx = *optTx;
            
            // 5. Categorize Merchant
            tx.category = trie.getCategory(tx.merchant);
            
            // 6. Analytics Layer
            analytics.processTransaction(tx);

            validTransactions.push_back(tx);
        }
    }

    // 7. Output Result Dashboard
    std::cout << std::string(85, '=') << std::endl;
    std::cout << std::setw(50) << "VORTEX-FINSHIELD DASHBOARD" << std::endl;
    std::cout << std::string(85, '=') << std::endl;

    std::cout << std::left 
              << std::setw(15) << "Date" 
              << std::setw(12) << "Type" 
              << std::setw(12) << "Amount" 
              << std::setw(25) << "Merchant/VPA" 
              << std::setw(15) << "Category" 
              << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    for (const auto& tx : validTransactions) {
        std::string typeStr = tx.type == TransactionType::DEBIT ? "DEBIT" : "CREDIT";
        std::cout << std::left 
                  << std::setw(15) << tx.dateStr 
                  << std::setw(12) << typeStr 
                  << "Rs." << std::setw(9) << std::fixed << std::setprecision(2) << tx.amount 
                  << std::setw(25) << tx.merchant.substr(0, 24) 
                  << std::setw(15) << tx.category 
                  << std::endl;
    }

    std::cout << std::string(85, '=') << std::endl;
    
    // Analytics Metrics
    AnalyticsReport report = analytics.getReport();
    std::cout << "SUMMARY STATISTICS:" << std::endl;
    std::cout << "- Total SMS Scanned : " << totalProcessed << std::endl;
    std::cout << "- Blocked OTP/Auth  : " << droppedOTP << " (Dropped for Privacy)" << std::endl;
    std::cout << "- Remaining Balance : Rs. " << std::fixed << std::setprecision(2) << analytics.getCurrentBalance() << std::endl;
    
    std::cout << "\nBEHAVIORAL NUDGES:" << std::endl;
    if (report.impulseSpikeDetected) {
        std::cout << "  [!!!] IMPULSE SPIKING DETECTED: You spent Rs. " << report.windowSpend 
                  << " recently. High velocity spending alert!" << std::endl;
    } else {
        std::cout << "  [ OK] Spending velocity is normal." << std::endl;
    }

    if (report.daysUntilZero > 0) {
        std::cout << "  [WARN] Based on your current burn-rate, your balance may reach ZERO in approx " 
                  << report.daysUntilZero << " days." << std::endl;
    }

    std::cout << std::string(85, '=') << std::endl;

    return 0;
}
