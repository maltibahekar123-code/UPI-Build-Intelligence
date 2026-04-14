# Vortex-FinShield

A high-performance C++ engine designed to parse UPI transaction SMS data locally, identify financial leaks, and prevent impulse spending through behavioral nudges. 

## Features
- **100% Offline Processing**: Prioritizes privacy with a local-only architecture.
- **Security Pre-Processing**: Drops SMS messages containing sensitive keywords (like `OTP`, `password`) immediately.
- **Regex Extraction Layer**: Handles assorted Indian bank SMS formats to accurately extract value, merchant, and timestamp.
- **Intelligence Layer (Merchant Trie)**: Fast categorization using an $O(L)$ Prefix Tree.
- **Behavioral Analytics**: Sliding window calculations detect Spending Velocity, while a Burn-Rate predictor estimates days till an empty account.

## Complexity of the Trie ($O(L)$ vs Naive Search)

To categorize merchants accurately in a robust financial application, we use a Prefix Tree (Trie) instead of generic iterative or naive sub-string search.

### Why is this better?
When searching for a merchant string of length $L$ amongst a dictionary of $N$ known merchants (where $K$ is the average merchant name length):
- **Naive Array Search**: Requires $O(N \cdot \min(L, K))$ complexity. It tests our input against everything.
- **Prefix Trie**: Reduces lookup drastically to $O(L)$ time. We trace the string character-by-character from the root of the Trie. Once we mis-match, we exit. We do not re-scan other potential $N$ strings. 
Additionally, Tries efficiently store common prefixes, significantly improving caching performance on constrained hardware (like mobile phones) when scaling to tens of thousands of localized merchants.

## Build and Run
This project requires CMake and a C++17 compatible compiler (e.g., GCC, Clang, MSVC).

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

After building, execute the tool from the `build` directory:
```bash
# On Linux/Mac:
./VortexFinShield

# On Windows (PowerShell):
.\Debug\VortexFinShield.exe
```

The app will read data from `data/sms_inbox.txt` and display the professional Terminal Dashboard.
