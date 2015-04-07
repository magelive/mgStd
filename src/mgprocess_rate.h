#ifndef _MG_PROCESS_RATE_
#define _MG_PROCESS_RATE_

typedef struct mgprocess_struct
{
    int     length;         //显示的总长度
    char    *bar;           //输出形状
    char    *message;       //前面的提示信息
}mgprocess_t;

mgprocess_t *mgprocess_init(int length, char base, char *message);

void mgprocess_out_rate(mgprocess_t *process, int rate);

void mgprocess_finish(mgprocess_t *process);

#endif

