#include "common.h"

#define SUFFIX_LENGTH 8

atomic<uint64_t> counter = 0; 


string next_suffix() {    
    uint64_t value = counter.fetch_add(1, memory_order_relaxed);

    const int base = 94;              // 94 printable ASCII characters
    string s(SUFFIX_LENGTH, '\0');

    for (int i = 0; i < SUFFIX_LENGTH; i++) {
        int idx = value % base;
        s[i] = static_cast<char>(33 + idx);  // Map 0 → '!', 93 → '~' 
        value /= base;
    }
    return s;
}

string sha_1(const string& input) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    stringstream ss;
    ss << hex << nouppercase;  // force lowercase
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << setw(2) << setfill('0') << (unsigned int)hash[i];
    }
    return ss.str();
}

// int main(){
//     string s = "hello world"; 
//     cout << sha_1(s); 
// }