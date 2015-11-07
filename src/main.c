#include "thread.h"
#include "cssismp_send.h"
#include "handle_data.h"
#include "parser.h"

extern MAC *mac_head;

int main(int arg, char **argv){
    FILE *config,*data;
    if(!(config = fopen(CONFIG_FILE,"r")))
    {
        printf("error while reading config file\n");
        exit(-1);
    }

    MAC* mac_p = (MAC*)malloc(sizeof(*mac_p));
    mac_head = mac_p;
    mac_head->next = NULL;
    mac_p = mac_head;

    if(!read_config(config,&mac_p))
    {
        printf("error while reading config\n");
        exit(-1);
    }
    fclose(config);


    MAC* test3 = mac_head->next->next;
    sort_mac_address(&test3);
    print_config_to_file(mac_head);

    info_p = (STUDENT_INFO*)malloc(sizeof(*info_p));
    info_head = info_p;
    info_head->next = NULL;
    info_p = info_head;
    print_school_info(&info_head->next);

    session_head = (SESSION*)malloc(sizeof(*session_head));
    session_head->next = NULL;
    session_p = session_head;

    academy_head = (OTHER_ACADEMY*)malloc(sizeof(OTHER_ACADEMY));
    academy_head->next = NULL;
    academy_p = academy_head;

    main_thread();
    return 0;
}
