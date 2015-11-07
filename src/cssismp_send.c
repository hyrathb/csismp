#include "thread.h"
#include "utils.h"
#include "handle_data.h"
#include "parser.h"

extern MAC config_mac;

void csismp_construct(
        unsigned char *buffer,
        unsigned char source_addr[6],
        unsigned char dest_addr[6],
        int c_type,
        int start,
        int end,
        int slice,
        int session,
        const char* s_tlvs,
        int len)
{
    struct packet *csismp = (void *)buffer;

    memcpy(csismp->smac, source_addr, 6);
    memcpy(csismp->dmac, dest_addr, 6);
    csismp->pro_type = 0x1122;
    csismp->c_type = c_type;

    csismp->start = start;
    csismp->end = end;
    csismp->slice = slice;
    csismp->session = session;

    strncpy(csismp->tlvs, s_tlvs, len);
    hton((void *)csismp, 22 + len);

}

/*

 */
int _csismp_send(int send_socket, const unsigned char *buffer, int len){
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
    if ((raw_send = sendto(send_socket, buffer, len, 0, (struct sockaddr *) &sll ,sizeof(struct sockaddr_ll))) == -1 )
    {
        fprintf(stderr ,"_csismp_send error\n");
        return -1;
    }
    return 0;
}


/*
typedef struct student_info{
    char* faculty;
    char* id;
    char* name;
    struct student_info* next;
}STUDENT_INFO;*/
STUDENT_INFO student_info;

int generate_tlvs(char **s_buffer){
    int len;
    STUDENT_INFO *tmp;
    for ( len = 0, tmp = &student_info ; tmp != NULL ; tmp = tmp->next ){
        len += strlen(tmp->faculty)  + strlen(tmp->id) + strlen(tmp->name) + 9;
    }
    len += 3;

    char type;
    char slen;
    int i;
    char * buffer = malloc(sizeof(char) * len);
    for ( i = 0, tmp = &student_info ; tmp != NULL ; tmp = tmp->next ){

        type = 1;                 buffer[i] = type;   i += 1;
        slen = strlen(tmp->id) + 1;buffer[i] = slen;    i += 1;
        strcpy(&buffer[i], tmp->id);      i += strlen(tmp->id)      +1;

        type = 2;                   buffer[i] = type;   i += 1;
        slen = strlen(tmp->name) + 1;buffer[i] = slen;    i += 1;
        strcpy(&buffer[i], tmp->name);    i += strlen(tmp->name)    +1;

        type = 3;                      buffer[i] = type;   i += 1;
        slen = strlen(tmp->faculty) + 1;buffer[i] = slen;    i += 1;
        strcpy(&buffer[i], tmp->faculty); i += strlen(tmp->faculty) +1;
    }

        type = 0;       buffer[i] = type;   i += 1;
        len = 0;        buffer[i] =slen;    i += 1;

    *s_buffer = buffer;
    return i;
}


int csismp_send(int send_socket, unsigned char dest_addr[6], int type, char* tlvs, int s_len){
    static unsigned char buffer[BUFFER_MAX];
    static int session = 0; // session id
    session += 1;
    if (session > 1000) session = 1;

    if (type == 3 || type == 4)
    {
        //rand int <1000 return randint1000
        csismp_construct(buffer, (unsigned char *)config_mac.mac_address, dest_addr,
                            type, 1, 1, 0, session, "\0\0",2); //!
        _csismp_send(send_socket, buffer, 24);
    }
    else if (type == 5)
    {
        int left_length = s_len;
        int s_num = 0;
        char *now_tlv = tlvs;
        struct tlv *t = (void *)now_tlv;
        while (left_length > 0)
        {
            unsigned char *now_point = buffer+22;
            int size = t->len;
            while (size < 1022)
            {
                int len = t->len + 2;
                memcpy(buffer, t, len);
                left_length -= len;
                now_point += len;
                now_tlv += len;
                t = (void *)now_tlv;
                size += t->len;
            }
            csismp_construct(buffer, (unsigned char *)config_mac.mac_address, dest_addr, type, (s_num == 0), (left_length <= 0), s_num, session, "", 0);
            _csismp_send(send_socket, buffer, size+22);
            ++s_num;
        }
    }

    free(tlvs);
    return 0;
}

