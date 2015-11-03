#ifndef PACKET_H
#define PACKET_H

struct stu_full
{
    enum
    {
        ID,
        NAME,
        ACADEMY
    }
    char *content;
    stu_full *next;
}

struct stu_id
{
    char *id;
    struct stu_id *next;
}

struct slice{
    enum TYPE
    {
        ADD,
        DEL,
        SYN,
        ERROR
    } type;
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
    }
}
#endif