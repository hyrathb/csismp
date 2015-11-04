#ifndef HANDLE_DATA_H
#define HANDLE_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct destination_mac{
    char* mac_address;
    struct destination_mac* next;
}DST_MAC;

typedef struct student_info{
    char* faculty;
    char* id;
    char* name;
    struct student_info* next;
}STUDENT_INFO;

struct stu_full
{
    enum S_TYPE
    {
        ID,
        NAME,
        ACADEMY
    } type;
    char *content;
    struct stu_full *next;
};

struct stu_id
{
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

typedef struct session
{
    int id;
    struct session* next;
    struct slice* info;
}SESSION;


#endif
