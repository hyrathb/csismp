#include "packet_struct.h"

struct packet
{
    char smac[6];
    char dmac[6];
    short pro_type;
    struct
    {
        char c_type;
    }
    
}

struct slice * parser(char *buf);
re_byte