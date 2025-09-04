#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <atomic> 
#include <openssl/sha.h>
#include <iomanip>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

#include <chrono>
#include <ctime>

using namespace std; 

// #define SERVER_IP "18.202.148.130"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3336



// TLS
SSL *tls_connect(const string& ip, int port, SSL_CTX* ctx, int &sock_out);
string read_line(SSL *ssl);
void write_line(SSL *ssl, const string& line);

// POW
string solve_pow(const string& authdata, int difficulty);

// SHA
string next_suffix();
string sha_1(const string& input);

// ThreadPool
class ThreadPool{
    public: 
    ThreadPool(size_t numOfThreads); 
    ~ThreadPool(); 

    void enqueue(function<void()> task); 

    private:
    vector<thread> workers {}; 
    queue<function<void()>> tasks{}; 
    mutex queueMutex; 
    condition_variable condition; 
    atomic<bool> stop;  
}; 
