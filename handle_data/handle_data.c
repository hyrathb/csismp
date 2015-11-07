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
        mac_address_format_convert(test1->mac_address);
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

    //this is the stu_info link test
    /*STUDENT_INFO* test2 = info_head->next;
    printf("link test2!\n");
    while(test2!=NULL)
    {
        printf("student info faculty is %s\n",test2->faculty);
        printf("student info id is %s\n",test2->id);
        printf("student info name is %s\n",test2->name);
        test2 = test2->next;
    }*/
    //test end

    //this is slice test
    /*SLICE test3 = {
        ADD,1,
    }*/

    //test end

    //begin handle data(struct slice):
    //todo:init func
    SESSION* session_head = (SESSION*)malloc(sizeof(*session_head));
    session_head->next = NULL;
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
int read_data_file(FILE* data_file,STUDENT_INFO** link_student_info)
{
    char a_line[81];
    char is_newline = 'n';
    char apart1[33];
    while(!feof(data_file))
    {
        fgets(a_line,81,data_file);     
        if (a_line[34]=='\n')                    //a_line[33]='\r',a_line[34]='\n'
        {
            is_newline = 'y';
            a_line[33] = '\0';
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
                store_student_info(join(apart1,apart2),student_id,name,link_student_info);
                is_newline = 'n';
            }
            /*else if (a_line[0]!='-'&&is_newline=='n'&&(!strstr(a_line,"Faculty")))
                {
                    char apart2[33],student_id[11],name[20];
                    sscanf(a_line,"%s %s %[a-zA-Z ]",apart2,student_id,name);
                    store_student_info(apart2,student_id,name,link_student_info);
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

int store_student_info(char* faculty, char* student_id, char* student_name,STUDENT_INFO** link_student_info)
{
    STUDENT_INFO* stu_tmp = *link_student_info;
    while(stu_tmp!=NULL)
        stu_tmp = stu_tmp->next;
    stu_tmp = (STUDENT_INFO*)malloc(sizeof(*stu_tmp));
    stu_tmp->faculty = (char*)malloc(strlen(faculty));
    stu_tmp->id = (char*)malloc(strlen(student_id));
    stu_tmp->name = (char*)malloc(strlen(student_name));
    strcpy(stu_tmp->faculty,faculty);
    strcpy(stu_tmp->id,student_id);
    strcpy(stu_tmp->name,student_name);

    (*link_student_info)->next = stu_tmp;
    *link_student_info = (*link_student_info)->next;
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

SESSION* which_session_to_go(SLICE* package,SESSION* session_func_head)
{
    while(session_func_head!=NULL)
    {
        if (package->session == session_func_head->id)
        {
            return session_func_head;
        }
        session_func_head = session_func_head->next;
    }
    return NULL;
}

int is_time_expire(time_t first_package_time,time_t current_package_time)
{
    if (current_package_time-first_package_time<5)
    {
        return 0;
    }
    return 1;
}

char* join(char* string1,char* string2)
{
    char* return_line = (char*)malloc(strlen(string1)+strlen(string2));
    strcpy(return_line,string1);
    strcat(return_line,string2);
    return return_line;
}
/*get data and decide to add,delete or
 *sync
 */
 
int slice_handle(SLICE package,SESSION** data_session,STUDENT_INFO** link_student_info)
{
    SESSION *p_session = *data_session;
    int judge_return = 0;
    while(p_session!=NULL)
    {
        if (is_time_expire(p_session->first_time,package.timestamp))
        {
            handle_session(&p_session,link_student_info);
        }
        else if (p_session->id == package.session&&p_session->type == package.type&&!strcmp(p_session->mac_address,package.smac))
        {
            append_slice_to_session(&p_session,package,package.type);
            judge_return = 1;
        }
        p_session = p_session->next;
    }
    if (judge_return == 0)
    {
        create_session_and_add_slice(data_session,package,package.type);
    }
    return 0;
}

int append_slice_to_session(SESSION** current_node,SLICE package,enum  P_TYPE type)
{
    MEMBER* member_head = (*current_node)->member;
    if (package.start == 1)                     //append,maybe nodes live!
    {
        package.sli_num = 0;
    }
    else if (package.end == 1)
    {
        package.sli_num == -1;
    }
    if (type == DEL)
    {
        //struct stu_id* stu_id_head = member_head->id_content;
        int state;
        while(member_head!=NULL)
        {
            if (package.sli_num>member_head->sli_num)
            {
                member_head = member_head->next;
            }
            else
            {
                MEMBER* member_tmp = (MEMBER*)malloc(sizeof(*member_tmp));
                member_tmp->id_content = package.id_content;
                member_tmp->sli_num = package.sli_num;
                member_tmp->next = member_head;
                member_head = member_tmp;
                state = 1;
            }
        }
        if (state != 1)
        {
            MEMBER* member_tmp = (MEMBER*)malloc(sizeof(*member_tmp));
            member_tmp->id_content = package.id_content;
            member_tmp->sli_num = package.sli_num;
            member_tmp->next = NULL;
            member_head = member_tmp;
        }
        return 1;
    }
    else
    {
        int state;
        while(member_head!=NULL)
        {
            if (package.sli_num>member_head->sli_num)
            {
                member_head = member_head->next;
            }
            else
            {
                MEMBER* member_tmp = (MEMBER*)malloc(sizeof(*member_tmp));
                member_tmp->content = package.content;
                member_tmp->sli_num = package.sli_num;
                member_tmp->next = member_head;
                member_head = member_tmp;
                state = 1;
            }
        }
        if (state != 1)
        {
            MEMBER* member_tmp = (MEMBER*)malloc(sizeof(*member_tmp));
            member_tmp->content = package.content;
            member_tmp->sli_num = package.sli_num;
            member_tmp->next = NULL;
            member_head = member_tmp;
        }
        return 1;
    }
    
}

int handle_session(SESSION** current_node,STUDENT_INFO** link_student_info)
{
    SESSION* session_head = *current_node;
    MEMBER* member_head = session_head->member;
    if (session_head->type == DEL)
    {
        if (member_head->sli_num!=-1) //maybe there's one
        {
            MEMBER* tmp = member_head->next;
            if (tmp!=NULL)
            {
                printf("this session is nonsense!\n");
                free_session(current_node);
                return 0;
            }
            else
            {
                deal_session_in_student_info(current_node,link_student_info,DEL);
                free_session(current_node);
                return 1;
            }
        }
        else
        {
            int start = -1;
            while(member_head!=NULL)
            {
                if (member_head->sli_num == start)
                {
                    member_head = member_head->next;
                    start++;
                }
                else
                {
                    printf("this session is nonsense!\n");
                    free_session(current_node);
                    return 0;
                }
            }
            deal_session_in_student_info(current_node,link_student_info,DEL);
            free_session(current_node);
            return 1;
        }

    }
    else
    {
        if (member_head->sli_num!=-1) //maybe there's one
        {
            printf("this session is nonsense!\n");
            free_session(current_node);
            return 0;
        }
        else
        {
            int start = -1;
            while(member_head!=NULL)
            {
                if (member_head->sli_num == start)
                {
                    member_head = member_head->next;
                    start++;
                }
                else
                {
                    printf("this session is nonsense!\n");
                    free_session(current_node);
                    return 0;
                }
            }
            deal_session_in_student_info(current_node,link_student_info,session_head->type);
            free_session(current_node);
            return 1;
        }
    }
}

int create_session_and_add_slice(SESSION** current_node,SLICE package,enum  P_TYPE type)
{
    SESSION* ses_tmp = *current_node;
    while(ses_tmp!=NULL)
        ses_tmp = ses_tmp->next;
    ses_tmp = (SESSION*)malloc(sizeof(*ses_tmp));
    ses_tmp->id = package.session;
    ses_tmp->type = type;
    ses_tmp->first_time = package.timestamp;
    strcpy(ses_tmp->mac_address,package.smac);
    ses_tmp->next = NULL;
    ses_tmp->member = (MEMBER*)malloc(sizeof(MEMBER));
    ses_tmp->member->sli_num = package.sli_num;
    if (type == DEL)
    {       
        ses_tmp->member->id_content = package.id_content;
    }
    else
        ses_tmp->member->content = package.content;
    ses_tmp->member->next = NULL;

    (*current_node)->next = ses_tmp;
    *current_node = (*current_node)->next;
    return 1;
}

char* mac_address_format_convert(char* config_format)
{
    int a = strtoul(config_format,0,16);
    printf("mac_address_format_convert is %d\n",a );
}

int free_session(SESSION**current_node)
{
    return 1;
}

int deal_session_in_student_info(SESSION** current_node,STUDENT_INFO** link_student_info,enum P_TYPE type)
{
    STUDENT_INFO* stu_tmp = *link_student_info;
    STUDENT_INFO* iter_tmp = stu_tmp;
    MEMBER* mem_tmp = (*current_node)->member;
    if (type == DEL)
    {
        MEMBER* mem_test = mem_tmp;
        STUDENT_INFO* stu_test = stu_tmp;
        while(stu_test!=NULL&&mem_test!=NULL)
        {
            if (strcmp(stu_test->id,mem_test->id_content->id))
            {
                stu_test = stu_test->next;
            }
            else
                mem_test = mem_test->next;
        }
        if (mem_test!=NULL)
        {
            printf("DEL package wrong!\n");
            return 0;
        }
        while(mem_tmp!=NULL&&iter_tmp!=NULL)
        {
            if (strcmp(iter_tmp->id,mem_tmp->id_content->id))
            {
                iter_tmp = iter_tmp->next;
            }
            else
            {
                free(iter_tmp);
                iter_tmp = iter_tmp->next;
                mem_tmp = mem_tmp->next;
            }
        }
        *link_student_info = stu_tmp;
        return 1;
    }
    else if(type == ADD )
    {
        STUDENT_INFO* stu_node = (STUDENT_INFO*)malloc(sizeof(*stu_node));
        while(mem_tmp!=NULL)
        {
            if (mem_tmp->content->type == ID)
            {
                stu_node->id = (char*)malloc(strlen(mem_tmp->content->content));
                strcpy(stu_node->id,mem_tmp->content->content);
            }
            else
                if (mem_tmp->content->type == NAME)
                {
                    stu_node->name = (char*)malloc(strlen(mem_tmp->content->content));
                    strcpy(stu_node->name,mem_tmp->content->content);
                }
                else
                    if (mem_tmp->content->type == ACADEMY)
                    {
                        stu_node->faculty = (char*)malloc(strlen(mem_tmp->content->content));
                        strcpy(stu_node->faculty,mem_tmp->content->content);
                        store_student_info(stu_node->faculty,stu_node->id,stu_node->name,link_student_info);
                        printf("stored success!\n");
                    }       
        }
        
        return 1;
    }
    else if(type == SYN)
    {
        STUDENT_INFO* stu_node = (STUDENT_INFO*)malloc(sizeof(*stu_node));
        int judge_existence = 0;
        while(mem_tmp!=NULL)
        {
            
            
            if (mem_tmp->content->type == ID)
            {
                for (STUDENT_INFO* loop_head = *link_student_info; loop_head!=NULL; loop_head = loop_head->next)
                {                
                    if (!strcmp(mem_tmp->content->content,loop_head->id))
                    {
                        judge_existence = 1;
                    }
                }
                if (judge_existence == 0)
                {
                    stu_node->id = (char*)malloc(strlen(mem_tmp->content->content));
                    strcpy(stu_node->id,mem_tmp->content->content);
                }
                else
                {
                    mem_tmp = mem_tmp->next->next;
                    judge_existence = 0;
                }                
            }
            else
                if (mem_tmp->content->type == NAME)
                {
                    stu_node->name = (char*)malloc(strlen(mem_tmp->content->content));
                    strcpy(stu_node->name,mem_tmp->content->content);
                }
                else
                    if (mem_tmp->content->type == ACADEMY)
                    {
                        stu_node->faculty = (char*)malloc(strlen(mem_tmp->content->content));
                        strcpy(stu_node->faculty,mem_tmp->content->content);
                        store_student_info(stu_node->faculty,stu_node->id,stu_node->name,link_student_info);
                        printf("stored success!\n");
                    }
            mem_tmp = mem_tmp->next;       
        }
        
        return 1;
    }
    return 0;
}

/*
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

