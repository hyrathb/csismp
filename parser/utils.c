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
