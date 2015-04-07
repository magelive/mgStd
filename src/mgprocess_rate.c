#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mgprocess_rate.h"
/*
//类似wget的进度条的实现，实际就是转移符\r的使用，\r的作用是返回至行首而不换行
int wget_test()
{
    int i;
    unsigned len = 100;
    char *bar = (char *)malloc(sizeof(char) * (len + 1));
    for (i = 0; i < len + 1; ++i)
    {
        bar[i] = '#';
    }
    for (i = 0; i < len; ++i)
    {
        printf("progress:[%s]%d%%\r", bar+len-i, i+1);
        fflush(stdout);//一定要fflush，否则不会会因为缓冲无法定时输出。
        usleep(100000);
        //sleep(1);
    }
    printf("\n");
    return 0;
}
*/

mgprocess_t *mgprocess_init(int length, char base, char *message)
{
    int i = -1;
    mgprocess_t *process = (mgprocess_t *)malloc(sizeof(mgprocess_t));
    if (process)
    {
        process->length = length;
        //process->base = base;
        process->bar = (char *)malloc(sizeof(char) * length + 1);
        if (process->bar)
        {
            for(i = 0; i < length; i++)
            {
                process->bar[i] = base;
            }
            process->bar[i] = '\0';
        }
        else
        {
            free(process);
            return NULL;
        }
        process->message = strdup(message);
    }
    return process;
}

void mgprocess_out_rate(mgprocess_t *process, int rate)
{
    int out_len = 0; //(length * rate / 100);
    if (process)
    {
        out_len = (process->length * rate/100);
        if (process->message)
            printf("%s:", process->message);
        if (process->bar)
            printf("[%s]", (process->bar + (process->length - out_len)));
    }
    printf("%d%%\r", rate);
    fflush(stdout);
    if (rate == 100)
    {
        printf("\n");
    }
}

void mgprocess_finish(mgprocess_t *process)
{
    if (process)
    {
        if (process->message)
        {
            free(process->message);
            process->message = NULL;
        }
        if (process->bar)
        {
            free(process->bar);
            process->bar = NULL;
        }
        free(process);
        process = NULL;
    }
}

#ifdef _MG_PROCESS_RATE_TEST_

void main()
{
    mgprocess_t *p1 = mgprocess_init(100, '#', "test1");
    mgprocess_t *p2 = mgprocess_init(20, '*', "test2");
    int i = 0;
    for ( i = 0; i < 101; i++)
    {
        mgprocess_out_rate(p1, i);
        usleep(100000);
    }
    for (i = 0; i < 101; i+=10)
    {
        mgprocess_out_rate(p2, i);
        usleep(1000000);
    }
    mgprocess_finish(p1);
    mgprocess_finish(p2);
    printf("over!\n");
    return;
}

#endif

