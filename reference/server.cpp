// server.cpp
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

#define PORT 3336

int main() {
    // 1. Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    // 2. Create SSL context
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        cerr << "Cannot create SSL context\n";
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // Load cert and key (self-signed)
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
        cerr << "Cannot load cert or key\n";
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // 3. Create TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }
    if (listen(sock, 5) < 0) { perror("listen"); return 1; }

    cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        int client = accept(sock, (sockaddr*)&client_addr, &len);
        if (client < 0) { perror("accept"); continue; }

        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            close(client);
            continue;
        }

        cout << "Client connected\n";

        // 4. Send HELO and POW commands
        SSL_write(ssl, "HELO\n", 5);
        SSL_write(ssl, "POW howareyou 9\n", 16);  // difficulty = 4 zeros

        // 5. Read client's suffix
        char buf[256] = {0};
        int n = SSL_read(ssl, buf, sizeof(buf)-1);
        if (n > 0) {
            buf[n] = '\0';
            cout << "Received suffix: " << buf;
        }

        // 6. Send END command
        SSL_write(ssl, "END\n", 4);

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
        cout << "Connection closed\n";
    }

    close(sock);
    SSL_CTX_free(ctx);
    return 0;
}