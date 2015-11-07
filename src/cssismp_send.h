#ifndef CSSISMP_SEND
#define CSSISMP_SEND

int generate_tlvs(char *buffer );
void print_time();
int csismp_send(int send_socket, unsigned char dest_addr[6], int type, char* tlvs, int s_len);

#endif
