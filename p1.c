//As of now, we can read all the file input by the specific forked processes
//Still need to add errors for when file does not match desired format
//This is a huge step forward, however
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
	
	int linesToPass = 1;
	
	pid_t pid;
	for (i = 0; i < childCount; i++)
	{
		printf("Now let's shift over for the next kiddo \n");
		char c;
		int j;
		for (j = 0; j < linesToPass; j++) { //we must pass over what the last child passed over so next child doesn't repeat work
			do
				c = fgetc(fp);
			while (c != '\n');
		}
		linesToPass = 2;
		
		
		pid = fork();
		if (pid > 0) {
			printf("Don't look at me I'm only the parent\n");
			wait(NULL);
			continue; //I am the parent, create more children
		}
		else if (pid == 0) {
			printf("This is a child with id %d\n", getpid());
			int sectionTotal;
			fscanf(fp, "%1d", &sectionTotal);
			printf("The first number in this section is %d\n", sectionTotal);
			printf("The remaining %d numbers are: ", sectionTotal);
			int j;
			for (j = 0; j < sectionTotal; j++) {
				int temp;
				fscanf(fp, "%d", &temp);
				printf(" %d ", temp);
			}
			printf("\n");
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
