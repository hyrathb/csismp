#include "thread.h"
#include "cssismp_send.h"
#include "handle_data.h"
#include "parser.h"

int main(int arg, char **argv){
    FILE *config,*data;
    if(!(config = fopen(CONFIG_FILE,"r")))
    {
        printf("error while reading config file\n");
        exit(-1);
    }

    MAC* mac_p = (MAC*)malloc(sizeof(*mac_p));
    MAC* mac_head = mac_p;
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

    if(!(data = fopen(DATA_FILE,"r+")))
    {
        printf("error while reading data file\n");
        exit(-1);
    }
    info_p = (STUDENT_INFO*)malloc(sizeof(*info_p));
    info_head = info_p;
    info_head->next = NULL;
    info_p = info_head;
    read_data_file(data, &info_p);
    fclose(data);
    print_school_info(&info_head->next);

    SESSION* session_head = (SESSION*)malloc(sizeof(*session_head));
    session_head->next = NULL;
    SESSION* session_p = session_head;

    main_thread(session_head);
    return 0;
}
