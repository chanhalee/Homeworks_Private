#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

int     analyseFile(int indexFrom, int indexTo, int findFrom, int findTo);
int     threadMain(int *args);
int     convertString(char *buffer);
char    *convertNum(int num);
void	myStrncpy(char *dest, char *cpy, int num);

static  int suitQuantity = 0;
static  int fd_read;
static  int fd_write;
static  int terminatedThreadNum = 0;
static	int NUMS;
static	pthread_mutex_t lock_read = PTHREAD_MUTEX_INITIALIZER;
static	pthread_mutex_t lock_write = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	sigset_t    mask;
	sigset_t    prev;
	int         findFrom;
	int         findTo;
	int         findGap;
	int         indexFrom;
	int         indexTo;
	int         threadNum;
	int         threadCounter;
	pthread_t   tid;
	int         result;
	int         threadArgs[4];
	char        *buf = (char *)malloc(7*sizeof(char));
	char		*buffer = buf;
	int         len;

	if(argc != 6)
		return(0);
	findFrom = atoi(argv[1]);
	findTo = atoi(argv[2]);
	threadNum = atoi(argv[3]);
	if(findTo < findFrom || threadNum < 1)
	{
		return(0);
	}
	fd_read = open(argv[4], O_RDONLY);
	fd_write = creat(argv[5], 0777);
	lseek(fd_write, 6, SEEK_SET);

	len = 6;
	while (len){
		int ret = pread(fd_read, buffer, 6, 0);
		if (ret < 0){
			if (errno == EINTR)
				continue ;
			perror("read");
			close(fd_read);
			close(fd_write);
			if(buf)
				free(buf);
			exit(1);
		}
		len -= ret;
		while (ret)
		{
			ret--;
			buffer++;
		}
	}
	buffer -= 6;
	NUMS = convertString(buffer);
	findGap = NUMS / threadNum;
	indexFrom = (threadNum - 1) * findGap;
	indexTo = NUMS-1;
	threadCounter = 0;
	result = 0;
	while(++threadCounter < threadNum)
	{
		indexFrom = (threadCounter -1) * findGap;
		indexTo = threadCounter * findGap -1;
		threadArgs[0] = indexFrom+1;
		threadArgs[1] = indexTo+1;
		threadArgs[2] = findFrom;
		threadArgs[3] = findTo;
		pthread_create(&tid, NULL, (void *)threadMain, (void *)threadArgs);
		pthread_detach(tid);
	}	
	indexFrom = (threadNum - 1) * findGap;
	indexTo = NUMS-1;
	suitQuantity += analyseFile(indexFrom+1, indexTo+1, findFrom, findTo);
	while(terminatedThreadNum+1 != threadCounter)
		sleep(1);
	printf("%d", suitQuantity);
	char *temp = convertNum(suitQuantity);
	pwrite(fd_write, temp, 6, 0);
	if(temp)
		free(temp);
	if(buf)
		free(buf);
	close(fd_read);
	close(fd_write);
	return (0);
}

int threadMain(int *args)
{
	suitQuantity += analyseFile(args[0], args[1], args[2], args[3]);
	terminatedThreadNum++;
	return (0);
}

int analyseFile(int indexFrom, int indexTo, int findFrom, int findTo)
{
	char	*rBuf = (char *)malloc(7 * sizeof(char));
	char	*rBuffer = rBuf;
	char	*wBuf = (char *)malloc(121 * sizeof(char));
	char	*wBuffer = wBuf;
	int		localIndex;
	int		len;
	int		ret;
	int		convertedNum;
	int		counter;

	counter = 0;
	rBuf[6] = '\0';
	wBuf[0] = '\0';
	wBuf[120] = '\0';
	if(indexFrom < 0 || indexTo >= NUMS+1 || indexFrom > indexTo)
		return (0);
	localIndex = indexFrom -1;
	pthread_mutex_lock(&lock_read);	//Mutex lock read
	while(++localIndex <= indexTo)
	{
		len = 6;
		while (len){
			ret = pread(fd_read, rBuffer, 6, 6*localIndex);
			if (ret < 0){
				if (errno == EINTR)
					continue ;
				perror("read");
				close(fd_read);
				close(fd_write);
				if(rBuf)
					free(rBuf);
				if(wBuf)
					free(wBuf);
				exit(1);
			}
			len -= ret;
			rBuffer += ret;
		}
		rBuffer = rBuf;
		convertedNum = convertString(rBuffer);
		if(convertedNum >= findFrom && convertedNum <= findTo)
		{
			myStrncpy(wBuffer,rBuf,6);
			wBuffer += 6;
			counter++;
			if((counter % 20) == 0||localIndex == indexTo){
				pthread_mutex_unlock(&lock_read);	//Mutex unlock read
				pthread_mutex_lock(&lock_write);	//Mutex lock write
				wBuffer = wBuf;
				write(fd_write, wBuffer, 6*((counter % 20)?counter%20:20));		
				pthread_mutex_unlock(&lock_write);	//Mutex unlock write
				pthread_mutex_lock(&lock_read);	//Mutex lock read
				wBuf[0] = '\0';
				wBuffer = wBuf;
			}
		}
	}
	pthread_mutex_unlock(&lock_read);	//Mutex unlock read
	if(rBuf)
		free(rBuf);
	if(wBuf)
		free(wBuf);
	return (counter);
}

int	convertString(char *buffer)
{
	int	result;
	int	index;

	result = 0;
	index = -1;
	while (++index < 5)
	{
		if(buffer[index] == ' ')
			continue ;
		result = result * 10 + (int)(buffer[index] - '0');
	}
	return (result);
}

char    *convertNum(int num)
{
	int     index = 5;
	char    *result = (char *)malloc(6*sizeof(char));
	result[0] = ' ';
	result[1] = ' ';
	result[2] = ' ';
	result[3] = ' ';
	result[4] = ' ';
	result[5] = '\n';

	while (num > 0)
	{
		result[--index] = (char)(num % 10 + '0');
		num /= 10;
	}
	return (result);
}
void	myStrncpy(char *dest, char *cpy, int num)
{
	int index = -1;

	while (++index < num)
		dest[index] = cpy[index];
}
