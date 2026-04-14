#pragma once

#include <string>
#include <vector>

class SecurityLayer {
public:
    // Initialize the security layer with default sensitive keywords
    SecurityLayer();

    // Check if the given SMS message is safe to parse
    // Returns true if safe, false if it contains sensitive keywords
    bool isMessageSafe(const std::string& message) const;

private:
    std::vector<std::string> sensitiveKeywords;
    
    // Helper for case-insensitive string search
    static bool containsIgnoreCase(const std::string& str, const std::string& searchStr);
};
