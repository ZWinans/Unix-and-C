/*
 * AUTHOR: ZACHARY WINANS
 * ASSIGNMENT: ASSIGNMENT 9
 * FILE: dGraph.c
 * PURPOSE: To create the functions that we made the stubs of in our header file "dGraph.h"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dGraph.h"

target *target_hd = NULL;

/*
 * PURPOSE: To throw a memory error if an alloc'd struct returns NULL
 */
void throwMemoryError()
{
	fprintf(stderr, "Out of memory!\n");
	exit(1);
}

/*
 * PURPOSE: to iterate through a given sequence of targetChildren in a postorder traversal
 * PARAMETERS:
 * 	aTarget - the target to traverse through (the children)
 */
void postOrder(target *aTarget) 
{
	// if aTarget visited, return
	if (aTarget->visited == 1) {
		return;
	}
	
	// mark aTarget as visited
	aTarget->visited = 1;

	// declare i
	dependency *i = aTarget->depFront;
	
	// for i = 1 to the end of the dependency list
	for (; i != NULL; i=i->next) {
		postOrder(i->targetPointer);
	}
	
	readLast(aTarget);
}


/*
 * PURPOSE: To build the target node in our targetLL struct
 * PARAMETERS:
 * 	targetName - a char *str to 
 */
target *buildTarget(char *targetStr)
{
	target *newTarget = malloc(sizeof(target));
	
	if (newTarget == NULL) {
		freeAllMemory();
		throwMemoryError();
	}

	newTarget->str = strdup(targetStr);

	if (newTarget->str == NULL) {
		freeAllMemory();
		throwMemoryError();
	}

	newTarget->visited = 0;
	newTarget->completed = 0;
	newTarget->depFront = NULL;
	newTarget->cmdFront = NULL;
	newTarget->next = NULL;

	return newTarget;

}

/*
 * PURPOSE: To build the dependency node in our dependencyLL struct
 * PARAMETERS:
 * 	location - the target to build the dependency to
 */
dependency *buildDependency(target *aTarget)
{
	dependency *newDependency = malloc(sizeof(dependency));
	
	// if null, free and throw memory error
	if (newDependency == NULL) {
		freeAllMemory();
		throwMemoryError();
	}

	newDependency->targetPointer = aTarget;
	newDependency->next = NULL;

	return newDependency;

}

/*
 * PURPOSE: To build a node in our commandLL struct
 * PARAMETERS:
 * 	commandName - a char *str to name the command
 */
command *buildCommand(char *commandName)
{
	command *newCommand = malloc(sizeof(command));
	if (newCommand == NULL) {
		freeAllMemory();
		throwMemoryError();
	}

	newCommand->cmdName = strdup(commandName);

	if (newCommand->cmdName == NULL) {
		freeAllMemory();
		throwMemoryError();
	}

	newCommand->next = NULL;

	return newCommand;
}

/*
 * PURPOSE: To find a target node by a given string, if not in the list, returns NULL
 * PARAMETERS:
 * 	str - a char str* to search for in the target list
 */
target *findTargetByStr(char *str)
{
	target *ptr = target_hd;

	for (; ptr != NULL; ptr=ptr->next) {
		if (strcmp(ptr->str, str) == 0) {
			return ptr;
		}
	}

	return ptr;
}


/*
 * PURPOSE: to add the target to the list if the target is not already in the list
 * PARAMETER:
 * 	aTargetStr - a char* str to either look for or add in the target list
 */
target *addTarget(char *aTargetStr)
{
	target *newTarget = findTargetByStr(aTargetStr);
	target *curr;

	// at this point, newTarget will be null or a target
	if (newTarget == NULL) {
		// build the target
		newTarget = buildTarget(aTargetStr);
		curr = target_hd;
		
		//iterate to end of the target list
		for (; curr->next != NULL; curr=curr->next) 
		{
		}

		curr->next = newTarget;
		return newTarget;

	} else {
		// else return newTarget if not NULL
		return newTarget;
	}
	
}

/*
 * PURPOSE: To add a dependency to the given target
 * PARAMETERS:
 * 	aTarget - a target to add the dependency to
 * 	dependencyName - a char *str dependency to add to the target
 */
void addDependency(target *aTarget, char *dependencyName)
{	
	// add the dependency target	
	target *dep = addTarget(dependencyName);
	
	dependency *ptr = aTarget->depFront;

	if (ptr == NULL) {
		aTarget->depFront = buildDependency(dep);
		return;
	}

	// iterate through the target list
	for ( ; ptr->next != NULL; ptr=ptr->next) {
		// check if the dependency already exists for that target
		if (ptr->targetPointer == dep) {
			freeAllMemory();
			fprintf(stderr, "I'm sorry, this target already has %s, as a dependency.\n", dep->str);
			exit(1);
		}
	}
	
	// at this point we're at the last node
	if (ptr->targetPointer == dep) {
		freeAllMemory();
		fprintf(stderr, "I'm sorry, this target already has %s, as a dependency.\n", dep->str);
		exit(1);
	} else {
		// else build the dependency
		ptr->next = buildDependency(dep);
	}
	
}


/*
 * PURPOSE: to add a command to a specific target
 * PARAMETERS:
 * 	addTo - the target to add the command to
 * 	command - the char *str command to add
 */
void addCommand(target *aTarget, char *commandName)
{

	command *commandPtr = aTarget->cmdFront;
	// if no current command, create one
	if (commandPtr == NULL) {
		aTarget->cmdFront = buildCommand(commandName);
		return;
	}
	
	// iterate through list
	for ( ; commandPtr->next != NULL; commandPtr=commandPtr->next)
	{
	}

	// at this point we're at the last node, create command
	commandPtr->next = buildCommand(commandName);
}

/*
 * PURPOSE: To process the last target in the postorder traversal
 * PARAMETER:
 * 	aTarget - the target to print
 */
void readLast(target *aTarget)
{
	printf("%s\n", aTarget->str);

	// get the command at the front of the target
	command *ptr = aTarget->cmdFront;
	
	// iterate through the command LL
	for (; ptr != NULL; ptr=ptr->next) {
		printf("  %s\n", ptr->cmdName);
	}
}

/*
 * PURPOSE: To free all of the memory in the used lists
 */
void freeAllMemory()
{	
	// initialize pointers
	target *targetPtr = target_hd;
	dependency *depPtr;
	command *cmdPtr;
	
	// initialize temp pointers
	target *targetNext;
	dependency *depNext;
	command *cmdNext;
	
	// iterate through outermost list target
	for ( ; targetPtr != NULL; targetPtr=targetNext) {
		
		targetNext = targetPtr->next;

		depPtr = targetPtr->depFront;
		// iterate through dependency list
		for (; depPtr != NULL; depPtr=depNext) {
			depNext = depPtr->next;
			free(depPtr);
		}

		cmdPtr = targetPtr->cmdFront;
		// iterate through command list	
		for (; cmdPtr != NULL; cmdPtr=cmdNext) {
			cmdNext = cmdPtr->next;
			free(cmdPtr->cmdName);
			free(cmdPtr);
		}
		
		// free targetPtr str and targetPtr
		free(targetPtr->str);
		free(targetPtr);
	}

}
