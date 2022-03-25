#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(int argc, char **argv)
{
	if (argc != 2)
		return 0;
	kill(getpid(), SIGSTOP);
	while (1)
	{
		printf("%c\n", argv[1][0]);
		usleep(200000);
	}
}