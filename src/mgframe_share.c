#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>

#define ALIGN_TO_4K(size)  (((size) + 0xfff) & ~0xfff)

//static int _g_frame_index = 0;

typedef struct frame {
    int fd;
    int index;
    int size;
    int flags;
    //int pending[4];
    void *addr;
}frame_t;

void frame_destroy(frame_t *frame);


int frame_create(frame_t *f, int index, int flags, int size, void *data) 
{
    int fd = -1; 
    void *addr = NULL;
    #if 0
    frame_t *frame = (frame_t *)malloc(sizeof(frame_t));
    if (!frame) {
        return NULL;
    }
    #endif
    f->fd = -1;
    f->index = index;
    f->size = size;
    f->flags = flags;

    char shm_n[64] = {0};
    snprintf(shm_n, sizeof(shm_n), "frame_%d", f->index);
    fd = shm_open((const char *)shm_n, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        printf("shm_open %s failed: (%d) %s", shm_n, errno, strerror(errno));
        goto create_error;
    }
    printf("shm_open: %d:%s\n", fd, shm_n);
    ftruncate(fd, ALIGN_TO_4K(size));
    f->fd = fd;

    addr = mmap(NULL, ALIGN_TO_4K(size), PROT_READ | PROT_WRITE, MAP_SHARED , fd, 0);
    if (!addr) {
        printf("mmap failed: %s", strerror(errno));
        goto create_error;
    }
    memcpy(addr, data, size);
    f->addr = addr;

    return 0;

create_error:
    frame_destroy(f);
    return -1;

}

void frame_destroy(frame_t *frame)
{
    printf("frame_destroy i: %d, f: %d\n", frame->index, frame->fd);
    if (!frame) {
        printf("frame is null");
        return;
    }
    if (frame->fd >= 0) {
        //close(frame->fd);
        char shm_n[64] = {0};
        snprintf(shm_n, sizeof(shm_n), "/tmp/frame_%d", frame->index);
        shm_unlink(shm_n);
        close(frame->fd);
        frame->fd = -1;
        printf("frame destroy: %d\n", frame->index);
    }
    if (frame->addr) {
        munmap(frame->addr, ALIGN_TO_4K(frame->size));
    }
    memset(frame, 0, sizeof(frame_t));
    frame->fd = -1;
    //free(frame);
    return;
}

int send_frame(int sockfd, frame_t *frame)
{
    struct msghdr msg = {0};

    struct iovec io = { .iov_base = (void *)frame, .iov_len = sizeof(frame_t) }; // 发送一个字节的标识符
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    // 使用控制消息发送文件描述符
    char buf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    *((int *)CMSG_DATA(cmsg)) = frame->fd;
    printf("send frame fd: %d\n", frame->fd);

    if (sendmsg(sockfd, &msg, 0) == -1) {
        printf("sendmsg %d error: %s\n", sockfd, strerror(errno));
        return -1;
    }
    //frame_destroy(frame);
    return 0;
}

int recv_frame(int sockfd, frame_t *frame)
{
    struct msghdr msg = {0};
    struct iovec io = { .iov_base = (void *)frame, .iov_len = sizeof(frame_t) }; // 接收一个字节的标识符
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char buf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    if (recvmsg(sockfd, &msg, 0) == -1) {
        printf("recvmsg error: %s\n", strerror(errno));
        return -1;
    }

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
        int fd = *((int *)CMSG_DATA(cmsg));
        //frame_t *__frame = (frame_t *)(io.iov_base);

        printf("recv frame fd: %d", fd);
        printf("recv frame size: %d\n", frame->size);
        frame->fd = fd;
        void *addr = mmap(NULL, ALIGN_TO_4K(frame->size), PROT_READ , MAP_SHARED , frame->fd, 0);
        if (addr != MAP_FAILED) {
            //memcpy(frame, __frame, sizeof(frame_t));
            frame->addr = addr;
            printf("recv frame addr: %p\n", frame->addr);
            return 0;
        } else {
            printf("mmap failed: %s", strerror(errno));
            return -1;
        }
    } else {
        printf("recvmsg cmsghdr is't frame.\n");
    }
    return -1;  
}

#define SOCK_PATH  "/tmp/frame_yuv_share"

