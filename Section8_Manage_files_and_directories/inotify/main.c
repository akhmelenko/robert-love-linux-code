
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // open()
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#include <sys/inotify.h>

// #include <sys/inotify.h>

// int inotify_init(void);
// - Initializes a new inotify instance and returns a
// file descriptor associated with a new inotify event queue.
// - RETURN: On success, these system calls return a new file descriptor.  On
// error, -1 is returned, and errno is set to indicate the erro

// int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
// adds a new watch, or modifies an existing
// watch, for the file whose location is specified in pathname
// returns a watch descriptor (a nonnegative integer).
// On error, -1 is returned and errno is set to indicate the error

// struct inotify_event {
//        int      wd;       /* Watch descriptor */
//        uint32_t mask;     /* Mask describing event */
//        uint32_t cookie;   /* Unique cookie associating related
//                              events (for rename(2)) */
//        uint32_t len;      /* Size of name field */
//        char     name[];   /* Optional null-terminated name */
//    };

// int inotify_rm_watch(int fd, int wd);
// - Removes the watch associated with the watch
// descriptor wd from the inotify instance associated with the file
// descriptor fd
// - On success, inotify_rm_watch() returns zero.
// On error, -1 is returned and errno is set to indicate the error

void MaskDecode(uint32_t mask)
{
    if (mask & IN_ACCESS)
        printf("IN_ACCESS\n");
    if (mask & IN_MODIFY)
        printf("IN_MODIFY\n");
    if (mask & IN_ATTRIB)
        printf("IN_ATTRIB\n");
    if (mask & IN_CLOSE_WRITE)
        printf("IN_CLOSE_WRITE\n");
    if (mask & IN_CLOSE_NOWRITE)
        printf("IN_CLOSE_NOWRITE\n");
    if (mask & IN_CLOSE)
        printf("IN_CLOSE\n");
    if (mask & IN_OPEN)
        printf("IN_OPEN\n");
    if (mask & IN_MOVED_FROM)
        printf("IN_MOVED_FROM\n");
    if (mask & IN_MOVED_TO)
        printf("IN_MOVED_TO\n");
    if (mask & IN_MOVE)
        printf("IN_MOVE\n");
    if (mask & IN_CREATE)
        printf("IN_CREATE\n");
    if (mask & IN_DELETE)
        printf("IN_ACCESS\n");
    if (mask & IN_DELETE_SELF)
        printf("IN_DELETE\n");
    if (mask & IN_MOVE_SELF)
        printf("IN_MOVE_SELF\n");
    /* Events sent by the kernel.  */
    if (mask & IN_UNMOUNT)
        printf("IN_UNMOUNT\n");
    if (mask & IN_Q_OVERFLOW)
        printf("IN_ACCESS\n");
    if (mask & IN_IGNORED)
        printf("IN_IGNORED\n");
    /* Special flags.  */
    if (mask & IN_ONLYDIR)
        printf("IN_ONLYDIR\n");
    if (mask & IN_DONT_FOLLOW)
        printf("IN_DONT_FOLLOW\n");
    if (mask & IN_EXCL_UNLINK)
        printf("IN_EXCL_UNLINK\n");
    if (mask & IN_MASK_CREATE)
        printf("IN_MASK_CREATE\n");
    if (mask & IN_MASK_ADD)
        printf("IN_MASK_ADD\n");
    if (mask & IN_ISDIR)
        printf("IN_ISDIR\n");
    if (mask & IN_ONESHOT)
        printf("IN_ONESHOT\n");
}

int main()
{
    // inotify init
    int fd = inotify_init();
    if (fd == -1)
    {
        fprintf(stderr, "initify_init() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // set watchdogs
    int wd = inotify_add_watch(fd, ".", IN_ALL_EVENTS);
    if (wd == -1)
    {
        fprintf(stderr, "inotify_add_watch() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("\n");
        // read events
        char buf[1024] __attribute__((aligned(4)));
        ssize_t len = read(fd, buf, sizeof(buf));
        if (len == -1)
        {
            fprintf(stderr, "read() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (i < len)
        {
            struct inotify_event *event = (struct inotify_event *)&buf[i];
            printf("wd=%d mask=%x cookie=%d len=%d dir=%s\n",
                   event->wd,
                   event->mask,
                   event->cookie,
                   event->len,
                   (event->mask & IN_ISDIR) ? "dir" : "file");
            MaskDecode(event->mask);
            if (event->len)
                printf("name=%s\n", event->name);
            i += sizeof(struct inotify_event) + event->len;
        }

        // check events queue length
        int queueLen;
        if (ioctl(fd, FIONREAD, &queueLen) == -1)
        {
            fprintf(stderr, "ioctl() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        printf("event queue len=%d\n", queueLen);
    }

    // clear watchdogs
    if (inotify_rm_watch(fd, wd) == -1)
    {
        fprintf(stderr, "inotify_rm_watch() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1)
    {
        fprintf(stderr, "close() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}