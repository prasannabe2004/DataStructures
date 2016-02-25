#include "list.h"

tSquidMsgQueue *gMsgQHead = NULL;
tSquidMsgQueue *gMsgQTail = NULL;
pthread_mutex_t msgQueueProtect;
pthread_cond_t msgQueueWait;
pthread_t gWorkerThreadId;
char i8FileInPath [D_FILE_NAME_SIZE + 1] = {0};
char i8FileOutPath [D_FILE_NAME_SIZE + 1] = {0};

static int32_t smInitMsgQueue (void)
{
    gMsgQHead = (tSquidMsgQueue *) malloc (sizeof (tSquidMsgQueue));

    if (NULL == gMsgQHead)
    {
         syslog (LOG_CRIT, "Failed to init MsgQHead: %s\n", strerror (errno));
         return -1;
    }

    gMsgQTail = (tSquidMsgQueue *) malloc (sizeof (tSquidMsgQueue));

    if (NULL == gMsgQTail)
    {
         syslog (LOG_CRIT, "Failed to init MsgQTail: %s\n", strerror (errno));
         return -1;
    }

    gMsgQHead->pNext = gMsgQTail;
    gMsgQHead->pPrev = NULL;
    gMsgQTail->pPrev = gMsgQHead;
    gMsgQTail->pNext = NULL;

    return 0;
}

static int32_t smFinitMsgQueue (void)
{
    tSquidMsgQueue *pTempNode = NULL;

    if (NULL == gMsgQHead)
    {
        return 0;
    }

    if (gMsgQHead->pNext == gMsgQTail)
    {
        M_FREEZ(gMsgQHead);
        M_FREEZ(gMsgQTail);
    }
    else
    {
        for (;gMsgQHead;)
        {
            pTempNode = gMsgQHead->pNext;
            M_FREEZ (gMsgQHead);
            gMsgQHead = pTempNode;
        }
    }

    return 0;
}

static int32_t smMsgEnqueue (const char *pLogMsg, int32_t receiveSize)
{
	tSquidMsgQueue *pNewNode = NULL;
    syslog(LOG_WARNING, "Queuing %s of size %d\n",pLogMsg, receiveSize);
	pNewNode = (tSquidMsgQueue *) malloc (sizeof (tSquidMsgQueue));

	if (NULL == pNewNode)
	{
		syslog (LOG_CRIT, "Failed to alloc mem for Msg: %s\n", strerror (errno));
		return -1;
	}

	memcpy (pNewNode->logBuffer, pLogMsg, receiveSize);

	pNewNode->receiveSize = receiveSize;

	pthread_mutex_lock(&msgQueueProtect);

	gMsgQHead->pNext->pPrev = pNewNode;
	pNewNode->pPrev = gMsgQHead;
	pNewNode->pNext = gMsgQHead->pNext;
	gMsgQHead->pNext = pNewNode;

	pthread_cond_signal (&msgQueueWait);
	pthread_mutex_unlock(&msgQueueProtect);

	return 0;
}

static tSquidMsgQueue *smMsgDequeue (void)
{
    tSquidMsgQueue *pNode = NULL;

    pthread_mutex_lock(&msgQueueProtect);
    if (gMsgQTail && gMsgQTail->pPrev != gMsgQHead)
    {
        pNode = gMsgQTail->pPrev;

        gMsgQTail->pPrev = pNode->pPrev;
        pNode->pPrev->pNext = gMsgQTail;
    }
    pthread_mutex_unlock(&msgQueueProtect);

    return pNode;
}

static void printBuffer (int32_t i32SurgeFile, char *pBuffer, int32_t receiveSize)
{

	write (i32SurgeFile, pBuffer, receiveSize);
	//write (1, pBuffer, receiveSize);

	sync ();

	return;
}

static void* smWorkerThread (void *pArg)
{
    char* pMsgBuffer;
    int32_t receiveSize;
    tSquidMsgQueue* pMsg = NULL;
    int32_t i32SurgeFile = -1;

    i32SurgeFile = open (i8FileOutPath, O_WRONLY|O_CREAT|O_DSYNC);

    if (-1 == i32SurgeFile)
    {
	    syslog (LOG_CRIT, "Unable to write raw data to file '%s': %s\n", D_SM_RAW_DATA, strerror (errno));
	    return NULL;
    }

    while (1)
    {
        pMsg = smMsgDequeue();

        if (pMsg)
        {
            pMsgBuffer = pMsg->logBuffer;
            receiveSize = pMsg->receiveSize;
            syslog(LOG_WARNING, "Dequeuing %s of size %d\n",pMsgBuffer, receiveSize);
        }
        else
        {
            pthread_mutex_lock (&msgQueueProtect);
            pthread_cond_wait (&msgQueueWait, &msgQueueProtect);

            pthread_mutex_unlock (&msgQueueProtect);

            continue;
        }

        printBuffer (i32SurgeFile, pMsgBuffer, receiveSize);

        M_FREEZ(pMsg);
    }

    return NULL;
}



int32_t processBuffer (char *pBuffer, int32_t size)
{
	return smMsgEnqueue (pBuffer, size);
}

