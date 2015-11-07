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

struct rsync_info{
    char *conifg_mac;
    char *local_mac;
    int mac_num;
    char *listen_mac;
    char tlvs;
    int len;
}rsync_info;


struct read_callback_buffer{
    char buffer[BUFFER_MAX];
    int len;
};

int generate_tlvs(char *buffer );
//!
void print_time();

void p_read_callback(int sock, short event, void *arg);
void p_sync_callback(int send_socket, short event, void *arg);
void p_reply(unsigned char dest_addr[6], int type);
void *sync_thread(void *arg);
int csismp_send(int send_socket, unsigned char dest_addr[6], int type, char* tlvs, int s_len);

#endif