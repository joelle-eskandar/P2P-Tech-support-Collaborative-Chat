#include "p2p_chat.h"
#include <string.h>
#include <unistd.h>

void start_listening() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;

    // إنشاء socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("خطأ في إنشاء socket\n");
        return;
    }

    // إعداد العنوان
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // ربط socket بالمنفذ
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("خطأ في عملية bind\n");
        return;
    }

    // الاستماع
    listen(server_socket, 5);
    printf(" يستمع الآن على المنفذ %d...\n", PORT);

    addr_size = sizeof(client_addr);

    // قبول الاتصال
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
    if (client_socket < 0) {
        printf(" خطأ في accept\n");
        return;
    }

    // استقبال الرسالة
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("رسالة واردة: %s\n", buffer);

    close(client_socket);
    close(server_socket);
}

void connect_to_peer(char* ip_address) {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024];

    // إنشاء socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("خطأ في إنشاء socket\n");
        return;
    }

    // إعداد عنوان الزميل
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    // الاتصال
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("فشل الاتصال بالزميل\n");
        return;
    }

    // أخذ الرسالة من المستخدم
    printf(" أدخل الرسالة: ");
    scanf(" %[^\n]", message);

    // إرسال الرسالة
    send(client_socket, message, strlen(message), 0);
    printf("تم إرسال الرسالة.\n");

    close(client_socket);
}