void cleanUpProcess (int32_t sigNo)
{
	syslog (LOG_WARNING, "Signal '%d' received. Cleaning up process...", sigNo);

	smFinitMsgQueue ();


	if (gWorkerThreadId)
	{
		pthread_cancel (gWorkerThreadId);
		pthread_join (gWorkerThreadId, NULL);
	}

	pthread_mutex_destroy (&msgQueueProtect);
	pthread_cond_destroy (&msgQueueWait);

	syslog (LOG_WARNING, "Gracious exit... Bye!");

	exit (EXIT_SUCCESS);
}

int32_t initialize (void)
{
	int32_t retVal = -1;

	pthread_mutex_init (&msgQueueProtect, NULL);
	pthread_cond_init (&msgQueueWait, NULL);

	if (0 != (retVal = smInitMsgQueue()))
	{
		return retVal;
	}

	retVal = pthread_create (&gWorkerThreadId, PTHREAD_CREATE_JOINABLE, smWorkerThread, NULL);

	if (0 != retVal)
	{
		syslog (LOG_CRIT, "Failed to create QoS Worker thread: %s\n", strerror (errno));
		return -1;
	}

	syslog (LOG_INFO, "Worker thread init done: %d\n", (int32_t)gWorkerThreadId);

	return 0;

}

void daemonize()
{
    FILE* FP;
    int i32MyPid = -1;

	FP = fopen (D_MY_PID_FILE, "r");

	if (FP)
	{
		fscanf (FP, "%d", &i32MyPid);

		if (0 == kill (i32MyPid, 0))
		{
			syslog (LOG_WARNING, "An instance is already running. Good Bye!");
			exit (EXIT_SUCCESS);
		}
		else
		{
			unlink (D_MY_PID_FILE);
		}

		fclose (FP);
	}
    i32MyPid = fork();

	switch(i32MyPid)
	{
		case 0:
			/* Child */
			printf("Child PID is %d\n",getpid());
			umask(0);
			if(-1 == setsid())
			{
				perror("setsid:");
				exit(EXIT_FAILURE);
			}
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			break;
		case -1:
			/* Failure */
			perror("Fork:");
			exit(EXIT_FAILURE);
		default:
			/* Parent */
			printf("Parent PID is %d\n",getpid());
			exit(EXIT_SUCCESS);
	}

}

int main (int argc, char **argv)
{

	char i8ReadBuf [BUFSIZ + 1] = {0};
	int32_t i32FileDes = -1;
	int32_t i32Current = -1;
	int32_t i32BytesRead = 0;

	int c = 0, nodaemonize = 0, inFile = -1, outFile = -1;
	extern char *optarg;

	while (-1 != (c = getopt (argc, argv, "i:o:fh")))
	{
		switch (c)
		{
			case 'i':
				printf("Option '%c' selected with argument %s\n",c,optarg);
				inFile = 1;
				strncpy (i8FileInPath, optarg, D_FILE_NAME_SIZE);
				break;
			case 'o':
				printf("Option '%c' selected with argument %s\n",c,optarg);
				outFile = 1;
				strncpy (i8FileOutPath, optarg, D_FILE_NAME_SIZE);
				break;
			case 'f':
				printf("Option '%c' selected with argument %s\n",c,optarg);
				nodaemonize = 1;
				break;
			default:
			case 'h':
				//print_usage (argv[0]);
				exit (EXIT_SUCCESS);
				break;
		}
	}
	if(!nodaemonize)
        daemonize();
    if(inFile)
        strncpy (i8FileInPath, D_DEFAULT_SQUID_LOG, D_FILE_NAME_SIZE);
    if(outFile)
        strncpy (i8FileOutPath, D_SM_RAW_DATA, D_FILE_NAME_SIZE);

	signal (SIGINT, cleanUpProcess);
	signal (SIGTERM, cleanUpProcess);
	signal (SIGUSR1, cleanUpProcess);

	i32FileDes = open (i8FileInPath, O_RDONLY);

	if (-1 == i32FileDes)
	{
		syslog (LOG_CRIT, "Unable to open file '%s'. Exiting %s...\n", i8FileInPath, argv[0]);
		exit (EXIT_FAILURE);
	}

	if (initialize ())
	{
		syslog (LOG_CRIT, "%s initialization failed. Exiting...\n", argv[0]);
		exit (EXIT_FAILURE);
	}

	i32Current = lseek (i32FileDes, 0, SEEK_END);

	while (1)
	{
		int32_t i32HowMuchCanIRead = BUFSIZ;
		int32_t i32HowMuchIHaveRead = 0;
		char *pBuffer = i8ReadBuf;

		sleep (1);

		while (i32HowMuchCanIRead)
		{

			do
			{
				i32BytesRead = read (i32FileDes, pBuffer, BUFSIZ);
			} while (i32BytesRead < 0 && errno == EINTR);

			if (i32BytesRead < 0)
			{
				if (i32HowMuchCanIRead)
				{
					processBuffer (i8ReadBuf, i32HowMuchCanIRead);
				}
				else
				{
					break;
				}
			}

			if (0 == i32BytesRead)
			{
				processBuffer (i8ReadBuf, i32BytesRead);
				break;
			}

			pBuffer = ((char *)pBuffer) + i32BytesRead;

			i32HowMuchIHaveRead += i32BytesRead;
			i32HowMuchCanIRead -= i32BytesRead;
		}

		processBuffer (i8ReadBuf, i32HowMuchIHaveRead);
	}

	return 0;
}

