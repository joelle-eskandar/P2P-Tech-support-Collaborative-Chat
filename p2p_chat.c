#include "p2p_chat.h"
#include <string.h>
#include <unistd.h>
#include <time.h>


// Receive Thread


   void* receive_thread(void* arg) {
    int sock = *(int*)arg;
    char buffer[1024];

    while (1) {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0) {
            printf(" Connection closed.\n");
            break;
        }

        buffer[bytes] = '\0';

        printf("\n📩 Incoming message: %s\n", buffer);

        FILE *log = fopen("chat_log.txt", "a");
        if (log != NULL) {
            fprintf(log, "Received: %s", buffer);
            fclose(log);
        }
    }

    return NULL;
}


// Send Thread

void* send_thread(void* arg) {
    int sock = *(int*)arg;
    char msg[1024];
    char username[50];

  printf("Enter technician name: ");
  fgets(username, sizeof(username), stdin);
  username[strcspn(username, "\n")] = '\0';

    while (1) {
        printf("Type a message (or use /ticket, /status, /solution, /exit): ");
        fgets(msg, sizeof(msg), stdin);

        msg[strcspn(msg, "\n")] = '\0';

        if (strcmp(msg, "/exit") == 0) {
            printf("Closing chat...\n");
            close(sock);
            break;
        }

        if (strncmp(msg, "/ticket", 7) == 0) {
            printf("Technical support ticket created.\n");
        }
        else if (strncmp(msg, "/status", 7) == 0) {
            printf("Ticket status updated.\n");
        }
        else if (strncmp(msg, "/solution", 9) == 0) {
            printf("Solution has been recorded.\n");
        }

        char full_msg[1200];
        snprintf(full_msg, sizeof(full_msg), "%s: %s", username, msg);
        send(sock, full_msg, strlen(full_msg), 0);


     FILE *log = fopen("chat_log.txt", "a");
     if (log != NULL) {
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strcspn(time_str, "\n")] = '\0';

       fprintf(log, "[%s] Sent: %s: %s\n", time_str, username, msg);
       fclose(log);
       }
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
