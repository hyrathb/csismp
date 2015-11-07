#include "thread.h"
#include "utils.h"


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

    csismp->pro_type = 0x1122;
    csismp->c_type = c_type;

    csismp->start = start;
    csismp->end = end;
    csismp->slice = slice;
    csismp->session = session;

    strncpy(csismp->tlvs, s_tlvs, len);
    hton(csismp, 22 + len);

    return (char*)csismp;
}

/*

 */
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
