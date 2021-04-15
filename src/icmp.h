#ifndef ICMP_H
#define ICMP_H
#include <stdint.h>
#include <netinet/in.h>

int icmp_pack(int num, uint8_t *buf, int size);
uint64_t icmp_unpack(char* buf);

int ping_send(int fd, struct sockaddr_in *dst, int num);
int ping_recv(int fd, int *seq, uint64_t *diff);

#endif