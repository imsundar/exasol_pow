#include "common.h"
#define THREAD_COUNT 16 

string solve_pow(const string& authdata, int difficulty){
    ThreadPool pool(THREAD_COUNT);
    atomic<bool> found(false); 
    atomic<uint64_t> hashCount(0);
    
    string result; 

    function<void()> task = [&](){
        while(!found){
            string suffix = next_suffix();

            string hash = sha_1(authdata + suffix);
            hashCount.fetch_add(1, memory_order_relaxed); 
            bool ok = true; 
            // cout << "new suffix : " << suffix << "new hash : " << hash << endl; 

            for(int i=0; i<difficulty; i++)
                if(hash[i]!= '0') {ok = false; break;}
            if(ok){
                result = suffix; 
                found = true; 
                cout << "total hashes generated : " << hashCount << endl;    
                break; 
            }
        }
    };

    for(int i=0; i<THREAD_COUNT; i++) 
        pool.enqueue(task); 

    // hold the func untill found: 
    // probably within 2 hours :) 

    while(!found) this_thread::sleep_for(chrono::milliseconds(10));

    return result; 
}