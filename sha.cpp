#include "common.h"

#define SUFFIX_LENGTH 8

atomic<uint64_t> counter = 0; 


string next_suffix(){
    string s(SUFFIX_LENGTH, '\0'); 
    uint64_t value = counter.fetch_add(1, memory_order_relaxed);     

    for(int i=0; i<SUFFIX_LENGTH; i++){
        unsigned char c = static_cast<unsigned char>((value >> (i * 8)) & 0xFF);
        // c = 33 + (c % (125 - 33 + 1)); // to void /n /r /t 
        c = 33 + (c % 94); // to void /n /r /t 
        s[i] = static_cast<char>(c);
    }

    return s; 
}

string sha_1(const string& input){
    unsigned char hash[SHA_DIGEST_LENGTH]; 
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash); 

    string result; 
    result.resize(SHA_DIGEST_LENGTH*2, '\0'); 
    static const char* hexLookup = "0123456789abcdef"; 

    for(int i=0; i<SHA_DIGEST_LENGTH; i++){
        unsigned char byte = hash[i];
        result[2*i] = hexLookup[((byte >> 4) & 0xF )]; //rotate and get the high nibble
        result[2*i + 1] = hexLookup[byte & 0xF ]; //get the low nibble
    }

    return result; 
}



// int main(){
//     string s = "hello world"; 
//     cout << sha_1(s); 
// }