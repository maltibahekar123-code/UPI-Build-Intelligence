#include "MerchantTrie.h"

MerchantTrie::MerchantTrie() {
    root = std::make_unique<TrieNode>();
}

std::string MerchantTrie::toLowercase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

void MerchantTrie::insert(const std::string& merchant, const std::string& category) {
    std::string key = toLowercase(merchant);
    TrieNode* current = root.get();

    for (char ch : key) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = std::make_unique<TrieNode>();
        }
        current = current->children[ch].get();
    }
    
    current->isEndOfWord = true;
    current->category = category;
}

std::string MerchantTrie::getCategory(const std::string& merchant) const {
    std::string key = toLowercase(merchant);
    TrieNode* current = root.get();

    // Iterate through the characters. We also want to support partial matches 
    // e.g., if Trie has "zomato" and the sms says "zomato online", we want to match "zomato".
    // A simpler way: we just walk the trie until we mis-match, and if we ever passed an isEndOfWord,
    // we keep track of the best category.
    
    std::string bestCategory = "Uncategorized";
    for (char ch : key) {
        if (current->children.find(ch) == current->children.end()) {
            break; // Mismatch
        }
        current = current->children[ch].get();
        if (current->isEndOfWord) {
            bestCategory = current->category;
        }
    }

    return bestCategory;
}
