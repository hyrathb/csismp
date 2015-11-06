#include "utils.h"

#include <stdlib.h>
#include <string.h>

static const unsigned char rbits[8] = {0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f};
static const unsigned char lbits[8] = {0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};

void swap_byte(unsigned char *b1, unsigned char *b2)
{
    unsigned char temp = *b1;
    *b1 = *b2;
    *b2 = temp;
}

void ntoh_nbytes(unsigned char *dst, size_t n, size_t silly_bits)
{
    if (n <= 8)
        return;
    unsigned char *h=dst, *t=dst+n-1;
    unsigned char silly=*h & lbits[silly];
    while (h<t)
    {
        swap_byte(h, t);
        ++h;
        --t;
    }
    if (silly_bits)
    {
        char *h=dst, *t=dst+n-1;
        *t <<= silly_bits;
        unsigned char left = silly;
        while (h <= t)
        {
            unsigned char tleft = (*h & rbits[silly_bits]) << (8 - silly_bits);
            *h >>= silly_bits;
            *h |= left;
            left = tleft;
            ++h;
        }
    }
}

void ntoh(unsigned char *dbuf, size_t len)
{
   ntoh_6bytes(dbuf);
   dbuf += 6;

   ntoh_6bytes(dbuf);
   dbuf += 6;

   ntoh_2bytes(dbuf);
   dbuf += 2;

   dbuf += 1;

   ntoh_nbytes(dbuf, 3, 2);
   dbuf += 3;

   ntoh_4bytes(dbuf);
   dbuf += 4;
}


char *get_interface_name(){
    struct ifaddrs *ifa = NULL, *ifList;
    char *interface_name = malloc(sizeof(char) * 12);
    interface_name[0] = '\0';

    if (getifaddrs(&ifList) < 0)
    {
        return NULL;
    }

    for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr->sa_family == AF_INET)
        {
            if (strlen(interface_name) == 0)
                strcpy(interface_name, ifa->ifa_name);
            else
                if (strcmp(interface_name, ifa->ifa_name) > 0)
                    strcpy(interface_name, ifa->ifa_name);
        }
    }

    freeifaddrs(ifList);
    return interface_name;
}

char *format(char *mac){  // "FF-FF-FF-FF-FF-FF" or "FFFFFFFFFFFF" to unsigned char[6]
    int j, k;
    unsigned char *format_mac = malloc(sizeof(char) * 7);
    memset (format_mac, 0, sizeof(char) * 7);

    if (mac[2] == '-'){
        for( j = k = 0 ;  mac[j]!='\0' && j < 18 ; j += 3, k += 1 ){

            format_mac[k] = ( (uint8_t) ( (mac[j] - 'A') >= 0 ? mac[j] - 'A' + 10 : mac[j] - '0' ) ) << 4 |
                                    ( (uint8_t) ((mac[j+1] - 'A') >= 0 ? mac[j+1] - 'A' +10 : mac[j+1] - '0' ));
        }
    }
    else{
        for( j = k = 0 ;  mac[j]!='\0' && j < 13 ; j += 2, k += 1 ){

            format_mac[k] = ( (uint8_t) ( (mac[j] - 'A') >= 0 ? mac[j] - 'a' +10 : mac[j] - '0' ) ) << 4 |
                                    ( (uint8_t)(mac[j+1] - 'A') >= 0? mac[j+1] - 'A' +10 : mac[j+1] - '0' );
        }
    }
    format_mac[6] = '\0';
    return format_mac;
}

uint64_t transform_mac_to_int64(unsigned char *mac){ //char[6] to 0xFFFFFFFFFFFF
    uint64_t smac = (uint64_t) (unsigned char)mac[0]<<40 |(uint64_t) (unsigned char)mac[1] <<32|(uint64_t) (unsigned char)mac[2]<<24|
                        (uint64_t) (unsigned char)mac[3] <<16|(uint64_t) (unsigned char)mac[4] <<8|(uint64_t) (unsigned char)mac[5];
    return smac;
}

char* mac_upper(const char *str){
    char *buffer = malloc(strlen(str));

    int i;
    for ( i = 0 ; *(str + i) != '\0' ; ++i ){
        if ( *(str + i) < 'z' && *(str + i) > 'a')
            *(buffer + i) = *(str + i) - 'a' + 'A';
        else
            *(buffer + i) = *(str + i);
    }
    return buffer;
}

