#include "thread.h"
#include "utils.h"

typedef struct mac{
    char* mac_address;
    struct mac* next;
}MAC;

typedef struct student_info{
    char* faculty;
    char* id;
    char* name;
    struct student_info* next;
}STUDENT_INFO;

MAC config_mac;


int _csismp_send(int send_socket, const char *buffer, int len);

int main(int argc, char **argv[])
{
    int listen_socket;
    listen_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if ( listen_socket < 0 )
    {
        fprintf(stderr ,"Failed to create socket\n");
        exit(-1);
    }

    struct ifreq ifr;
    /*
    struct ifaddrs *ifaddr;
    getifaddrs(&ifap);
    char *ip = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin-addr);
    if (ioctl(listen_socket, SIOCGIFINDEX, &ifr) == -1){
        fprintf(stderr, "Unable to find intreface index : %s\n", strerror(errno));
        exit(-1);
    }
    char *a = get_interface_name();
    printf("%s", a);*/


    //strcpy(ifr.ifr_name, get_interface_name());
    strcpy(ifr.ifr_name, "enp4s0f1");

    if (ioctl(listen_socket, SIOCGIFINDEX, &ifr) == -1) {
        fprintf(stderr ,"ioctl error,no such interface\n");
        close(listen_socket);
        exit(-1);
    }

    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(struct sockaddr_ll));
    sll.sll_family = AF_PACKET;
    //sll.sll_protocol = htons(0x1122);
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = ifr.ifr_ifindex;
    //ssl.sll_hatype = ARPHRD_ETHER;
    //ssl.sll_pkttype = PACKET_OTHERHOST;
    //ssl.sll_sll_halen = sll_hatype;

    if ( -1 == ( bind(listen_socket, (struct sockaddr *)&sll, sizeof(struct sockaddr_ll)) ) )
    {
        fprintf(stderr ,"bind error\n");
        exit(-1);
    }

/*
    int a, i;
    char buffer[BUFFER_MAX];
    while(1)
    {
        printf("recvfrom\n");
        a = recvfrom(listen_socket, buffer, BUFFER_MAX, 0, NULL, NULL);
        for ( i=0 ; i < a ; i++)
            printf("%.2X ",(unsigned char)buffer[i]);
            if(((i+1)%16)==0) printf("\n");
    }
    //test
*//*
    char dest_addr[6];
    dest_addr[0] = dest_addr[1] = dest_addr[2] = dest_addr[3] = dest_addr[4] = dest_addr[5] = 0xFF;
    rsync_info.local_mac = malloc(sizeof( char) * 6);
    rsync_info.local_mac[0] = rsync_info.local_mac[1] =rsync_info.local_mac[2] = rsync_info.local_mac[3] = rsync_info.local_mac[4] = rsync_info.local_mac[5] = 0xFF;
    printf("start reply()\n");
    p_reply(dest_addr, 3);
    printf("end reply()\n");
//    //test*/

    struct event_base *base = event_base_new();
    struct event *listen_event;
    listen_event = event_new(base, listen_socket, EV_READ|EV_PERSIST, p_read_callback, NULL);

    event_add(listen_event, NULL);

    pthread_t sync_tid;
    pthread_create(&sync_tid, NULL, sync_thread, NULL);

    fprintf(stdout, "- START LISTENING -\n");
    event_base_dispatch(base);
}

void *sync_thread(void *arg){
    struct event_base *base = event_base_new();
    struct timeval timer={.tv_sec = 30, .tv_usec = 0};
    struct event sync_ev;

    int send_socket = socket(PF_INET, SOCK_RAW, htons(ETH_P_ALL));
    if ( send_socket < 0 )
    {
        fprintf(stderr ,"Failed to create socket\n");
        exit(-1);
    }

    event_set(&sync_ev, send_socket, EV_PERSIST, p_sync_callback, &rsync_info);//!
    event_base_set(base, &sync_ev);
    event_add(&sync_ev, &timer);

    event_base_loop(base, 0);
    return 0;
}


/*******************************/
void read_thread(void *arg){
    struct read_callback_buffer *buffer_arg = (struct read_callback_buffer *)arg;

    uint64_t smac = transform_mac_to_int64((char *) (buffer_arg->buffer + 6) );
    uint16_t eth_type = ((uint16_t)buffer_arg->buffer[12]) << 8 | (uint16_t)buffer_arg->buffer[13];

    if (eth_type == 0x1122 && smac == transform_mac_to_int64(config_mac.mac_address) ){
/*      int i;
        for ( i=0 ; i<len ; i++){
            printf("%.2X ",(unsigned char)buffer[i]);
            if(((i+1)%16)==0) printf("\n");
*/
        //TO HYR
        printf("- REALLY TO HYR TO HYR!-");
    }
    printf("-TO HYR TO HYR!-");
}



/******************************/

