#ifndef PACKET_H
#define PACKET_H

#include <time.h>

struct stu_full
{
    enum S_TYPE
    {
        ID,
        NAME,
        ACADEMY
    } type;
    unsigned char len;
    char *content;
    struct stu_full *next;
};

struct stu_id
{
    unsigned char len;
    char *id;
    struct stu_id *next;
};

typedef struct slice{
    enum P_TYPE
    {
        ADD,
        DEL,
        SYN,
        ERROR
    } type;

    time_t timestamp;

    unsigned char smac[6];

    int start;
    int end;
    int sli_num;
    int session;
    union
    {
        struct stu_full* content;
        struct stu_id *id_content;
    };
} SLICE;
#endif
