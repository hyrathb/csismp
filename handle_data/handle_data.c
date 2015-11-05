#include "handle_data.h"


int main(void)
{
    FILE *config,*data;
    char config_file_path[]=CONFIG_FILE;
    char data_file_path[]=DATA_FILE;
    if(!(config = fopen(config_file_path,"r")))
    {
        printf("error while reading config file\n");
        exit(-1);
    }
    //todo:create init function
    MAC* mac_p = (MAC*)malloc(sizeof(*mac_p));
    MAC* mac_head = mac_p;                              //head(the first) node does not store data
    mac_head->next = NULL;
    mac_p = mac_head;

    if(!read_config(config,&mac_p))
    {
        printf("error while reading config\n");
        exit(-1);
    }

    //this is the mac_address link test
    MAC* test1 = mac_head->next;
    printf("link test!\n");
    while(test1!=NULL)
    {
        printf("mac address is %s\n",test1->mac_address );
        test1 = test1->next;
    }
    //test end

    if(!(data = fopen(data_file_path,"r+")))
    {
        printf("error while reading data file\n");
        exit(-1);
    }
    STUDENT_INFO* info_p = (STUDENT_INFO*)malloc(sizeof(*info_p));
    STUDENT_INFO* info_head = info_p;
    info_head->next = NULL;
    info_p = info_head;
    read_data_file(data, &info_p);



    //begin handle data(struct slice):
    //todo:init func
    SESSION* session_head = (SESSION*)malloc(sizeof(*session_head));
    session_head->next = NULL;
    session_head->info = NULL;
    SESSION* session_p = session_head;

    return 0;
}


/*function name:read_config
 *
 *to read Config.txt and get the local mac
 *and destination mac
 */
int read_config(FILE* config,MAC** link_mac_address)//todo:deal with wrong input file
{
    //sscanf
    int line;
    char StrLine[80];
    while (!feof(config)) 
    {
        char tmp_mac_address[18]; 
        fgets(StrLine,80,config);  //read a line
        if (strstr(StrLine,"local mac"))
        {
            sscanf(StrLine, "%*[^:]:%s" , tmp_mac_address ) ;
            //printf("local mac is %s\n",tmp_mac_address );
            store_mac_address(tmp_mac_address,link_mac_address);
        }
        else
            if (strstr(StrLine,"destination mac"))
        {
            sscanf(StrLine, "%*[^:]:%s" , tmp_mac_address ) ;
            //printf("destination mac is %s\n",tmp_mac_address );
            store_mac_address(tmp_mac_address,link_mac_address);
        }
            else
                {
                    sscanf(StrLine, "%[a-zA-Z0-9]" , tmp_mac_address );
                    //printf("destination mac is %s\n",tmp_mac_address );
                    store_mac_address(tmp_mac_address,link_mac_address);
                }

    }
    printf("read config over!\n"); 
    char* content;
    return 1;
}

/*function name:read_data_file
 *
 *to read Config.txt and get the local mac
 *and destination mac
 */
int read_data_file(FILE* data_file,STUDENT_INFO** link_mac_address)
{
    char a_line[81];
    char is_newline = 'n';
    char apart1[33];
    while(!feof(data_file))
    {
        fgets(a_line,81,data_file);
        if (a_line[34]=='\n')
        {
            is_newline = 'y';
            a_line[34] = '\0';
            strcpy(apart1,a_line);
        }
        else if (is_newline == 'y'&&a_line[0]!='-')
            {
                char apart2[33],student_id[11],name[20];
                sscanf(a_line,"%s %s %[a-zA-Z ]",apart2,student_id,name);
                printf("apart1 is :%s\n",apart1);
                printf("apart2 is :%s\n",apart2);
                printf("student_id is :%s\n",student_id);
                printf("name is :%s\n",name );
                is_newline = 'n';
            }
            /*else if (a_line[0]!='-')
                {
                    char apart2[33],student_id[11],name[20];
                    sscanf(a_line,"%s %s %s",apart2,student_id,name);
                    printf("%s %s %s\n",apart2,student_id,name );
                }*/

        
        //printf("%s\n",a_line );
    }

    return 1;
}

/*function name:store_dst_mac_address
 *to store destination mac address to a 
 *created link
 */
int store_mac_address(char* mac_address, MAC** link_mac_address)
{
    int address_length = strlen(mac_address);
    MAC* tmp_p = *link_mac_address;
    while(tmp_p!=NULL)
        tmp_p = tmp_p->next;
    //this step tmp_p is NULL
    tmp_p = (MAC*)malloc(sizeof(MAC));
    tmp_p->mac_address = (char*)malloc(address_length);
    strcpy(tmp_p->mac_address,mac_address);
    (*link_mac_address)->next = tmp_p;
    *link_mac_address=(*link_mac_address)->next;
    return 1;
}

/*function name:if_dst_mac_address
 *to judge if the mac_address is dst_address 
 */
int if_dst_mac_address(char* compare_list, MAC* mac_head)
{
    MAC * dst_start = mac_head->next->next;
    while(dst_start!=NULL)
    {
        if (strstr(compare_list, dst_start->mac_address))
        {
            return 1;
        }
    }
    return 0;
}

int which_session_to_go(SLICE* package,SESSION* session_func_head)
{
    return 1;
}

int is_time_expire(time_t fist_package_time,time_t current_package_time)
{
    return 1;
}
/*get data and decide to add,delete or
 *sync
 */
 /*
int data_handle()
{
    return 1;
}

int add_in_file()
{
    return 1;
}

int del_in_file()
{
    return 1;
}

int sync_in_file()
{
    return 1;
}*/

/* TODO
int free_link()
{
    return 1;
}
*/

