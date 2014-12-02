

#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "mgtime.h"

void mg_get_string_time(char *buf, int buf_len)
{
    if (buf == NULL)
        return;
    time_t tm;
    time(&tm);
    struct tm *tf = localtime( &tm );
    snprintf( buf, buf_len, "%04d-%02d-%02d %02d:%02d:%02d",
            1900+tf->tm_year, 1+tf->tm_mon, tf->tm_mday,
            tf->tm_hour, tf->tm_min, tf->tm_sec );
    return;
}


uint64_t mg_get_tick_time_ms()
{
    uint64_t time=0;
    struct timeval now;
    gettimeofday( &now , NULL );

    time = now.tv_sec;
    time = time*1000000;
    time += now.tv_usec;
    return time/1000;
}


void mg_get_rand_str(char *s,int number)
{
    if (s==NULL)
        s = malloc(number+1);
    bzero(s, number+1);
    char str[64] = "00123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int i;
    srand(mg_get_tick_time_ms());
    for(i=0;i<number;i++)
    {
        s[i]=str[(rand()%62)+1];
    }
}

int get_rand_num(int t1, int t2)
{
    int max = t1>t2?t1:t2;
    int min = t1<t2?t1:t2;
    int num = 0;
    srand(mg_get_tick_time_ms());
    num = rand()%(max-min)+min;
    return num;
}
