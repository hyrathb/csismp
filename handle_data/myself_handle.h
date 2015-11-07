#ifndef HANDLE_DATA_H
#define HANDLE_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#define CONFIG_FILE "Config.txt"
#define WRITE_CONFIG_FILE "CfgInfo.txt"
#define DATA_FILE "StuInfo.test"
#define DATA_TEST_FILE "StuInfo.txt"

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
typedef struct slice{
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
}SLICE;

typedef struct member
{
    int sli_num;
    struct member* next;
    union
    {
        struct stu_full* content;
        struct stu_id *id_content;
    };   
}MEMBER;

typedef struct other_academy
{
    STUDENT_INFO* stu_information;
    unsigned char smac[6];
    struct other_academy* next;
}OTHER_ACADEMY;

typedef struct session
{
    int id;
    int amount;
    enum P_TYPE type;
    time_t first_time;
    char mac_address[6];
    struct session* next;
    struct member* member;
}SESSION;

int read_config(FILE *config, MAC** link_mac_address);
int store_mac_address(char* mac_address, MAC** link_mac_address);
int if_dst_mac_address(char* compare_list, MAC* mac_head);
int is_time_expire(time_t fist_package_time,time_t current_package_time);
int read_data_file(FILE* data_file,STUDENT_INFO** link_mac_address);
char* join(char* string1,char* string2);
int store_student_info(char* faculty, char* student_id, char* student_name,STUDENT_INFO** link_student_info);
SESSION* which_session_to_go(SLICE package,SESSION* session_func_head);
//char* mac_address_format_convert(char* config_format);
int append_slice_to_session(SESSION** current_node,SLICE package,enum  P_TYPE type);
int handle_session(SESSION** current_node,STUDENT_INFO** link_student_info,OTHER_ACADEMY** other_stu_info);
int free_session(SESSION**current_node);
int deal_session_in_student_info(SESSION** current_node,STUDENT_INFO** link_student_info,\
    enum P_TYPE type,OTHER_ACADEMY** other_stu_info);
int slice_handle(SLICE package,SESSION** data_session,STUDENT_INFO** link_student_info);
int create_session_and_add_slice(SESSION** current_node,SLICE package,enum  P_TYPE type);
int print_config_to_file(MAC* mac_head);
int sort_mac_address(MAC** link_mac_address);
int print_school_info(STUDENT_INFO** link_student_info);
int sync_school_info_in_memory(SESSION** current_node,OTHER_ACADEMY** other_stu_info);
int free_stu_node(STUDENT_INFO** all_clean);
char* get_time(void);
#endif
