#ifndef P2P_CHAT_H
#define P2P_CHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


void start_listening();
void connect_to_peer(char* ip);


void* receive_thread(void* arg);
void* send_thread(void* arg);

#endif
