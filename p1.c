//I've been working on p1 for several days now
//by coding parts of the project in other test files
//so now I began the process of putting it all together
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
	printf("Welcome to the project\n");
	int i;
	
	
	char inputFileName[] = "input.dat";
	char outputFileName[] = "output.dat";

	FILE *fp;
	fp = fopen(inputFileName, "r");
	if (fp == NULL)
	{
		perror("Failed to locate file.");
		exit(1);
	}
	printf("Opened file %s\n", inputFileName);

	int childCount;
	
	fscanf(fp, "%1d", &childCount);

	printf("The first number is our file is %d\n", childCount);
	printf("Meaning we need to fork %d children to handle the file\n", childCount);
	
	
	pid_t pid;
	for (i = 0; i < childCount; i++)
	{
			pid = fork();
			if (pid > 0) {
				printf("Don't look at me I'm only the parent\n");
				wait(NULL);
				continue; //I am the parent, create more children
			}
			else if (pid == 0) {
				printf("This is a child with id %d\n", getpid());
				exit(0);
				break; //I am the child, get to work
			}
			else {
				perror("Error while forking");
				exit(EXIT_FAILURE);
			}
			
		
	}
	
	printf("And now we're done\n");
	
	return 0;
}
