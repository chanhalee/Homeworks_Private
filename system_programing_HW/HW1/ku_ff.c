#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#ifndef REDO
#include "ku_ps_input.h"
#endif

#include <sys/msg.h> 
#include <sys/ipc.h> 
#include <sys/types.h>

int getMessage();
int sendMessage(int msg);
int analyseArr(int indexFrom, int indexTo, int findFrom, int findTo);

typedef struct{
	long id;
	int number;
}	t_mymsg;

int main(int argc, char **argv)
{
	sigset_t	mask;
	sigset_t	prev;
	int			findFrom;
	int			findTo;
	int			findGap;
	int 		indexFrom;
    int 		indexTo;
	int 		forkNum;
	int 		forkCounter;
	pid_t		pid;
	int			result;

	if(argc != 4)
		return(0);
	findFrom = atoi(argv[1]);
	findTo = atoi(argv[2]);
	forkNum = atoi(argv[3]);
	if(findTo < findFrom || forkNum < 1)
	{
		return(0);
	}
	findGap = NUMS / forkNum;
	forkCounter = 0;
	indexFrom = (forkNum - 1) * findGap;
	indexTo = NUMS-1;
	result = 0;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);

	while(++forkCounter < forkNum)
	{
		sigprocmask(SIG_BLOCK, &mask, &prev);
		if((pid = fork()) == 0)
		{
			indexFrom = (forkCounter -1) * findGap;
			indexTo = forkCounter * findGap -1;
			sendMessage(analyseArr(indexFrom, indexTo, findFrom, findTo));
			exit(0);
		}
		while(!pid){
			sigsuspend(&prev);
		}
		result += getMessage();
		sigprocmask(SIG_SETMASK, &prev, &mask);
	}
	result += analyseArr(indexFrom, indexTo, findFrom, findTo);
	printf("%d", result);
	return (0);
}

int analyseArr(int indexFrom, int indexTo, int findFrom, int findTo)
{
	int	localIndex;
	int	counter;

	counter = 0;
	if(indexFrom < 0 || indexTo >= NUMS || indexFrom > indexTo)
		return (0);
	localIndex = indexFrom -1;
	while(++localIndex <= indexTo)
	{
		if(input[localIndex] >= findFrom && input[localIndex] <= findTo)
		{
			counter++;
		}
	}
	return counter;
}

int sendMessage(int msg)
{
	key_t	ipckey;
	t_mymsg	mymsg;
	int		mqdes;
	size_t	buf_len;

	buf_len = sizeof(mymsg.number);
	ipckey = ftok("./tmp/ftfffoo", 1788);
	mqdes = msgget(ipckey, IPC_CREAT|0600);
	if(mqdes < 0){
		perror("msgget()");
		exit(0);
	}
	mymsg.id = 8711;
	mymsg.number = msg;

	if(msgsnd(mqdes, &mymsg, buf_len, 0) == -1){
		perror("msgsnd()");
		exit(0);
	}
	return (0);
}

int getMessage()
{
	key_t	ipckey;
	t_mymsg	mymsg;
	int		mqdes;
	int		i;
	size_t	buf_len;

	buf_len = sizeof(mymsg.number);
	ipckey = ftok("./tmp/ftfffoo", 1788);
	mqdes = msgget(ipckey, IPC_CREAT|0600);
	if(mqdes < 0){
		perror("msgget()");
		exit(0);
	}
	if(msgrcv(mqdes, &mymsg, buf_len, 8711, 0) == -1){
		perror("msgrcv()");
		exit(0);
	}
	return (mymsg.number);
}

