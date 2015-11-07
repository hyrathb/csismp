#include "thread.h"
#include "utils.h"
#include "handle_data.h"
#include "parser.h"
#include "cssismp_send.h"

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
MAC listen_mac = { "\xff\xff\xff\xff\xff\xff", NULL };
MAC config_mac = { "\xff\xff\xff\xff\xff\xff", &listen_mac };

#define INTERFACE_NAME "enp4s0f1"

int _csismp_send(int send_socket, const char *buffer, int len);

int main_thread()
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
    pthread_t sync_tid, delay_tid;
    pthread_create(&sync_tid, NULL, sync_thread, NULL);
    pthread_create(&delay_tid, NULL, delay_thread, NULL);

    fprintf(stdout, "- START LISTENING -\n");
    event_base_dispatch(base);

    return 0;
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

    event_set(&sync_ev, send_socket, EV_PERSIST, p_sync_callback, NULL);
    event_base_set(base, &sync_ev);
    event_add(&sync_ev, &timer);

    event_base_loop(base, 0);
    return 0;
}

void *delay_thread(void *arg){
    struct event_base *base = event_base_new();
    struct timeval delay_timer={.tv_sec = 5, .tv_usec = 0};
    struct event delay_ev;

    event_set(&delay_ev, 0, EV_PERSIST, NULL, NULL);
    event_base_set(base, &delay_ev);
    event_add(&delay_ev, &delay_timer);

    return 0;
}

/*
    recv data from p_read_callback()
    analyze its header and send package to parser();
 */
void read_thread(void *arg){
    struct read_callback_buffer *buffer_arg = (struct read_callback_buffer *)arg;

    uint64_t dmac = transform_mac_to_int64((unsigned char *) (buffer_arg->buffer) );
    uint16_t eth_type = ((uint16_t)buffer_arg->buffer[12]) << 8 | (uint16_t)buffer_arg->buffer[13];
    pthread_rwlock_rdlock(&rwlock);    // Lock

/*
    int i;
    for ( i=0 ; i<buffer_arg->len ; i++){
        fprintf(stdout, "%.2X ",(unsigned char)buffer_arg->buffer[i]);
        if(((i+1)%16)==0) fprintf(stdout, "\n");
    }

*/
    if (eth_type == 0x1122 && dmac == transform_mac_to_int64(config_mac.mac_address)){

    fprintf(stdout, "gegegegegegegege a packet 0x1122\n");
/*
    pthread_rwlock_rdlock(&rwlock);    // Lock
    pthread_rwlock_unlock(&rwlock);    // Release Lock
*/
        //TO HYR
        fprintf(stdout, "- REALLY TO HYR TO HYR!-\n");
        parser(buffer_arg->buffer, buffer_arg->len);

        pthread_rwlock_unlock(&rwlock);    // Release Lock
    }
    free(arg);
}

/***Registering Callbacks Funcs******************************************/
/*
    Registering Callbacks Func:
        recvform listen_socket and start new thread to handle data
    recv data to read_thread();
 */
void p_read_callback(int sock, short event, void *arg){
//  printf("-RECIVED PACKET-\n");

    char buffer[BUFFER_MAX];
    int len;

    len = recvfrom(sock, buffer, BUFFER_MAX, 0, NULL, NULL);

//  printf("length : %d\n", len);
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

    free(buffer);
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

