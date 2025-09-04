#include "common.h"


void handle_server(SSL* ssl) {
    string authdata;
    while(true) {
        string line = read_line(ssl);
        if(line.empty()) continue;

        // Split line by space
        istringstream iss(line);
        vector<string> parts;
        string temp;
        while(iss >> temp) parts.push_back(temp);
        if(parts.empty()) continue;

        string cmd = parts[0];

        if(cmd == "HELO") {
            write_line(ssl, "EHLO\n");
        }
        else if(cmd == "ERROR") {
            cerr << "Server ERROR: " << line << endl;
            break;
        }
        else if(cmd == "POW") {
            cout << "pow command received. finding suffix.. " << endl;
             
            authdata = parts[1];
            cout << "received authdata : " << authdata << endl;
            auto now1 = chrono::system_clock::now();
            time_t now_c1 = chrono::system_clock::to_time_t(now1);

            cout << "Program started at: " << std::ctime(&now_c1) << endl;
            int difficulty = stoi(parts[2]);
            string suffix = solve_pow(authdata, difficulty);
            cout << "suffix found : " << suffix << endl;    
            
            auto now = chrono::system_clock::now();
            time_t now_c = chrono::system_clock::to_time_t(now);

            cout << "Program finished at: " << std::ctime(&now_c) << endl;

            write_line(ssl, suffix + "\n");
        }
        else if(cmd == "END") {
            write_line(ssl, "OK\n");
            cout << "reached the END !" << endl; 
            break;
        }
        else if(cmd == "NAME") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " +"Sundar\n");
        }
        else if(cmd == "MAILNUM") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "1\n");
        }
        else if(cmd == "MAIL1") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "sundar.eee6593@gmail.com\n");
        }
        else if(cmd == "MAIL2") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "sundar.eee6593@gmail.com\n");
        }
        else if(cmd == "SKYPE") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "sundar.eee6593@gmail.com\n");
        }
        else if(cmd == "BIRTHDATE") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "06.05.1993\n");
        }
        else if(cmd == "COUNTRY") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "India\n");
        }
        else if(cmd == "ADDRNUM") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "2\n");
        }
        else if(cmd == "ADDRLINE1") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "Chennai\n");
        }
        else if(cmd == "ADDRLINE2") {
            write_line(ssl, sha_1(authdata+parts[1]) + " " + "Tamilnadu\n");
        }
    }
}

int main(){
    // 1. Init TLS
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if(!ctx){ ERR_print_errors_fp(stderr); return 1; }

    int sock;
    SSL* ssl = tls_connect(SERVER_IP, SERVER_PORT, ctx, sock);
    if(!ssl) return 1;

    // 2. Handle server
    handle_server(ssl);    

    // 3. Cleanup
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);

    return 0;
}