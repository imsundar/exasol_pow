#include "common.h"

SSL *tls_connect(const string& ip, int port, SSL_CTX* ctx, int &sock_out){
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server_addr {}; 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(SERVER_PORT);  
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr); 

    cout << "connecting to server..." << endl;; 
    if(connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        cerr << "Error : Connect to server failed "; 
        return nullptr; 
    } 

    // add tls to the socket 
    SSL *ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sock); 

    if(SSL_connect(ssl) <= 0){
        ERR_print_errors_fp(stderr); 
        return nullptr; 
    }

    sock_out = sock; 
    return ssl;     
}

string read_line(SSL *ssl){
    string line; 
    char c; 
    while(SSL_read(ssl, &c, 1) > 0){
        if(c == '\n') break; 
        line.push_back(c); 
    }
    return line; 
}

void write_line(SSL *ssl, const string& line){
    SSL_write(ssl, line.c_str(), line.size());
}