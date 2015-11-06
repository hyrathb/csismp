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


#define BUFFER_MAX 1049



struct rsync_info{
    char *conifg_mac;
    char *local_mac;
    int mac_num;
    char *listen_mac;
    char tlvs;
    int len;
}rsync_info;


typedef struct packet
{
    char smac[6];
    char dmac[6];
    short pro_type;
    struct
    {
        char c_type;
        int start:1;
        int end:1;
        int slice:22;
        int session;
    };
    char tlvs[BUFFER_MAX];
} packet;

struct read_callback_buffer{
    char buffer[BUFFER_MAX];
    int len;
};


void p_read_callback(int sock, short event, void *arg);
void p_sync_callback(int send_socket, short event, void *arg);
void p_reply(char dest_addr[6], int type);
void *sync_thread(void *arg);
int csismp_send(int send_socket, char dest_addr[6], int type, char* tlvs, int s_len);
char *get_interface_name();
char *format_mac(char *mac);
uint64_t transform_mac_to_int64(char *mac);


#endif
