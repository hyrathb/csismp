#include "packet_struct.h"
#include "parser.h"
#include <stdlib.h>

static const unsigned char rbits[8] = {0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f};
static const unsigned char lbits[8] = {0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};

void swap_byte(unsigned char *b1, unsigned char *b2)
{
    unsigned char temp = *t1;
    *t1 = *t2;
    *t2 = temp;
}

void ntoh_nbits(unsigned char *dst, size_t n)
{
    if (n <= 8)
        return;
    unsigned int bytes = n/8, tail = n%8;
    unsigned char *h = dst, *t = dst+bytes-1;
    while (h < t)
    {
        swap_byte(h, t);
        ++h;
        --t;
    }
    if (tail)
    {
        unsigned char left = (*(dst+bytes) & lbits[tail]);
        unsigned char *tmp = dst;
        while (tmp < dst+bytes)
        {
            unsigned char tleft = (*tmp & rbits[tail]) << (8 - tail);
            *tmp >>= tail;
            *tmp |= left;
            left = tleft;
            ++tmp;
        }
        *tmp &= rbits[8-tail];
        *tmp |= left;
    }
}

void ntoh(unsigned char *dbuf, const unsigned char *sbuf, size_t len)
{
   memcpy(dbuf, sbuf, len);
   ntoh_nbits(dbuf, 24);
   dbuf += 6;
   ntoh_nbits
}

struct slice * parser(unsigned char *buf)
{
    
}