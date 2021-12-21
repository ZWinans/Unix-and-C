/*
 * Author: Zachary Winans
 * Class: CSC352
 * Assignment: calls/assignment 8
 * Semester: Fall 21
 * Purpose: The purpose of this program is to, when given a file of connected calls
 * to register the connection as well as allow querying so when a user enters
 * two numbers, it will get the number of the times talked and if not directly linked
 * it will iterate through the linked list and find the path using BFS.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct numLL {
	char *num;
	struct call *head;
	struct number *next;
	struct number *queuePos;
	int BFSDepth;
} number;

typedef struct callLL {
	int count;
	number *called;
	struct call *next;
} call;

int exitCode = 0;
number *front = NULL;

/*
 * Purpose: To find the shortest possible path from one number to another, -1 if there is no connection
 * Parameters:
 * 	start - the first struct to start with
 *	target - the number target to look for
 */
int BFS(number *start, number *target)
{
	//set all BFSDepths to -1 to begin
	
	number *ptr = front;

	for (; ptr != NULL; ptr=ptr->next) {
		ptr->BFSDepth = -1;
		ptr->queuePos = NULL;
	}

	number *a = start;
	number *tail = start;
	
	// set level of start to 0
	a->BFSDepth = 0;
	
	//mark start as queued
	a->queuePos = NULL;

	// while not empty
	//for (; beginning != NULL; beginning = beginning->queuePos) {
	do {	
		if (a == target) {
			return a->BFSDepth - 1;
		}

		call *ptr2 = a->head;

		for (; ptr2 != NULL; ptr2=ptr2->next) {
		
			number *c = ptr2->called;
			if (c->BFSDepth < 0) {
				tail->queuePos = c;
				tail = c;
				tail->queuePos = NULL;
				c->BFSDepth = a->BFSDepth + 1;

			}

		}
		a = a->queuePos;
	} while (a != NULL);
	
	// no path from start to target
	return -1;

}

/*
* PURPOSE: To throw memory errors as needed
*/
void throwMemoryError()
{
	fprintf(stderr, "Out of memory!\n");
	exit(1);
}

/*
 * Purpose: to build the head of our linked list
 */
void buildFront()
{
	front = malloc(sizeof(number));

	if (front == NULL) {
		throwMemoryError();
	} else {
		front->next = NULL;
		char *str = strdup("front");
		front->num = str;

		if (front->num == NULL) {
			throwMemoryError();
		}

		front->head = NULL;
		front->queuePos = NULL;
	}

}

/*
 * Purpose: To check if both of the inputted numbers are legal returns 1 if yes, 0 if no
 * Parameters:
 * 	one - the first char str* to check legality of
 * 	two - the second char str* to check legality of
 */
int areLegal(char *one, char *two) {
	if (strlen(one) != 12 || strlen(two) != 12) {
		return 0;
	} else {
		for (int i = 0; i < 12; i++) {
			if (i == 3 || i == 7) {
				if (one[i] != '-' || two[i] != '-') {
					return 0;
				}
			} else {
				if (one[i] < '0' || two[i] < '0' || one[i] > '9' || two[i] > '9') {
					return 0;
				}
			}

		}
	}
	return 1;
}

/*
 * Purpose: To, when given a number, search through the number links for that number
 * Parameter:
 * 	num - a char *str representation of a number to look for
 */
number *findNumber(char *num)
{
	number *ptr;

	for (ptr = front; ptr != NULL; ptr=ptr->next) {
		if (strcmp(ptr->num, num) == 0) {
			return ptr;
		}
	}
	return ptr;
}

/*
 * Purpose: To build a call node by setting the called field to the parameter num
 * Parameter:
 * 	num - the number to "call"
 */
call *buildCall(number *num)
{
	call *number = malloc(sizeof(call));
	if (number == NULL) {
		throwMemoryError();
	}

	number->called = num;
	number->count = 1;
	number->next = NULL;
	
	return number;
}

/*
 * Purpose: To build a number struct from a given string
 * Parameters: 
 * 	numToBuild - a char *str to turn into a number struct
 */
number *buildNumber(char *numToBuild)
{
	number *numb = malloc(sizeof(number));
	if (numb == NULL) {
		throwMemoryError();
	}

	numb->num = strdup(numToBuild);

	if (numb->num == NULL) {
		throwMemoryError();
	} 
			
	numb->head = NULL;
	numb->next = NULL;
	numb->queuePos = NULL;

	return numb;

}

/*
 * Purpose: to add the number to the end of the list
 * Parameter:
 * 	num - a string representation of the number to create a number struct of
 * 	and add to the end of the list
 */
void addNumber(char *num)
{
	number *ptr = front;
	
	// iterate to end of the list;
	for (; ptr->next != NULL; ptr=ptr->next) 
	{
	}

	ptr->next = buildNumber(num);
}

/*
 * Purpose: to add the call to the list
 * Parameters:
 * 	from - a number struct from to connect
 *	to   - a number struct to to connect to
 */
void addCall(number *from, number *to)
{
	call *ptr = from->head;
	
	// if no calls
	if (ptr == NULL) {
		from->head = buildCall(to);
		return;
	}
	
	// if called/increment count
	for (; ptr->next != NULL; ptr=ptr->next) {
		if (ptr->called == to) {
			ptr->count++;
			return;
		}
	}

	// at this point we are at the end of the linked list so we'll check
	// the final node
	if (ptr->called == to) {
		ptr->count++;
		return;
	} else {
		// else we will build the call
		ptr->next = buildCall(to);
	}

}

