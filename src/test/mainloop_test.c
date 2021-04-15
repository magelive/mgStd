#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

#include "mainloop.h"

int main()
{
    struct timeval tv = {0x00};
    gettimeofday(&tv, NULL);
    int ret = mainloop_init();
    if (ret < 0) {
        printf("mainloop_init error(%d): %s\n", ret, strerror(-ret));
        return -1;
    }
    mainloop_add_timeout(500, timer_process, NULL, &tv);
    mainloop_add_timeout(1000, timer_process, NULL, &tv);
    mainloop_run();
    printf("mainloop over.\n\n");
    return 0;
}
