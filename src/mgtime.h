#ifndef __MG_TIME__
#define __MG_TIME__


//获取当前时间，并将其转换为字符型，其格式为 :year-mon-day hour:min:sec
void mg_get_string_time(char *buf, int buf_len);

//获取毫秒时间，time函数返回的为秒
uint64_t mg_get_tick_time_ms();

//获取字符串，字符串范围(数字0~9，字母小写a~z,大写A~Z)
void mg_get_rand_str(char *s,int number);

//获取t1,t2之间的随机数
int get_rand_num(int t1, int t2);

#endif