/*
 * Purpose: To find a call if it exists/add a call if it does not
 * Parameters:
 * 	first - char * str representation of the first number
 * 	second - char * str representation of the second number
 */
void addNumbersAndCalls(char *first, char *second)
{
	number *num1 = findNumber(first);
	number *num2 = findNumber(second);

	// if num1 returns null we'll add it to the list
	if (num1 == NULL) {
		addNumber(first);
		num1 = findNumber(first);
	}
	
	if (num2 == NULL) {
		addNumber(second);
		num2 = findNumber(second);
	}

	// add calls in reciprocal manner so each numbers "call" eachother
	addCall(num1, num2);
	addCall(num2, num1);

}

/*
 * Purpose: To find out if, given a from and to number, the numbers interacted
 * if not, return -1
 * Parameter:
 * 	from - the char *str representation of the first number
 * 	to - the char *str representation of the second number
 */
int isConnected(number *from, number *to)
{
	call *ptr = from->head;

	for (; ptr != NULL; ptr = ptr->next) {
		if (ptr->called == to) {
			return ptr->count;
		}
	}
	return -1;

}

/*
 * Purpose: To handle the queries that a user may enter
 * Parameters: 
 * 	first = the char *str representation of the first number
 *	second = the char *str representation of the second number
 */
void handleQueries(char *first, char *second)
{
	number *num1 = findNumber(first);
	number *num2 = findNumber(second);

	if (num1 == NULL || num2 == NULL) {
		fprintf(stderr, "I'm sorry, one of the numbers you entered does not exist\n");
		exitCode = 1;
		return;
	} else {
		// check to see if the two numbers have called eachother directly
		int connected = isConnected(num1, num2);
		// if they have not, breadth first search to find if they are connected
		// otherwise, print number of times called
		if (connected  == -1) {
			connected = BFS(num1, num2);
			if (connected == -1) {
				printf("Not connected\n");
			} else {
				printf("Connected through %d numbers\n", connected);
			}

		} else {
			printf("Talked %d times\n", connected);
			return;

		}

	}

}

/*
 * Purpose: To read the line of the given file, if the flag is set to 0, we will
 * insert into the LL, if it is 1 we will handle a query
 *
 */
void readLine(char *line, int flag)
{
	char first[14];
	char second[14];
	char third[14];
	
	//this will be 0 if the line is blank
	int status = sscanf(line, "%13s %13s %13s", first, second, third);
	
	// blank
	if (status <= 0) {
		return;
	}
	
	if (status == 2) {
		// if both numbers are the same
		if (strcmp(first, second) == 0) {
			fprintf(stderr, "I'm sorry, but you can't enter the same number twice.\n");
			exitCode = 1;
			return;
		}

		// if both numbers are legal
		int legal = areLegal(first, second);

		if (legal == 1) {
			
			// flag will be 0 if we need to insert
			if (flag == 0) {
				addNumbersAndCalls(first, second);
			// else flag is 1 for query
			} else {
				handleQueries(first, second);	
			}
			

		} else {
			fprintf(stderr, "I'm sorry, one of the numbers you entered was not legal\n");
			exitCode = 1;
			return;
		}

	} else {
		fprintf(stderr, "You entered an illegal number of arguments\n");
		exitCode = 1;
		return;
	}


}

/*
 * Purpose: to free all the memory in the lists
 *
 */
void freeAllMemory()
{
	number *numberPtr = front;
	number *nextNumberPtr;
	call *callPtr;
	call *nextCallPtr;

	for (; numberPtr != NULL; numberPtr=nextNumberPtr) {
		
		nextNumberPtr = numberPtr->next;
		
		//handle calls
		callPtr = numberPtr->head;

		if (callPtr == NULL) {
			return;
		}
		
		// else iterate through call
		nextCallPtr = callPtr->next;
		free(callPtr);
		for (; callPtr != NULL; callPtr=nextCallPtr) {
			nextCallPtr = callPtr->next;
			free(callPtr);
		}
		
		// free num and ptr
		free(numberPtr->num);
		free(numberPtr);
		
	}
}


int main(int argc, char **argv)
{
	// initialize the head of our LL
	buildFront();	
	
	// check argc length
	if (argc <= 1) {
		fprintf(stderr, "Please enter at least ONE input file\n");
		exit(1);
	} else {
		int fNum = 1;
		for (; fNum < argc; fNum++) {

			char *fName = argv[fNum];
			FILE *fPtr = fopen(fName, "r");
			
			// check file pointer
			if (fPtr == NULL) {
				fprintf(stderr, "There was an error opening the indicated file.\n");
				exitCode = 1;
			}
			
			// initialize the line/size for input
			char *line = NULL;
			size_t size = 0;

			// read the input files
			while (getline(&line, &size, fPtr) != EOF) {
				readLine(line, 0);
			}
			
			// close file and free line
			fclose(fPtr);
			free(line);
		}


		// at this point we have created our lists but now we need
		// to handle querying
		char *line = NULL;
		size_t size = 0;
		while (getline(&line, &size, stdin) != EOF) {
			readLine(line, 1);
		}
		
		// free everything here
		free(line);
		freeAllMemory();
			

		return exitCode;
		
	}
	
}

