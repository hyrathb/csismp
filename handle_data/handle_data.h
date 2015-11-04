#ifndef HANDLE_DATA_H
#define HANDLE_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE "Config.txt"

typedef struct mac{
    char* mac_address;
    struct mac* next;
}MAC;

typedef struct student_info{
    char* faculty;
    char* id;
    char* name;
    struct student_info* next;
}STUDENT_INFO;

/*struct name:stu_full
 *
 *to provide category and content 
 *information
 */
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

/*struct name:stu_id
 *
 *if the state is del,just use it
 *to del students who has the same
 *id
 */
struct stu_id
{
    char *id;
    struct stu_id *next;
};

/*struct name:
 *
 *the data passing format
 *
 */
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

typedef struct session
{
    int id;
    struct session* next;
    struct slice* info;
}SESSION;

int read_config(FILE *config, MAC** link_mac_address);
int store_mac_address(char* mac_address, MAC** link_mac_address);

#endif
