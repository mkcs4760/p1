//p1 is almost done
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h> //make sure you need all of these...
#include <string.h>
#include <errno.h>

int top = -1;

int pop(int stack[]) {
   int data;
   data = stack[top];
   top = top - 1;   
   return data;
}

void push(int stack[], int data) {
	top = top + 1;
	stack[top] = data;
}

void errorMessage(char programName[100], char errorString[100]){
	printf("ERROR MESSAGE\n");
	printf("%s\n", programName);
	char errorFinal[100];
	//snprintf(errorString, sizeof errorString, "Failed to locate file %s ", inputFileName);
	//snprintf(errorFinal, sizeof errorFinal, "This is just a test\n");
	snprintf(errorFinal, sizeof errorFinal, programName, ": Error: ", errorString);
	perror(errorFinal);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	char inputFileName[] = "input.dat";
	char outputFileName[] = "output.dat";
	
	//this code allows us to print the program name in error messages
	char programName[100];
	strcpy(programName, argv[0]);
	//printf("%s\n", programName);
	if (programName[0] == '.' && programName[1] == '/') {
		memmove(programName, programName + 2, strlen(programName));
	}
	//printf("%s\n", programName);
	//printf("%s\n", argv[0]);
	
	int option;
	while ((option = getopt(argc, argv, "hi:o:")) != -1) {
		switch (option) {
			case 'h' :	printf("Help page for OS_Klein_project1\n");
						printf("Consists of the following:\n\tOne .c file titled p1.c\n\tOne Makefile\n\tOne README.md file\n\tOne version control log.\n");
						printf("The command 'make' will run the makefile and compile the program\n");
						printf("Usage: ./p1 -i <inputFileName> -o <outputFileName> | ./p1 -h\n");
						printf("Version control acomplished using github. Log obtained using command 'git log > versionLog.txt\n");
						exit(0);
						break;
			case 'i' :	strcpy(inputFileName, optarg);
						break;
			case 'o' :	strcpy(outputFileName, optarg);
						break;
			default :	errno = 22; //invalid argument
						errorMessage(programName, "You entered an invalid argument. Please use argument -h to see the help page. ");
						//printf("'%d' is an invalid argument, and therefore will be ignored", option);
						printf("You entered an invalid argument. Please use argument -h to see the help page.");
						//printf("You entered an invalid argument\n");
		}
	}
	
	printf("Welcome to the project\n");

	FILE *input;
	input = fopen(inputFileName, "r");
	FILE *output;
	output = fopen(outputFileName, "w");
	if (input == NULL) {
		char errorString[100];
		snprintf(errorString, sizeof errorString, "Failed to locate file %s ", inputFileName);
		errorMessage(programName, errorString);
		//char errorString[100];
		//snprintf(errorString, sizeof errorString, "Failed to locate file %s ", inputFileName);
		//perror(errorString);
		//exit(EXIT_FAILURE);
	}
	printf("Opened file %s\n", inputFileName);

	int childCount;
	
	fscanf(input, "%d", &childCount);

	int parentPid;
	int listOfPids[childCount];
	int linesToPass = 1; //the child processes don't change the parent's reading position, so we must pass ahead in the parent as well
	
	pid_t pid;
	int i;
	for (i = 0; i < childCount; i++) {
		char c;
		int j;
		for (j = 0; j < linesToPass; j++) { //we must pass over what the last child passed over so next child doesn't repeat work
			do
				c = fgetc(input);
			while (c != '\n');
		}
		linesToPass = 2;
		
		pid = fork();
		if (pid > 0) { //parent case
			parentPid = getpid();
			wait(NULL);
			listOfPids[i] = pid;
			continue; //I am the parent, create more children
		}
		else if (pid == 0) { //child case
			int sectionTotal;
			fscanf(input, "%d", &sectionTotal);
			int j;
			int stack[sectionTotal];
			for (j = 0; j < sectionTotal; j++) {
				int temp;
				fscanf(input, "%d", &temp);
				push(stack, temp);
			}
			fprintf(output, "%d: ", getpid());
			for (j = 0; j < sectionTotal; j++) {
				fprintf(output, " %d ", pop(stack));
			}
			fprintf(output, "\n");
			exit(0);
			break; //I am the child, get to work
		}
		else {
			errorMessage(programName, "Could not create child");
		}
	}
	
	fclose(input);
	
	fprintf(output, "All children were: ");
	for (i = 0; i < childCount; i++) {
		fprintf(output, " %d ", listOfPids[i]);
	}
	fprintf(output, "\nAnd the parent was %d\n", parentPid);
	
	fclose(output);
	
	printf("Results stored in %s\n", outputFileName);
	printf("End of program\n");
	
	return 0;
}
