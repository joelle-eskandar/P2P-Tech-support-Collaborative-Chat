
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "p2p_chat.h"

int main() {
    int choice;
    char ip[50];

    printf("=== P2P Tech-Support Collaborative Chat ===\n");
    printf("1. Start Listening for a Peer\n");
    printf("2. Connect to a Peer\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        start_listening();
    } 
    else if (choice == 2) {
        printf("Enter peer IP address: ");
        scanf("%s", ip);
        getchar();
        connect_to_peer(ip);
    } 
    else {
        printf("Invalid choice.\n");
    }

    return 0;
}