int unix_server_socket_init()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("socket create failed: %s", strerror(errno));
        return -1;
    }
    struct sockaddr_un addr = {0};

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);
    unlink(SOCK_PATH); // 删除已有的socket
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return fd;
}

int unix_client_socket_init()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("socket create failed: %s\n", strerror(errno));
        return -1;
    }
    struct sockaddr_un addr = {0};

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);
    //unlink(SOCK_PATH); // 删除已有的socket
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("connect unix server error: %s\n", strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
}

//#define SERVER 1


#ifdef SERVER 
#define MAX_CLIENT 10
int clients[MAX_CLIENT];

void *frame_send_thread(void *arg)
{
    while (1) {
        sleep(3);

        frame_t frame = {.fd = -1, .size = 0};

        char buf[1024] = {0};
        snprintf(buf, sizeof(buf), "AAAAAAAAAA send frame index: %d.", _g_frame_index);

        if (frame_create(&frame, 0, strlen(buf) + 1, buf) < 0) {
            printf("frame create failed.\n");
            continue;
        }

        for (int i = 0; i < MAX_CLIENT; i++) {
            if (clients[i] != -1) {
                send_frame(clients[i], &frame);
            }
        }
        frame_destroy(&frame);
    }
}

#endif

int main(int argc, char *argv[])
{
    fd_set fds;
#ifdef SERVER 
    
    int i;
    for ( i = 0; i < MAX_CLIENT; i++) {
        clients[i] = -1;
    }

    pthread_t tid = 0;
    pthread_create(&tid, NULL, frame_send_thread, NULL);

    int sfd = unix_server_socket_init();
    while (1) {
        FD_ZERO(&fds);
        FD_SET(sfd, &fds);
        int maxfd = sfd;
        for (i = 0; i < MAX_CLIENT; i++) {
            if (clients[i] != -1) {
                FD_SET(clients[i], &fds);
                maxfd = maxfd > clients[i] ? maxfd : clients[i];
            }
        }

        int n = select(maxfd + 1, &fds, NULL, NULL, NULL);
        if (n <= 0) {
            if (n == 0)
                printf("select timeout.\n");
            else {
                printf("select error: %s\n", strerror(errno));
            }
            continue;
        }
        if (FD_ISSET(sfd, &fds)) {
            int connfd = accept(sfd, NULL, NULL);
            if (connfd < 0) {
                printf("accept error: %s\n", strerror(errno));
                continue;
            }
            int f = 1;
            for (i = 0; i < MAX_CLIENT; i++) {
                if (clients[i] == -1) {
                    clients[i] = connfd;
                    printf("new client %d:%d\n", i, clients[i]);
                    f = 0;
                    break;
                }
            }
            if (f) {
                printf("connect max than the %d.\n", MAX_CLIENT);
                close(connfd);
            }
        }
        for (i = 0; i < MAX_CLIENT; i++) {
            if (clients[i] != -1 && FD_ISSET(clients[i], &fds)) {
                char buf[1024] = {0};
                int ret = read(clients[i], buf, sizeof(buf));
                if (ret <= 0) {
                    printf("close client %d:%d\n", i, clients[i]);
                    close(clients[i]);
                    clients[i] = -1;
                }
            }
        }
    }

#else
    
    int fd = unix_client_socket_init();
    if (fd < 0) {
        printf("unix client socket init failed.\n");
        exit(0);
    }
    frame_t g_frame = {0};
    while(1) {
        fd_set recvfds;
        FD_ZERO(&recvfds);
        FD_SET(fd, &recvfds);
        int ret = select(fd + 1, &recvfds, NULL, NULL, NULL);
        if (ret <= 0) {
            if (ret == 0)
                printf("select timeout.\n");
            else {
                printf("select error: %s\n", strerror(errno));
            }
            continue;
        }
        frame_t frame;
        ret = recv_frame(fd, &frame);
        if (ret < 0) {
            printf("recv frame error.\n");
            //continue;
            close(fd);
            exit(0);
        }
        printf("recv frame index: %d\n", frame.index);
        printf("recv frame flags: %x\n", frame.flags);
        printf("recv frame size: %d\n", frame.size);
        printf("recv frame addr: %p\n", frame.addr);
        printf("recv frame fd: %d\n", frame.fd);
        printf("recv data: %s\n", (char *)frame.addr);
        frame_destroy(&frame);
    }
#endif

    return 0;
}
