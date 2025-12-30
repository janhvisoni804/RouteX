#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <HOSTNAME> <PORT>\n", argv[0]);
        return 1;
    }

    char *hostname = argv[1];
    int port = atoi(argv[2]);

    printf("--- RouteX Client (Remote DNS Mode) ---\n");
    printf("Target: %s:%d\n", hostname, port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    
    server.sin_addr.s_addr = inet_addr("0.0.0.1"); 

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed (Did you inject tord.so?)");
        return 1;
    }
    
    char req[1024];
    snprintf(req, sizeof(req), 
        "GET / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36\r\n"
        "Connection: close\r\n"
        "\r\n", hostname);

    write(sock, req, strlen(req));

    char buffer[4096];
    int bytes;
    printf("\n--- Encrypted Response ---\n");
    while ((bytes = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }
    printf("\n");

    close(sock);
    return 0;
}