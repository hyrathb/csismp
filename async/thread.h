#ifndef SYNC_H
#define SYNC_H

#include <net/if.h>
#include <netpacket/packet.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <ifaddrs.h>
//#include <linux/in.h>

#include <event.h>
#include <event2/event.h>
#include <pthread.h>
#include <string.h>
#include <sys/ioctl.h>

#include <time.h>

#define BUFFER_MAX 1049


int main_thread();

struct read_callback_buffer{
    char buffer[BUFFER_MAX];
    int len;
};

void p_read_callback(int sock, short event, void *arg);
void p_sync_callback(int send_socket, short event, void *arg);
void p_reply(unsigned char dest_addr[6], int type);
void *sync_thread(void *arg);

#endif
