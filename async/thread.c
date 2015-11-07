#include "thread.h"
#include "../utils.h"
#include "../handle_data/handle_data.h"
#include "../parser/parser.h"


pthread_rwlock_t rwlock; // Init Posix Read-write lock
/*
    pthread_rwlock_rdlock(&rwlock);    // Lock
    pthread_rwlock_unlock(&rwlock);    // Release Lock
*/

// wait for zjd



/*

typedef struct mac{
    char* mac_address;
    struct mac* next;
}MAC;

*/
MAC listen_mac = { "\xcc\xcc\xcc\xcc\xcc\xcc", NULL };
MAC config_mac = { "\xaa\xbb\xcc\xdd\xdd\xde", &listen_mac };

#define INTERFACE_NAME "lo"

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

    /*bind and set listen_sock*/
    struct ifreq ifr;

#ifdef INTERFACE_NAME                                       //set interface name
    strcpy(ifr.ifr_name, INTERFACE_NAME);
#else
    strcpy(ifr.ifr_name, get_interface_name());             //get the first interface name whit lexicographical order
#endif

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

    /*start event loop*/
    struct event_base *base = event_base_new();
    struct event *listen_event;
    listen_event = event_new(base, listen_socket, EV_READ|EV_PERSIST, p_read_callback, NULL);

    event_add(listen_event, NULL);

    pthread_t sync_tid;
    pthread_create(&sync_tid, NULL, sync_thread, NULL);

    fprintf(stdout, "- START LISTENING -\n");
    event_base_dispatch(base);
}

/***thread Funcs******************************************/
/*
    new thread to set a timer for synchronization event every 30 second.
    call p_sync_callback()
 */
void *sync_thread(void *arg){
    fprintf(stdout, "- sync_thread -\n");
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

/*
    recv data from p_read_callback()
    analyze its header and send package to parser();
 */
void read_thread(void *arg){

    struct read_callback_buffer *buffer_arg = (struct read_callback_buffer *)arg;

    uint64_t smac = transform_mac_to_int64((char *) (buffer_arg->buffer + 6) );
    uint16_t eth_type = ((uint16_t)buffer_arg->buffer[12]) << 8 | (uint16_t)buffer_arg->buffer[13];

    int i;
    /*
    for ( i=0 ; i<buffer_arg->len ; i++){
        printf("%.2X ",(unsigned char)buffer_arg->buffer[i]);
        if(((i+1)%16)==0) printf("\n");
    }
*/
    if (eth_type == 0x1122 /*&& smac == transform_mac_to_int64(config_mac.mac_address)*/ ){



    printf("gegegegegegegege a packet 0x1122\n");
/*
    pthread_rwlock_rdlock(&rwlock);    // Lock
    pthread_rwlock_unlock(&rwlock);    // Release Lock
*/


        //TO HYR
        printf("- REALLY TO HYR TO HYR!-");
        parser(buffer_arg->buffer, buffer_arg->len);
    }
}

/***Registering Callbacks Funcs******************************************/
/*
    Registering Callbacks Func:
        recvform listen_socket and start new thread to handle data
    recv data to read_thread();
 */
void p_read_callback(int sock, short event, void *arg){
    printf("-RECIVED PACKET-\n");

    char buffer[BUFFER_MAX];
    int len;

    len = recvfrom(sock, buffer, BUFFER_MAX, 0, NULL, NULL);

    printf("length : %d\n", len);
    if(len < 23)
        return;


    struct read_callback_buffer *buffer_arg = malloc( sizeof (struct read_callback_buffer) );
    memcpy(buffer_arg->buffer, buffer, len);
    buffer_arg->len = len;

    pthread_t sync_tid;
    pthread_create(&sync_tid, NULL, (void * (*)(void *))&read_thread, buffer_arg);
}

/*
    Registering Callbacks Func:
        every 30 seconds called to send sync message
 */
void p_sync_callback(int send_socket, short event, void *arg){
    print_time();
    fprintf(stdout, "- START SYNC START -\n");

    //MAC *mac = &;
    char *buffer;
    int len = generate_tlvs(&buffer);

    MAC *dmac = config_mac.next;

    for ( ; dmac != NULL ; dmac = dmac->next)
    {
        csismp_send(send_socket, dmac->mac_address, 5, buffer, len);//!
    }
//!
}

/*
    Public Called Func:
        send reply message
*/
void p_reply(unsigned char dest_addr[6], int type){
    int reply_socket = socket(PF_INET, SOCK_RAW, htons(ETH_P_ALL));
    if ( reply_socket < 0 )
    {
        fprintf(stderr ,"Failed to create socket\n");
        exit(-1);
    }

    csismp_send(reply_socket, dest_addr, type, NULL, 0);//!
    close(reply_socket);
}

/***other func******************************************/
/*
    print current time
*/
void print_time(){
    time_t timep;
    time(&timep);
    fprintf(stdout, "%s", ctime(&timep));
}




