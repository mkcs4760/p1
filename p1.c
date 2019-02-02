//As of now, we can read all the file input by the specific forked processes
//Still need to add errors for when file does not match desired format
//This is a huge step forward, however
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int MAXSIZE = 100;
//int stack[];
int top = -1;

int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

int peek(int stack[]) {
	return stack[top];
}

int pop(int stack[]) {
   int data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

int push(int stack[], int data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}


int main(int argc, char *argv[]) {
	char inputFileName[] = "input.dat";
	char outputFileName[] = "output.dat";
	
	printf("There are %d arguments\n", argc);
	
	int option;
	char temp[100]; //setting max size as very high
	while ((option = getopt(argc, argv, "hi:o:")) != -1) {
		switch (option) {
			case 'h' :
						printf("You entered h\n"); //make sure everything needed is stated here
						printf("Help page for OS_Klein_project1\n");
						printf("Consists of the following:\n\tOne .c file titled p1.c\n\tOne Makefile\n\tOne README.md file\n\tOne version control log.\n");
						printf("The command 'make' will run the makefile and compile the program\n");
						printf("Usage: ./p1 -i <inputFileName> -o <outputFileName> | ./p1 -h\n");
						printf("Version control acomplished using github. Log obtained using command 'git log > versionLog.txt\n");
						exit(0);
						break;
			case 'i' :
						//&temp = optarg;
						printf("You entered i\n");
						printf("Argument is %s\n", optarg);
						strcpy(inputFileName, optarg);
						break;
			case 'o' :
						printf("You entered o\n");
						printf("Argument is %s\n", optarg);
						strcpy(outputFileName, optarg);
						break;
			default :	printf("You entered an invalid argument\n");
		}
	}
	
	printf("Welcome to the project\n");
	int i;
	//exit(0); //for testing
	

	FILE *input;
	input = fopen(inputFileName, "r");
	FILE *output;
	output = fopen(outputFileName, "w");
	if (input == NULL)
	{
		char errorString[100];
		snprintf(errorString, sizeof errorString, "Failed to locate file %s ", inputFileName);
		perror(errorString);
		//error("Failed to locate file %s ", inputFileName);
		exit(1);
	}
	printf("Opened file %s\n", inputFileName);

	int childCount;
	
	fscanf(input, "%d", &childCount);

	printf("The first number is our file is %d\n", childCount);
	printf("Meaning we need to fork %d children to handle the file\n", childCount);
	
	int parentPid;
	int listOfPids[childCount];
	int linesToPass = 1; //the child processes don't change the parent's reading position, so we must pass ahead in the parent as well
	
	pid_t pid;
	for (i = 0; i < childCount; i++)
	{
		printf("Now let's shift over for the next kiddo \n");
		char c;
		int j;
		for (j = 0; j < linesToPass; j++) { //we must pass over what the last child passed over so next child doesn't repeat work
			do
				c = fgetc(input);
			while (c != '\n');
		}
		linesToPass = 2;
		
		pid = fork();
		//listOfPids[i] = getpid();
		if (pid > 0) {
			printf("Don't look at me I'm only the parent\n");
			parentPid = getpid();
			wait(NULL);
			printf("The child that just completed had the pid %d\n", pid);
			listOfPids[i] = pid;
			continue; //I am the parent, create more children
		}
		else if (pid == 0) {
			printf("This is a child with id %d\n", getpid());
			//listOfPids[i] = getpid();
			printf("That's right, %d\n", listOfPids[i]);
			int sectionTotal;
			fscanf(input, "%d", &sectionTotal);
			printf("The first number in this section is %d\n", sectionTotal);
			printf("The remaining %d numbers are: ", sectionTotal);
			int j;
			int stack[sectionTotal];
			for (j = 0; j < sectionTotal; j++) {
				int temp;
				fscanf(input, "%d", &temp);
				printf(" %d ", temp);
				push(stack, temp);
			}
			for (j = 0; j < sectionTotal; j++) {
				fprintf(output, " %d ", pop(stack));
			}
			
			printf("\n");
			fprintf(output, "\n");
			//fputs("\n", output);
			exit(0);
			break; //I am the child, get to work
		}
		else {
			perror("Error while forking");
			exit(EXIT_FAILURE);
		}
	}
	//listOfPids(childCount) = gitppid();
	
	fclose(input);
	fclose(output);
	printf("All children were: ");
	for (i = 0; i < childCount; i++) {
		printf(" %d ", listOfPids[i]);
	}
	printf("\n And the parent was %d\n", parentPid);
	
	printf("And now we're done\n");
	
	return 0;
}
