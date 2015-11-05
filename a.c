/*

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


#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/in.h>

#include <stdint.h>
#include <net/if.h>
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
//#include <linux/in.h>

#include <event.h>
#include <pthread.h>
#include <string.h>
#include <sys/ioctl.h>




struct rsync_info{
    char *conifg_mac;
    char *local_mac;
    int mac_num;
    char *listen_mac;
    char tlvs;
}rsync_info;

#define BUFFER_MAX 1049

struct packet
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
    char tlvs[];

};

int main(int argc, char **argv[])
{
    int listen_socket;
    listen_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    //报收本机收到的所有二层报文 PF_PACKET or PF_INET
    //接收数据不包括数据链路帧头
    if ( listen_socket < 0 )
    {
        fprintf(stderr ,"Failed to create socket\n");
        exit(-1);
    }
    //
    struct ifreq ifr;
    /*
    struct ifaddrs *ifaddr;
    getifaddrs(&ifap);
    char *ip = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin-addr);
    if (ioctl(listen_socket, SIOCGIFINDEX, &ifr) == -1){
        fprintf(stderr, "Unable to find intreface index : %s\n", strerror(errno));
        exit(-1);
    }*/

    strcpy(ifr.ifr_name, "enp4s0f1");
    ioctl(listen_socket, SIOCGIFINDEX, &ifr);

    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(struct sockaddr_ll));
    sll.sll_family = AF_PACKET;
    //sll.sll_protocol = htons(0x1122);
    sll.sll_protocol = htons(ETH_P_ALL);

    sll.sll_ifindex = ifr.ifr_ifindex;
    //ssl.sll_hatype = ARPHRD_ETHER;  //
    //ssl.sll_pkttype = PACKET_OTHERHOST;  //
    //ssl.sll_sll_halen = sll_hatype; //

    if ( -1 == ( bind(listen_socket, (struct sockaddr *)&sll, sizeof(struct sockaddr_ll)) ) )
    {
        fprintf(stderr ,"bind error\n");
        exit(-1);
    }
    //   then, we can
    //   n = recvfrom(socketlisten, buffer, BUFFER_MAX, 0, NULL, NULL);
    //   sendto(skfd, buf, pks_len, 0, (struct sockaddr*)target, sizeof(struct sockaddr_ll));

    /**********************************/
    struct event_base *base = event_base_new();
    struct event *listen_event;
    listen_event = event_new(base, listen_socket, EV_HEAD|EV_PERSIST, p_read_callback, struct config);
    event_add(listen_event, NULL);

    pthread_t sync_tid;
    pthread_create(&sync_tid, NULL, sync_thread, NULL);

    event_base_dispatch(base);
}


void sync_thread(void *arg){
    struct event_base *base = event_base_new();
    struct timeval timer={.tv_sec = 30, .tv_usec = 0};
    struct event sync_ev;

    int send_socket = socket(PF_INET, SOCK_RAW, 0);     //send
    if ( send_socket < 0 )
    {
        fprintf(stderr ,"Failed to create socket\n");
        exit(-1);
    }
    /*
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "enp4s0f1");
    //char *ip = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin-addr);
    if (ioctl(listen_socket, SIOCGIFINDEX, &ifr) == -1){
        fprintf(stderr, "unable to find interface index : %s\n", strerror(errno));
        exit(-1);
    }

    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(struct sockaddr_ll));
    sll.sll_family = PF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_APLL);
    if(bind(send_socket, &addr, sizeof(addr)) < 0)
    {
        perror("send bind error");
        exit(1);
    }*/

    event_set(&sync_ev, send_socket, EV_PERSIST, p_sync_callback, struct rsync_info *rsync_info);
    event_base_set(base, &sync_ev);
    event_add(&sync_ev, &timer);
    event_base_loop(base, 0);
}

void set_mac(char *setto, char *from){
    int i;
    for( i = 0 ; i < 6; ++i )
    {
        setto[i] = from[i];
    }
}

void read_thread(char buffer[]){
    int i;
    eth = (struct ethhdr*)buffer;

    char *dmac = (char *)malloc(len(char) * 6);
    char *smac = (char *)malloc(len(char) * 6);
    for ( i = 0 ; i < 6; ++i )
    {
        dmac[i] = (unsigned char)buffer[i];
        smac[i] = (unsigned char)buffer[i+6];
    }
    dmac[++i] = '\0'; smac[++i] ='\0';

    char *eth_type = ((unsigned char)buffer[13])*16*16 + (unsigned char)buffer[14];
    if (eth_type == 0x1122 /*&& is_corrent_mac(smac, dmac) */){

        printf("-TO HYR TO HYR!-");

        //to hyr
    }
}

