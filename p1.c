//working on adding input format testing
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h> //make sure you need all of these...
#include <string.h>
#include <errno.h>
#include <ctype.h>

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
	char errorFinal[200];
	sprintf(errorFinal, "%s : Error : %s", programName, errorString);
	//printf("%d\n", errno);
	perror(errorFinal);
	kill(0, SIGKILL);
}

void removeSpaces(char* s) {
	int length = strlen(s);
	char test = s[length - 1];
	if (isspace(test)) {
		s[strlen(s)-1] = '\0'; //remove ending whitespace
	}
}

int readOneNumber(FILE *input, char programName[100]) {
	printf("Checkpoing 4 \n");
	char line[100];
	char *token;
	fgets(line, 100, input);
	printf("Read line equals <%s>", line);
	if (line[0] == '\0') {
		errno = 1;
		errorMessage(programName, "Invalid input file format. Expected more lines then read. ");
		exit(EXIT_FAILURE);
	}
	
	
	printf("Checkpoing 4.2 \n");
	token = strtok(line, " "); //this is our first number
	printf("Checkpoing 4.3 \n");
	
	
	
	printf("Token equals <%s>", token);
	
	
	
	
	removeSpaces(token); //testing this new line
	printf("Checkpoing 4.4 \n");
	int ourValue = atoi(token);
	printf("Checkpoing 5 \n");
	if ((token = strtok(NULL, " ")) != NULL) {
		//this also catches hanging whitespace
		if (token[0] == '\n') {
			return ourValue;
		}
		else {
			return -1;
		}
	}
	else {
		return ourValue;
	}
}

void readAndStackNumbers(FILE *input, FILE *output, char programName[100], int sectionTotal) {
	printf("Checkpoing 7 \n");
	char line[100];
	char *token;
	int counter = 0;
	int singleNum;
	fgets(line, 100, input);
	//printf("Here's our line: %s\n", line);
	printf("Checkpoing 8 \n");
	int stack[sectionTotal];
	
	token = strtok(line, " "); //first element
	//singleNum = atoi(token);
	printf("Checkpoing 9 \n");
	while (token != NULL && token[0] != '\n' && counter < sectionTotal) {
		
		//printf("Yay %s yay\n", token);
		singleNum = atoi(token);
		//printf("Yay %d yay\n", singleNum);
		push(stack, singleNum);
		counter++;
		token = strtok(NULL, " ");
	}
	//printf("Counter equals %d and sectionTotal equals %d\n", counter, sectionTotal);
	printf("Checkpoing 10 \n");
	if (token != NULL && token[0] != '\n') { //this handles too many numbers
		errno = 1;
		errorMessage(programName, "Invalid input file format. Line contains more numbers then expected. ");
		exit(EXIT_FAILURE);
	}
	if (counter != sectionTotal) { //this handles too many numbers
		errno = 1;
		errorMessage(programName, "Invalid input file format. Line contains fewer numbers then expected. ");
		exit(EXIT_FAILURE);
	}
	
	//printf("Counter equals %d and sectionTotal equals %d\n", counter, sectionTotal);
	
	//printf("Made progress at least...\n");
	printf("Checkpoing 11 \n");
	fprintf(output, "%d: ", getpid());
	int j;
	for (j = 0; j < sectionTotal; j++) {
		fprintf(output, " %d ", pop(stack));
	}
	fprintf(output, "\n");
	printf("Checkpoing 12 \n");
	//printf("Made it to the end...\n");
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
	}
	printf("Opened file %s\n", inputFileName);

	int childCount;
	
	childCount = readOneNumber(input, programName);
	if (childCount == -1) {
		errno = 1;
		errorMessage(programName, "Invalid input file format. First line must contain a single digit. ");		
	}
	
	int parentPid;
	int listOfPids[childCount];
	int linesToPass = 0; //the child processes don't change the parent's reading position, so we must pass ahead in the parent as well
	
	pid_t pid;
	int i;
	for (i = 0; i < childCount; i++) {
		char c;
		int j;
		printf("Checkpoing 1 \n");
		for (j = 0; j < linesToPass; j++) { //we must pass over what the last child passed over so next child doesn't repeat work
			do
				c = fgetc(input);
			while (c != '\n');
		}
		linesToPass = 2;
		printf("Checkpoing 2 \n");
		pid = fork();
		if (pid > 0) { //parent case
			parentPid = getpid();
			wait(NULL);
			listOfPids[i] = pid;
			continue; //I am the parent, create more children
		}
		else if (pid == 0) { //child case
			printf("Checkpoing 3 \n");
			int sectionTotal;
			sectionTotal = readOneNumber(input, programName);
			if (sectionTotal == -1) {
				errno = 1;
				errorMessage(programName, "Invalid input file format. Line contains incorrect number of digits. ");		
			}
			
			//void readAndStackNumbers(FILE *input, FILE *output, char programName[100], int sectionTotal)	
			printf("Checkpoing 6 \n");
			readAndStackNumbers(input, output, programName, sectionTotal);
			printf("Checkpoing 13 \n");
			exit(0);
			break; //I am the child, get to work
		}
		else {
			errorMessage(programName, "Could not create child ");
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