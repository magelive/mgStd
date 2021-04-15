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
#include "icmp.h"

typedef struct {
	int fd;
	struct sockaddr_in addr;
	int times;
}icmp_data_t;

void ping_recv_process(int fd, events_t events, void *user_data)
{
	icmp_data_t *idata = (icmp_data_t *)user_data;

	if (events & EVENT_ERR) {
		mainloop_quit();
		return;
	}
	int seq = 0;
	uint64_t diff = (uint64_t)-1;
	if (ping_recv(fd, &seq, &diff) == 0) {
		fprintf(stdout, "The icmp packet num %d diff time is: %.2f ms\n", seq, diff/1000.0);
	} else {
		fprintf(stderr, "ping recv error.\n");
	}
	
	if (idata->times++ > 100) {
		mainloop_quit();
		return;
	}
	ping_send(idata->fd, &idata->addr, idata->times);
	return;
}

int main(int argc, char *argv[])
{
	icmp_data_t *idata = malloc(sizeof(icmp_data_t));
	if (!idata) {
		fprintf(stderr, "OOM at main.\n");
		return -1;
	}
	bzero(idata, sizeof(icmp_data_t));
	idata->fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (idata->fd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno));
		free(idata);
		return -1;
	}
	idata->addr.sin_family = AF_INET;
	idata->addr.sin_addr.s_addr = inet_addr(argv[1]);

	mainloop_init();
	mainloop_add_fd(idata->fd, EVENT_IN|EVENT_ERR|EVENT_OUT, ping_recv_process, NULL, idata);

	int ret = ping_send(idata->fd, &idata->addr, idata->times);
	if (ret < 0) {
		printf("ret = %d, %s\n", ret, strerror(errno));
	}

	mainloop_run();
	close(idata->fd);
	free(idata);
	return 0;
}