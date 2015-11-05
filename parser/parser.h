#include <stdlib.h>
#include "packet_struct.h"

struct tlv
{
    char type;
    unsigned char len;
    char val[0];
}

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
    };
    char tlvs[];
    
};
#define ntoh_2bytes(buf) ntoh_nbytes(buf, 2, 0)
#define ntoh_4bytes(buf) ntoh_nbytes(buf, 4, 0)
#define ntoh_6bytes(buf) ntoh_nbytes(buf, 6, 0)
#define ntoh_8bytes(buf) ntoh_nbytes(buf, 8, 0)

void swap_byte(unsigned char *b1, unsigned char *b2);

void ntoh_nbytes(unsigned char *buf, size_t len, size_t silly_bits);

void ntoh(unsigned char *dbuf, size_t len);

void fill_slice_common(struct slice *s, const struct packet *p);

int check_field()

int fill_slice_stu_full(stuct slice *s, const struct stu_full *stu);

int fill_slice_stu_id(struct slice *s, const struct stu_id *id);

struct slice * parser(unsigned char *buf, size_t len);