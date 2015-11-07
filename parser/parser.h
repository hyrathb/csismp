#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "packet_struct.h"

#pragma pack(2)
struct tlv
{
    char type;
    unsigned char len;
    char val[0];
};

#pragma pack(2)
struct packet
{
    unsigned char smac[6];
    unsigned char dmac[6];
    short pro_type;
    struct
    {
        int c_type:8;
        int start:1;
        int end:1;
        int slice:22;
    };
    int session;
    char tlvs[];
};


void fill_slice_common(struct slice *s, const struct packet *p);

int check_tlv(const struct tlv *t, size_t left_len, char type_limit);

int fill_slice_stu_full(struct slice *s, const char *stu, size_t max_len);

int fill_slice_stu_id(struct slice *s, const char *id, size_t max_len);

/************Don't do anything if it returns NULL************************/
struct slice * parser(unsigned char *buf, size_t len);
#endif