void p_read_callback(int sock, short event, void *arg){

    printf("-RECIVED PACKET-\n");

    char buffer[BUFFER_MAX];
    int len;

    len = recvfrom(sock, buffer, BUFFER_MAX, 0, NULL, NULL);

    printf("length : %d\n", len);
    if(len < 23)
        return;

    struct read_callback_buffer *buffer_arg = malloc(sizeof(struct read_callback_buffer));
    strncpy(buffer_arg->buffer, buffer, len);
    buffer_arg->len = len;

    pthread_t sync_tid;
    pthread_create(&sync_tid, NULL, (void * (*)(void *))&read_thread, buffer_arg);
}


int generate_tlvs(void *);

//!
void p_sync_callback(int send_socket, short event, void *arg){
    fprintf(stdout, "- A SYNC START -\n");

    //MAC *mac = &;
    char buffer[BUFFER_MAX * 8];
    int len = generate_tlvs(&buffer);

    MAC *dmac = config_mac.next;

    for ( ; dmac != NULL ; dmac = dmac->next)
    {
        csismp_send(send_socket, dmac->mac_address, 5, buffer, len);//!
    }
//!
    fprintf(stdout, "- A SYNC END -\n");
}

void p_reply(char dest_addr[6], int type){
    int reply_socket = socket(PF_INET, SOCK_RAW, htons(ETH_P_ALL));
    if ( reply_socket < 0 )
    {
        fprintf(stderr ,"Failed to create socket\n");
        exit(-1);
    }

    csismp_send(reply_socket, dest_addr, type, NULL, 0);//!
    close(reply_socket);
}


/******************************************/
char* csismp_construct(
        char source_addr[6],
        char dest_addr[6],
        int c_type,
        int start,
        int end,
        int slice,
        int session,
        const char* s_tlvs,
        int len)
{
    struct packet *csismp = malloc(sizeof(struct packet));

    //set_mac(csismp->smac, source_addr);
    //set_mac(csismp->dmac, dest_addr);

    csismp->pro_type = htons(0x1122);
    csismp->c_type = c_type;

    csismp->start = start;
    csismp->end = end;
    csismp->slice = slice;
    csismp->session = session;
    strncpy(csismp->tlvs, s_tlvs, len);

    return (char*)csismp;
}

int _csismp_send(int send_socket, const char *buffer, int len){
    int raw_send;

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy (ifr.ifr_name, "enp4s0f1", sizeof(ifr.ifr_name) - 1);
    ifr.ifr_name[sizeof(ifr.ifr_name)-1] = '\0';

    if (ioctl(send_socket, SIOCGIFINDEX, &ifr) == -1) {
        fprintf(stderr ,"ioctl error,no such interface\n");
        close(send_socket);
        exit(-1);
    }

    ioctl(send_socket, SIOCGIFFLAGS, &ifr);
    if ( (ifr.ifr_flags & 0x1) == 0) {
        fprintf(stderr ,"ioctl error,the interface is down\n");
        close(send_socket);
    }
    ioctl(send_socket, SIOCGIFINDEX, &ifr);

    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(struct sockaddr_ll));
    sll.sll_ifindex = ifr.ifr_ifindex;
/*
    fprintf(stdout, "sned lens:%d\n", len);
    int i;
    for ( i=0 ; i<len ; i++){
        fprintf(stdout, "%.2X ",(unsigned char)buffer[i]);
        if(((i+1)%16)==0) printf("\n");
    }
    fprintf(stdout,"print end\n");
*/
    if (raw_send = sendto(send_socket, buffer, len, 0,
                    (struct sockaddr *) &sll ,sizeof(struct sockaddr_ll)) ==-1)
    {
        fprintf(stderr ,"_csismp_send error\n");
        return -1;
    }
    return 0;
}

int csismp_send(int send_socket, char dest_addr[6], int type, char* tlvs, int s_len){
    char *s_tlvs = tlvs;
    char *buffer = malloc(sizeof(char) * BUFFER_MAX);
    int end;
    int slice; // slice
    static int session = 0; // session id
    session += 1;
    if (session > 1000) session = 1;

    if (type == 3 || type == 4)
    {
        //rand int <1000 return randint1000
        buffer = csismp_construct(config_mac.mac_address, dest_addr,
                            type, 1, 1, 1, session, "\0",1); //!
        _csismp_send(send_socket, buffer, 23);
    }
    else if (type == 5)
    {
        //ranint
        for ( slice = 0 ; s_len > 0; s_len =-1023 )
        {

            srand((unsigned)time(NULL));
            //rand int > 1000 return randint
            buffer = csismp_construct(config_mac.mac_address, dest_addr,
                            type, slice? 0:1, (s_len < 1024)? 1:0, slice, session + 1000, s_tlvs, s_len % 1024); //!
            _csismp_send(send_socket, buffer, 22 + s_len % 1024);
            s_tlvs -= 1024;
        }
    }
}
