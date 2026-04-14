#include "SecurityLayer.h"
#include <algorithm>
#include <cctype>

SecurityLayer::SecurityLayer() {
    sensitiveKeywords = {"otp", "verification", "password", "code"};
}

bool SecurityLayer::containsIgnoreCase(const std::string& str, const std::string& searchStr) {
    if (searchStr.length() > str.length()) return false;
    auto it = std::search(
        str.begin(), str.end(),
        searchStr.begin(), searchStr.end(),
        [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
    );
    return (it != str.end());
}

bool SecurityLayer::isMessageSafe(const std::string& message) const {
    for (const auto& keyword : sensitiveKeywords) {
        if (containsIgnoreCase(message, keyword)) {
            return false;
        }
    }
    return true;
}
