#include "handle_data.h"


int main(void)
{
    //printf("hello world!\n");
    FILE *config;
    char config_file_path[]=CONFIG_FILE;
    if(!(config = fopen(config_file_path,"r")))
    {
        printf("error while reading config file\n");
        exit(-1);
    }
    if(!read_config(config))
    {
        printf("error while reading config\n");
        exit(-1);
    }
    //fscanf(config,)
    return 0;
}


/*function name:read_config
 *
 *to read Config.txt and get the local mac
 *and destination mac
 */
int read_config(FILE *config)
{
    //sscanf
    int line;
    char StrLine[80];
    while (!feof(config)) 
    {
        char local_mac_address[18];
        char tmp_mac_address[18]; 
        fgets(StrLine,80,config);  //read a line
        //printf("%s\n", StrLine); //print it
        if (strstr(StrLine,"local mac"))
        {
            sscanf(StrLine, "%*[^:]:%s" , tmp_mac_address ) ;
            printf("local mac is %s\n",tmp_mac_address );
        }
        else
            if (strstr(StrLine,"destination mac"))
        {
            sscanf(StrLine, "%*[^:]:%s" , tmp_mac_address ) ;
            printf("destination mac is %s\n",tmp_mac_address );
        }
            else
                {
                    sscanf(StrLine, "%[a-zA-Z0-9]" , tmp_mac_address );
                    printf("destination mac is %s\n",tmp_mac_address );
                }

    }
    printf("read config over!\n"); 
    char* content;
    return 1;
}

/*function name:store_dst_mac_address
 *to store destination mac address to a 
 *created link
 */
int store_mac_address(char* mac_address)
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

