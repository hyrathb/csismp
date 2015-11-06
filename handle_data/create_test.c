#include "handle_data.h"

int main()
{
    SLICE session1_0, session1_1, session1_2, session1_3;
    /*
    SLICE session2_0, SESSION2_1;
    SLICE session3;
    SLICE session4, session5, session6;*/

    struct stu_full _stu_full[9];
    struct stu_id _stu_id[9];

    int i = 0;
    _stu_full[0].content = _stu_id[i].id = _stu_id[i+1].id = _stu_id[i+2].id = "U201414093";
    _stu_full[i].type = _stu_full[i+3].type = _stu_full[i+6].type = ID;
    i = 1;
    _stu_full[3].content = _stu_id[i+2].id = _stu_id[i+3].id = _stu_id[i+4].id = "U201314093";
    _stu_full[i].type = _stu_full[i+3].type = _stu_full[i+6].type = NAME;
    i = 2;
    _stu_full[6].content = _stu_id[i+5].id = _stu_id[i+6].id = _stu_id[i+7].id = "U201214093";
    _stu_full[i].type = _stu_full[i+3].type = _stu_full[i+6].type = ACADEMY;

    _stu_full[1].content = "nico1";_stu_full[4].content = "nico2";_stu_full[7].content = "nico3";

    _stu_full[2].content = "ll1";_stu_full[5].content =  "ll2";_stu_full[8].content =  "ll3";

    for ( i = 0 ; i < 9; ++i)
    {
        _stu_full[i].next = NULL;
        _stu_id[i].next = NULL;
    }

    /*****session 1 ************/
    session1_0.type = session1_1.type = session1_2.type = session1_3.type = ADD;

    session1_0.smac[0] = session1_1.smac[0] = session1_2.smac[0] = session1_3.smac[0] = 0xff;
    session1_0.smac[1] = session1_1.smac[1] = session1_2.smac[1] = session1_3.smac[1] = 0xff;
    session1_0.smac[2] = session1_1.smac[2] = session1_2.smac[2] = session1_3.smac[2] = 0xff;
    session1_0.smac[3] = session1_1.smac[3] = session1_2.smac[3] = session1_3.smac[3] = 0xff;
    session1_0.smac[4] = session1_1.smac[4] = session1_2.smac[4] = session1_3.smac[4] = 0xff;
    session1_0.smac[5] = session1_1.smac[5] = session1_2.smac[5] = session1_3.smac[5] = 0xff;


    session1_0.start = session1_1.start = session1_2.start = session1_3.start = 0;
    session1_0.end = session1_1.end = session1_2.end = session1_3.end = 0;

    session1_0.sli_num = 0;
    session1_1.sli_num = 1;
    session1_2.sli_num = 2;
    session1_3.sli_num = 3;

    session1_0.start = 1;   session1_3.end = 1;

    session1_0.session = session1_1.session = session1_2.session = session1_3.session = 1;


    session1_0.content = &_stu_full[0];
    session1_1.content = &_stu_full[1];
    session1_2.content = &_stu_full[2];
    session1_3.content = &_stu_full[3];

    session1_0.id_content = &_stu_id[0];
    session1_1.id_content = &_stu_id[1];
    session1_2.id_content = &_stu_id[2];
    session1_3.id_content = &_stu_id[3];

    for( i = 3 ; i < 9 ; ++i ){
        _stu_full[i].next = &_stu_full[i+1];
        _stu_id[i].next = &_stu_id[i+1];
    }





}
