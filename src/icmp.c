#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

#include "mainloop.h"
#define BUF_SIZE 1024
#define PACK_LEN 56

static uint64_t diftime(const struct timeval* end, const struct timeval* bg) 
{
  return (end->tv_sec - bg->tv_sec) * 1000 * 1000 + (end->tv_usec - bg->tv_usec);
}

static uint16_t checksum(uint16_t *data, int len)
{
	uint32_t ret = 0;
	while(len > 1) {
		ret += *data ++;
		len -= sizeof(uint16_t);
	}

	if (len == 1) {
		ret += *(uint8_t *)data;
	}

	ret = (ret >> 16) + (ret & 0xFFFF);
	ret += (ret >> 16);
	return (uint16_t)~ret;
}

int icmp_pack(int num, uint8_t *buf, int size)					//填装报文信息
{
	if (size < PACK_LEN + 8) {
		return -1;
	}

    memset(buf, 0x00, size);
    struct icmp* p = (struct icmp*)buf;
    p->icmp_type = ICMP_ECHO;
    p->icmp_code = 0;
    p->icmp_cksum = 0;
    p->icmp_seq = num;
    p->icmp_id = getpid();
    struct timeval tval;
    gettimeofday(&tval, NULL);
    memcpy((void*)p->icmp_data, (void*)&tval, sizeof(tval));

    p->icmp_cksum = checksum((unsigned short*)buf, PACK_LEN + 8); //ICMP报头有8个字节
    return PACK_LEN + 8;
}

uint64_t icmp_unpack(char* buf)
{
    struct ip* pip = (struct ip*)buf;
    struct icmp* picmp = (struct icmp*)(buf+ (pip->ip_hl << 2));
	if (picmp->icmp_id != getpid()) {
		return -1;
	}
    struct timeval end;
    gettimeofday(&end, NULL);
    return diftime(&end, (struct timeval*)(picmp->icmp_data));
}

int ping_send(int fd, struct sockaddr_in *dst, int num)
{
	char buf[BUF_SIZE] = {0x00};
	int len = icmp_pack(num, buf, sizeof(buf));
	if ( len < 0) {
		fprintf(stderr, "ICMP icmp_pack error.\n");
		return -1;
	}
	return sendto(fd, buf, len, 0, (struct sockaddr*)dst, sizeof(struct sockaddr_in));
}

int ping_recv(int fd, int *seq, uint64_t *diff)
{
	struct sockaddr_in from = {0x00};
	uint8_t buf[BUF_SIZE] = {0x00};
    socklen_t len = sizeof(from);

    ssize_t ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&from, &len);
    if (ret < 0) {
        perror("recvfrom");
        return -1;
    }

	struct ip* pip = (struct ip*)buf;
    struct icmp* picmp = (struct icmp*)(buf+ (pip->ip_hl << 2));
	if (picmp->icmp_id != getpid()) {
		return -1;
	}

    struct timeval end;
    gettimeofday(&end, NULL);
    if (diff) {
        *diff = diftime(&end, (struct timeval*)(picmp->icmp_data));
    }

    if (seq) {
        *seq = picmp->icmp_seq;
    }

    return 0;
}
