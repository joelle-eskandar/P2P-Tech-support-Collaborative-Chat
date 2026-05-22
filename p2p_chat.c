#include "p2p_chat.h"
#include <string.h>
#include <unistd.h>

// Receive Thread

void* receive_thread(void* arg) {
    int sock = *(int*)arg;
    char buffer[1024];

    while (1) {
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            printf(" Connection closed.\n");
            break;
        }

        buffer[bytes] = '\0';
        printf("\n Incoming message: %s\n", buffer);
    }

    return NULL;
}


// Send Thread

void* send_thread(void* arg) {
    int sock = *(int*)arg;
    char msg[1024];

    while (1) {
        printf("Type a message: ");
        fgets(msg, sizeof(msg), stdin);

        send(sock, msg, strlen(msg), 0);
    }

    return NULL;
}

// Listening Mode (Server)

void start_listening() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error creating socket.\n");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Bind failed.\n");
        return;
    }

    listen(server_socket, 5);
    printf(" Listening on port %d...\n", PORT);

    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);

    if (client_socket < 0) {
        printf("Accept failed.\n");
        return;
    }

    printf(" Peer connected.\n");

    pthread_t t_recv, t_send;
    pthread_create(&t_recv, NULL, receive_thread, &client_socket);
    pthread_create(&t_send, NULL, send_thread, &client_socket);

    pthread_join(t_recv, NULL);
    pthread_join(t_send, NULL);

    close(client_socket);
    close(server_socket);
}

// Connect Mode (Client)

void connect_to_peer(char* ip_address) {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error creating socket.\n");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to connect to peer.\n");
        return;
    }

    printf(" Connected successfully.\n");

    pthread_t t_recv, t_send;
    pthread_create(&t_recv, NULL, receive_thread, &client_socket);
    pthread_create(&t_send, NULL, send_thread, &client_socket);

    pthread_join(t_recv, NULL);
    pthread_join(t_send, NULL);

    close(client_socket);
}
