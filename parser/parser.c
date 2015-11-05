#include "packet_struct.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const unsigned char rbits[8] = {0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f};
static const unsigned char lbits[8] = {0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
static const char max_len[4] = {0, 12, 16, 64};

void swap_byte(unsigned char *b1, unsigned char *b2)
{
    unsigned char temp = *b1;
    *b1 = *b2;
    *b2 = temp;
}

void ntoh_nbytes(unsigned char *dst, size_t n, size_t silly_bits)
{
    if (n <= 8)
        return;
    unsigned char *h=dst, *t=dst+n-1;
    unsigned char silly=*h & lbits[silly];
    while (h<t)
    {
        swap_byte(h, t);
        ++h;
        --t;
    }
    if (silly_bits)
    {
        char *h=dst, *t=dst+n-1;
        *t <<= silly_bits;
        unsigned char left = silly;
        while (h <= t)
        {
            unsigned char tleft = (*h & rbits[silly_bits]) << (8 - silly_bits);
            *h >>= silly_bits;
            *h |= left;
            left = tleft;
            ++h;
        }
    }
}

void ntoh(unsigned char *dbuf, size_t len)
{
   ntoh_6bytes(dbuf);
   dbuf += 6;
   
   ntoh_6bytes(dbuf);
   dbuf += 6;
   
   ntoh_2bytes(dbuf);
   dbuf += 2;
   
   dbuf += 1;
   
   ntoh_nbytes(dbuf, 3, 2);
   dbuf += 3;
   
   ntoh_4bytes(dbuf);
   dbuf += 4;
}

void fill_slice_common(struct slice *s, const struct packet *p)
{
    s->timestamp = time(NULL);
    memcpy(s->smac, p->smac, 6);
    s->smac[6] = 0;
    s->start = p->start;
    s->end = p->end;
    s->sli_num = p->slice;
    s->session = p->session;
}

int check_tlv(const struct tlv *t, size_t left_len, char type_limit)
{
    if ((t->type < 0) || (t->type > 3))
        return -1;
    
    if ((type_limit != -1) && t->type && (t->type != type_limit))
        return -1;
    
    if ((t->len > left_len) || (t->len > max_len[t->type]))
        return -1;
    
    if ((strlen(t->val)+1) != t->len)
        return -1;
    
   return 0; 
}

int fill_slice_stu_full(struct slice *s, const char *stu, size_t max_len)
{
    s->content = NULL;
    struct tlv *tlvs = (void *)stu;
    if (check_tlv(tlvs, max_len, -1))
        return -1;
    
    if (!tlvs->type)
        return 0;
    
    struct stu_full *tmplv = malloc(sizeof(struct stu_full));
    
    tmplv->type = tlvs->type - 1;
    tmplv->len = tlvs->len-1;
    tmplv->content = malloc(tlvs->len);
    strcpy(tmplv->content, tlvs->val);
    tmplv->next = NULL;
    max_len -= tlvs->len + 2;
    s->content = tmplv;
    stu += tlvs->len + 2;
    
    int next_type = tlvs->type;
    
    while (max_len > 0)
    {
        tlvs = (void *)stu;
        if (next_type == 3)
            next_type = 1;
        else
            ++next_type;
        
        if (check_tlv(tlvs, max_len, next_type))
        {
            struct stu_full *t1, *t2;
            for (t1=s->content, t2=t1->next; t2; t1=t2, t2=t2->next)
            {
                free(t1->content);
                free(t1);
            }
            free(t1->content);
            free(t1);
            s->content =NULL;
            return -1;
        }
        else if (tlvs->type)
        {
            struct stu_full *new_lv = malloc(sizeof(struct stu_full));
            new_lv->type = tlvs->type-1;
            new_lv->len = tlvs->len-1;
            new_lv->content = malloc(tlvs->len);
            strcpy(new_lv->content, tlvs->val);
            new_lv->next = NULL;
            max_len -= tlvs->len + 2;
            tmplv->next = new_lv;
            tmplv = new_lv;
            stu += tlvs->len + 2;
        }
        else
            return 0;
    }
    
    struct stu_full *t1, *t2;
    for (t1=s->content, t2=t1->next; t2; t1=t2, t2=t2->next)
    {
        free(t1->content);
        free(t1);
    }
    free(t1->content);
    free(t1);
    s->content =NULL;
    return -1;
}

int fill_slice_stu_id(struct slice *s, const char *id, size_t max_len)
{
    s->id_content = NULL;
    struct tlv *tlvs = (void *)id;
    if (check_tlv(tlvs, max_len, 1))
        return -1;
    
    if (!tlvs->type)
        return 0;
    
    struct stu_id *tmpid = malloc(sizeof(struct stu_id));
    tmpid->len = tlvs->len-1;
    tmpid->id = malloc(tlvs->len);
    strcpy(tmpid->id, tlvs->val);
    tmpid->next = NULL;
    max_len -= tlvs->len + 2;
    s->id_content = tmpid;
    id += tlvs->len + 2;
    
    while (max_len > 0)
    {
        tlvs = (void *)id;
        if (check_tlv(tlvs, max_len, 1))
        {
            struct stu_id *t1, *t2;
            for (t1=s->id_content, t2=t1->next; t2; t1=t2, t2=t2->next)
            {
                free(t1->id);
                free(t1);
            }
            free(t1->id);
            free(t1);
            s->id_content =NULL;
            return -1;
        }
        else if (tlvs->type)
        {
            struct stu_id *new_id = malloc(sizeof(struct stu_id));
            new_id->len = tlvs->len-1;
            new_id->id = malloc(tlvs->len);
            strcpy(new_id->id, tlvs->val);
            new_id->next = NULL;
            max_len -= tlvs->len + 2;
            tmpid->next = new_id;
            tmpid = new_id;
            id += tlvs->len + 2;
        }
        else
            return 0;
    }
    
    struct stu_id *t1, *t2;
    for (t1=s->id_content, t2=t1->next; t2; t1=t2, t2=t2->next)
    {
        free(t1->id);
        free(t1);
    }
    free(t1->id);
    free(t1);
    s->id_content =NULL;
    return -1;
}

struct slice * parser(unsigned char *buf, size_t len)
{
    struct slice *new_slice = malloc(sizeof(struct slice));
    new_slice->type = ERROR;
    new_slice->smac[0] = 0;
    new_slice->content = NULL;
    
    if (len <= 22 || len >1046)
        return new_slice;
    
    unsigned char *new_buf = malloc(len);
    memcpy(new_buf, buf, len);
    ntoh(new_buf, len);
    struct packet *p = (void *)new_buf;
    
    if (p->pro_type != 0x1122)
    {
        free(new_buf);
        return new_slice;
    }
    
    switch(p->c_type)
    {
        case 1:
        {
            fill_slice_common(new_slice, p);
            if (fill_slice_stu_full(new_slice, p->tlvs, len-22))
            {
                free(new_slice->smac);
                new_slice->smac[0] = 0;
                new_slice->type = ERROR;
            }
            else
                new_slice->type = ADD;
            break;
        }
        case 2:
        {
            fill_slice_common(new_slice, p);
            if (fill_slice_stu_id(new_slice, p->tlvs, len-22))
            {
                free(new_slice->smac);
                new_slice->smac[0] = 0;
                new_slice->type = ERROR;
            }
            else
                new_slice->type = DEL;
            break;
        }
        case 3:
        case 4:
            free(new_buf);
            free(new_slice);
            return NULL;
        case 5:
        {
            fill_slice_common(new_slice, p);
            if (fill_slice_stu_full(new_slice, p->tlvs, len-22))
            {
                free(new_slice->smac);
                new_slice->smac[0] = 0;
                new_slice->type = ERROR;
            }
            else
                new_slice->type = SYN;
            break;      
        }
        default:
            break;
    }
    free(new_buf);
    return new_slice;
}