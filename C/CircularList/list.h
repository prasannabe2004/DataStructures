#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct SquidMsgQueue
{
    struct SquidMsgQueue *pNext;
    struct SquidMsgQueue *pPrev;
    char logBuffer [BUFSIZ + 1];
    int32_t receiveSize;
} tSquidMsgQueue;

#define D_BUFFER_SIZE 127
#define D_DEFAULT_SQUID_LOG "/var/log/auth.log"
#define D_SM_RAW_DATA "/tmp/raw.data"
#define D_MY_PID_FILE "/var/run/mylist.pid"
#define D_FILE_NAME_SIZE 255

#define M_FREEZ(x) {\
	free (x);\
	x=NULL;\
}

#endif // LIST_H_INCLUDED
