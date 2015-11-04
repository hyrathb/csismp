#include "async.h"
#define BUFFER_MAX 1049

struct control{
    char type; // 1 add 2 del 3 ack 4 rjt 5 sync
    int begin:1;
    int end:1;
    int number:22;
    char id[4];
};

typedef struct csismp{
    char dest_addr[6];
    char src_addr[6];
    char protocol[2]; // 0x1122
    struct control;
    char* Tlvs; // max 1024 bytes
}cssismp;


int p_sync_callback();     // Send all data message
int p_reply_callback();    // ACK or RJT
int p_read_callback();     // Recive all message

//组织整个以太网数据帧
struct sockaddr_ll{
    unsigned short sll_family;     /* 总是 AF_PACKET */
    unsigned short sll_protocol;   /* 物理层的协议 */   //   0x1122
    int            sll_ifindex;    /* 接口号 */         //  0表示处理所有接口
    unsigned short sll_hatype;     /* 报头类型 */       //  ARPHRD_ETHER
    unsigned char  sll_pkttype;    /* 分组类型 */      //   包含分组类型。
    //目前，有效的分组类型有：
    //目标地址是本地主机的分组用的 PACKET_HOST，
    //物理层广播分组用的 PACKET_BROADCAST ，
    //发送到一个物理层多路广播地址的分组用的 PACKET_MULTICAST，
    //在混杂(promiscuous)模式下的设备驱动器发向其他主机的分组用的 PACKET_OTHERHOST，
    //源于本地主机的分组被环回到分组套接口用的 PACKET_OUTGOING。这些类型只对接收到的分组有意义
    unsigned char  sll_halen;      /* 地址长度 */      //指示物理层
    unsigned char  sll_addr[8];    /* 物理层地址 */
};

int async_main(int argc, char **argv[])
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
    //!getifaddrs(&ifap);
    char *ip = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin-addr);
    if (ioctl(listen_socket, SIOCGIFINDEX, &ifr) == -1){
        fprintf(stderr, "Unable to find intreface index : %s\n", strerror(errno));
        exit(-1);
    }

    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(struct sockaddr_ll));
    sll.sll_family = PF_PACKET;
    //sll.sll_protocol = 0x1122;      ////ntohs?
    sll.sll_ifindex = ifr.ifr_ifindex;
    //ssl.sll_hatype = ARPHRD_ETHER;  //
    //ssl.sll_pkttype = PACKET_OTHERHOST;  //
    //ssl.sll_sll_halen = sll_hatype; //

    sll.sll_protocol = htons(ETH_P_APLL);

    if ( -1 == ( bind(listen_socket, (struct sockaddr *)&sll, sizeof(struct sockaddr_ll)) ) )
    {
        fprintf(stderr ,"bind error\n");
        exit(-1);
    }
    //   then,
    //   n = recvfrom(socketlisten, buffer, BUFFER_MAX, 0, NULL, NULL);
    //   sendto(skfd, buf, pks_len, 0, (struct sockaddr*)target, sizeof(struct sockaddr_ll));

    /**********************************/
    struct event *base = event_base_new();
    struct event *listen_event;
    listen_event = event_new(base, listen_socket, EV_HEAD|EV_PERSIST, p_read_callback, NULL);
    event_add(listen_event, NULL);

    pthread_t sync_tid;
    pthread_create(&sync_tid, NULL, sync_thread, NULL);

    event_base_dispatch(base);
}


void sync_thread(void *arg){
    struct event *base = event_base_new();
    struct timeval timer={.tv_sec = 30, .tv_usec = 0};
    struct event sync_ev;

    int send_socket = socket(PF_INET, SOCK_RAW, 0);     //send
    if ( send_socket < 0 )
    {
        fprintf(stderr ,"Failed to create socket\n");
        exit(-1);
    }

    struct ifreq ifr;
    char *ip = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin-addr);
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
    }

    event_set(&sync_ev, send_socket, EV_PERSIST, p_sync_callback, struct rsync_info *rsync_info);
    event_base_set(base, &sync_ev);
    event_add(&sync_ev, &timer);
    event_base_loop(base, 0);
}

void read_thread(char buffer[]){
    //to hyr
}


/******************************/
int p_read_callback(int sock, short event, void *arg){
    char buffer[BUFFER_MAX];
    n_read = recvfrom(sock, buffer, BUFFER_MAX, 0, NULL, NULL);
    /*
    if (n_read < 32){
    }
    printf("-START-\n");
    char *eth_type = ((unsigned char)buffer[16])*16*16 + (unsigned char)buffer[17];
    if (eth_type == 0x1122){
        for ( i=0 ; i<n_read ; i++)
            printf("%.2X ",(unsigned char)buffer[i]);
            if(((i+1)%16)==0) printf("\n");
    }
    printf("\n-END-\n");
    return 0;*/
    pthread_t sync_tid;
    pthread_create(&sync_tid, NULL, sync_thread, buffer);

}


int p_sync_callback(int send_socket, short event, struct rsync_info *rsync_info){
    char buffer[BUFFER_MAX];
    //analysis return a list
    for ()
    {
    csismp_send(send_socket, dest_addr, type);
    }
}


/************************************/
int p_reply(char dest_addr[6], int type){

    int send_socket = socket(PF_INET, SOCK_RAW, 0);     //send

    struct ifreq ifr;
    char *ip = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin-addr);
    // strcpy(ifr.ifr_name, "eth0");
    // 获取到网卡的 index 编号先~
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
    }

    csismp_send(send_socket, dest_addr, type, NULL);

    //close send_socket
}

/************************************/
char* csismp_byte_order(struct* cssismp){
    char buffer[BUFFER_MAX];
    return buffer;
}

char* csismp_construct(char dest_addr[6], int type, int number, int id, const char* s_tlvs)
{

}

int _csismp_send(int send_socket, const char *buffer)
{
    sendto(sockfd, buffer, len, 0, &addr, addr_len);
}

int csismp_send(int send_socket, char dest_addr[6], int type, int tlvs){
    char *s_tlvs = tlvs; //!
    int s_len = len(s_tlvs);
    char *buffer;
    int number; // slice
    int id; // connect id

    if (type == 3 || type == 4)
    {
        //rand int <1000 return randint1000
        buffer = csismp_construct(dest_addr, type, 1, id, NULL);
        _csismp_send(send_socket, buffer);
    }
    else if (type == 5)
    {
        for ( number = 1 ; s_len > 0; s_len =-1023 )
        {
            //rand int > 1000 return randint
            buffer = csismp_construct(dest_addr, type, number, ud ,s_tlvs);
            _csismp_send(send_socket, buffer);
            s_tlvs -= 1024;
        }
    }
}

