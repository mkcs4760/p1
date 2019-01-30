//I've been working on p1 for several days now
//by coding parts of the project in other test files
//so now I began the process of putting it all together
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	printf("Welcome to the project\n");

	pid_t pid;

	pid = fork();

	if (pid < 0)
	{
		perror("Error while forking");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		int j;
		for (j = 0; j < 10; j++)
		{
			printf("child: %d\n", j);
			sleep(1);
		}
		exit(0);
	}
	else
	{
		int i;
		for (i = 0; i < 10; i++)
		{
			printf("parent: %d\n", i);
			sleep(1);
		}
		exit(0);
	}
	return 0;
}