/*
int is_corrent_mac(char *dmac, char *smac)
{
    char mac[7];
    int i;
    int flag = 0;
    if( !( strcmp(rsync_info->conifg_mac, dmac) != 0 && strcmp(rsync_info->local_mac, dmac) != 0 ) )
        return 0;
    for( i = 0 ; i < rsync_info->mac_num ; ++i )
    {
        strncpy(mac, rsync_info->listen_mac + 6 * i, 6);
        if (strcmp(smac, mac) == 0)
        {
            flag = 1;
            break;
        }
    }
    (flag == 1)? return 1: return 0;
}
*/


/******************************/

int p_read_callback(int sock, short event, void *arg){
    char buffer[BUFFER_MAX];
    struct ethhdr *eth;

    n_read = recvfrom(sock, buffer, BUFFER_MAX, 0, NULL, NULL);


    printf("-RECIVED-\n");
    char *eth_type = ((unsigned char)buffer[16])*16*16 + (unsigned char)buffer[17];
    if (eth_type == 0x1122){
        for ( i=0 ; i<n_read ; i++)
            printf("%.2X ",(unsigned char)buffer[i]);
            if(((i+1)%16)==0) printf("\n");
    }
    printf("\n-END-\n");

    pthread_t sync_tid;
    pthread_create(&read_thread, NULL, read_thread, buffer);
}


int p_sync_callback(int send_socket, short event, struct rsync_info *rsync_info){
/*
    struct rsync_info *rsync_info = arg;
    int mac_num = rsync_info->rsync_info;
    char *mac = rsync_info->mac;
    char dest_addr[6];

    for ( ; mac_num > 0 ; --mac_num )
    {
        strncpy(dest_addr, mac, 6);
        csismp_send(send_socket, dest_addr, type, rsync_info->tlvs);
        mac += 6;
    }
    */
    printf("- p_sync_callback start! -\n");
}


/************************************/
int p_reply(char dest_addr[6], int type){

    int send_socket = socket(PF_INET, SOCK_RAW, 0);     //send

    struct ifreq ifr;

    strcpy(ifr.ifr_name, "eth0");
    octl(sockfd, SIOCGIFINDEX, &ifr);


    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(struct sockaddr_ll));
    sll.sll_family = PF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_APLL);
    if(bind(send_socket, &addr, sizeof(addr)) < 0)
    {
        perror("send bind error");
        exit(1);
    }


    csismp_send(send_socket, dest_addr, type, NULL);

    close(send_socket);
    //close send_socket
}


/************************************/
//void ntoh(unsigned char *dbuf, size_t len){
//
//}


char* csismp_construct(
        char source_addr[6],
        char dest_addr[6],
        int c_type,
        int start,
        int end,
        int slice,
        int session,
        const char* s_tlvs)
{
    struct packet *csismp;

    set_mac(csismp.smac, dest_addr);
    set_mac(csismp.dmac, //sendmac);

    csismp.pro_type = htons(0x1122);
    csismp.c_type = type;

    csismp.start = start;
    csismp.end = end;
    csismp.slice = slice;
    csismp.session = session;
    strcpy(cssismp.tlvs, s_tlvs);

    return cssismp;
}


int _csismp_send(int send_socket, const char *buffer){
    sendto(send_socket, buffer, len(buffer), 0, &addr, addr_len);//!
}

int csismp_send(int send_socket, char dest_addr[6], int type, int tlvs){
    char *s_tlvs = tlvs; //!
    int s_len = len(s_tlvs);
    char *buffer;
    int end;
    int slice; // slice
    static int session = 0; // session id
    session += 1;
    if (session > 1000) session = 1;

    if (type == 3 || type == 4)
    {
        //rand int <1000 return randint1000
        buffer = csismp_construct(sourc_addr, dest_addr,
                            type, 1, 1, 1, session, NULL);
        _csismp_send(send_socket, buffer);
    }
    else if (type == 5)
    {
        //ranint
        for ( slice = 0 ; s_len > 0; s_len =-1023 )
        {

            srand((unsigned)time(NULL));
            //rand int > 1000 return randint
            buffer = csismp_construct(source_addr, dest_addr,
                            type, slice? 0:1, (s_len < 1024)? 1:0, slice, session + 1000, s_tlvs);
            _csismp_send(send_socket, buffer);
            s_tlvs -= 1024;
        }
    }
}

