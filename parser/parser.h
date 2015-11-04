#include <stdlib.h>
#include "packet_struct.h"

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
    }
    char tlvs[];
    
}
#define ntoh_4bytes(buf) ntoh_nbits(buf, 32)
#define ntoh_2bytes(buf) ntoh_nbits(buf, 16)
#define ntoh_6bytes(buf) ntoh_nbits(buf, 24)
#define ntoh_8bytes(buf) ntoh_nbits(buf, 64)

void swap_byte(unsigned char *b1, unsigned char *b2);

void ntoh_nbits(unsigned char *buf, size_t len);

void ntoh(unsigned char *dbuf, unsigned char *sbuf, size_t len);

struct slice * parser(unsigned char *buf);