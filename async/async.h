#ifndef SYNC_H
#define SYNC_H



#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <event.h>
#include <pthread.h>
#include <pcap.h>

struct rsync_info{
    char *conifg_mac;
    char *local_mac;
    int mac_num;
    char *listen_mac;
    char tlvs;
}rsync_info;




#endif
