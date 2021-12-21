/*
 * AUTHOR: ZACHARY WINANS
 * SEMESTER: FALL21
 * CLASS: CS352
 * DATE: 11/12/2021
 * FILE: myMake.c -- Assignment 9
 * PURPOSE: 
 * 	The purpose of this program is to construct a data dependency graph based on an input file
 * 	then, print out all dependencies of the requested input if those dependencies exist.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dGraph.h"

//init external head node
extern target *target_hd;

// initialize these variables here so we can fclose/free them if need be
int exitCode = 0;
FILE *fPtr;
char *line;

/*
 * PURPOSE: To free all memory and exit in the event of an error
 * PARAMETER:
 * 	flag - an integer representing which error occurred
 */
void freeAndExit(int flag)
{
	freeAllMemory();

	if (flag == 0) {
		fclose(fPtr);
	}

	if (flag == 1) {
		fprintf(stderr, "There was an error opening the file you requested.\n");
	}

	if (flag == 2) {
		fprintf(stderr, "I'm sorry, you didn't enter the correct number of arguments.\n");
	}

	if (flag == 3) {
		free(line);
		fclose(fPtr);
		fprintf(stderr, "I'm sorry, the first line must contain a target.\n");
	}

	if (flag == 4) {
		free(line);
		fclose(fPtr);
		fprintf(stderr, "I'm sorry, the target must contain a string and then a colon.\n");
	}

	if (flag == 5) {
		free(line);
		fclose(fPtr);
		fprintf(stderr, "I'm sorry, but the target you indicated is empty.\n");
	}

	if (flag == 6) {
		free(line);
		fclose(fPtr);
		fprintf(stderr, "I'm sorry, but the target you indicated is already a target.\n");
	}

	if (flag == 7) {
		free(line);
		fclose(fPtr);
		fprintf(stderr, "I'm sorry, but the target you indicated was not found.\n");
	}

	exit(1);
}

/*
 * PURPOSE: To check if a line contains only white space
 * PARAMETER:
 * 	line - a char str* to check if it is a space
 */
int isEmpty(char *line)
{
	char *linePtr = line;
	
	for (; *linePtr != '\0'; linePtr++) {
		// if not space
		if (!isspace(*linePtr)) {
			return 0;
		}

	}	
	
	return 1;

}

/*
 * PURPOSE: to process the line and clean spaces on either side/ensure there is a colon in the
 * 	    correct position
 * PARAMETERS:
 * 	line - a char *line to read and process
 */
target *read_line(char *line)
{
	char buf[65]; // limit of 64 characters
	char *ptr = line;
	int read;
	char colon;
	
	// regex that reads up to colon, then colon, then remaining
	char *regex = "%64[^:]%[:]%n";	

	int scan = sscanf(ptr, regex, buf, &colon, &read);	

	if (scan != 2 || colon != ':') {
		// line input is incorrect
		freeAndExit(4);
	}

	
	// iterate ptr
	ptr += read;

	char *aTargetStr = buf;
	
	if (sscanf(buf, "%s", aTargetStr) != 1) {
		// target is empty
		freeAndExit(5);
	}

	target *aTarget = addTarget(aTargetStr);
	
	// check to see if the target is already a target
	if (aTarget->completed != 0) {
		freeAndExit(6);
	} 
	
	// set target to completed	
	aTarget->completed = 1;

	// add dependencies to target and iterate ptr
	for (; sscanf(ptr, "%64s%n", buf, &read) == 1; ptr += read) {
		addDependency(aTarget, buf);
	}

	return aTarget;

}

/*
 * PURPOSE: to get rid of any white spaces at the beginning of the line
 * PARAMETERS:
 * 	line - a char *str to clean the front of
 */
char *processCommandFront(char *line)
{
	char *commandStart = line;
	
	// iterate through line while char is space
	for (; isspace(*commandStart); commandStart++) 
	{
	}

	return commandStart;

}

/*
 * PURPOSE: To find and print all dependencies
 * PARAMETERS:
 * 	aTarget - a char *str to look for in the dependencies list
 */
void findTargetDependencies(char *aTarget)
{

	target *finalTarget = findTargetByStr(aTarget);

	// check if finalTarget is found
	if (finalTarget == NULL || finalTarget->completed == 0) {
		freeAndExit(7);
        }

	postOrder(finalTarget);

}

int main(int argc, char **argv)
{
	line = NULL;
	size_t size = 0;
	target *curr = NULL;

	// check for correct number of arguments
	if (argc == 3) {
		char *aMakeFile = argv[1];
		char *aTarget = argv[2];
		fPtr = fopen(aMakeFile, "r");

		if (fPtr == NULL) {
			printf("%s\n", aMakeFile);
			// error opening file, pass indicator of error and exit
			freeAndExit(1);
		}
		
		// build an empty node to check for memory issues
		target_hd = buildTarget("EMPTY");

		size_t lines = getline(&line, &size, fPtr);

		while (lines != EOF && isEmpty(line) == 1) {
			// if line is empty
			lines = getline(&line, &size, fPtr);
		}
		
		// checking if the first line is a target
		if (line[0] == '\t') {
			freeAndExit(3);
		}

		curr = read_line(line);

		// read until EOF
		while ((lines = getline(&line, &size, fPtr)) != EOF) {
		
			if (isEmpty(line)) {
				continue;
			}

			// if not command
			if (line[0] != '\t') {

				curr = read_line(line);

			} else {
				
				char *command = processCommandFront(line);
				int length = strlen(line) - 1;

				char *commandEnd = line + length;
				
				if (isspace(*commandEnd)) {
					// if last char isspace, iterate through setting end to \0
					for (; isspace(*commandEnd); commandEnd--) {
						*commandEnd = '\0';
					}
				}

				//add command to the current target
				addCommand(curr, command);

			}

		}

		findTargetDependencies(aTarget);

		free(line);
		freeAndExit(0);

		return exitCode;

	} else {
		// else incorrect number of arguments, pass indicator of error and exit
		freeAndExit(2);
	}

	return exitCode;

}
