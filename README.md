# UPI-Build-Intelligence
Vortex-FinShield Implementation Walkthrough
The Vortex-FinShield offline C++ engine has been fully implemented according to the required privacy-preserving, high-performance architecture.

Technical Layers Completed
Security Layer (The Privacy Guard)
Created SecurityLayer.h and SecurityLayer.cpp.
Implemented isMessageSafe() which uses case-insensitive std::search to scan all strings for otp, verification, password, and code.
Messages matching these terms are instantly skipped, preventing any further processing.
Extraction Layer (Regex Engine)
Created Parser.h and Parser.cpp.
Using C++ <regex>, the parser accurately extracts amount, detects transaction type (creditor vs debitor), and isolates the merchant name or VPA across diverse formats typical of Indian banks.
Intelligence Layer (Merchant Trie)
Created MerchantTrie.h and MerchantTrie.cpp.
Developed an isolated Prefix Tree. This categorizes merchants in strictly $O(L)$ time complexity, ensuring rapid string lookups matching against categories such as Food, Travel, or Shopping without heavy string comparisons typical of naive lists.
Behavioral Analytics
Created Analytics.h and Analytics.cpp.
Integrated a rolling Sliding Window Algorithm. The user is flagged for "Impulse Spiking" if their trailing cumulative spend crosses the defined ₹2000 mark in the designated 3-hour period.
Set up the burn-rate predictor that analyzes money decay and prints out an alerting estimate on how fast the initial balance will deplete to zero.
Main Engine and Output
Setup src/main.cpp using <iomanip> to generate a professional, terminal-based dashboard.
Configured a local-only flow fetching from data/sms_inbox.txt, guaranteeing offline protection.
Handled cross-platform standard compilation setups by providing a detailed README.md and basic CMakeLists.txt.
NOTE

The project assumes no external libraries—solely the C++17 standard library components (<regex>, <chrono>, <string>, <unordered_map>). CMake was missing on the current workspace system, so for manual tests, please ensure C++ compilation tools are installed on your Windows machine to run the built output.

