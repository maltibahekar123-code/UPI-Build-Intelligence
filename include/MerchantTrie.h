#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cctype>

struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool isEndOfWord{false};
    std::string category{"Uncategorized"};
};

class MerchantTrie {
public:
    MerchantTrie();

    // Insert a merchant prefix and its corresponding category into the Trie
    void insert(const std::string& merchant, const std::string& category);

    // Search for a merchant and return its category in O(L) time where L is length of merchant string
    std::string getCategory(const std::string& merchant) const;

private:
    std::unique_ptr<TrieNode> root;
    
    // Helper to lower case strings to keep search case-insensitive
    static std::string toLowercase(const std::string& str);
};
