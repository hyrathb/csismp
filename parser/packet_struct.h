#ifndef PACKET_H
#define PACKET_H

#include <sys/time.h>

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

struct slice{
    enum P_TYPE
    {
        ADD,
        DEL,
        SYN,
        ERROR
    } type;
    
    time_t timestamp;
    
    char smac[6];
    
    struct
    {
        int start:1;
        int end:1;
        int sli_num:22;
    };
    int session;
    union
    {
        struct stu_full* content;
        struct stu_id *id_content;
    };
};
#endif