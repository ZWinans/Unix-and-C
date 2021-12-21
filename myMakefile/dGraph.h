/*
 * AUTHOR: ZACHARY WINANS
 * ASSIGNMENT: ASSIGNMENT 9
 * FILE: dGraph.h
 * PURPOSE: To create a header file for our dependency graph
 */

#ifndef _DGRAPH_H
#define _DGRAPH_H

/*
 * PURPOSE: Initialize target, dependency, and command structs
 */

typedef struct targetLL 
{
	int visited;
	int completed;
	char *str;
	struct dependencyLL *depFront;
	struct commandLL *cmdFront;
	struct targetLL *next;
} target;

typedef struct dependencyLL {
	target *targetPointer;
	struct dependencyLL *next;
} dependency;	

typedef struct commandLL {
	char *cmdName;
	struct commandLL *next;
} command;

/*
 * PURPOSE: To create the target global variable
 */

extern target *target_hd;

/*
 * PURPOSE: initialize function stubs here
 */

void postOrder(target *aTarget);

target *buildTarget(char *targetStr);

dependency *buildDependency(target *location);

command *buildCommand(char *commandName);

target *addTarget(char *aTargetStr);

void addDependency(target *aTarget, char *dependencyName);

void addCommand(target *addTo, char *command);

target *findTargetByStr(char *str);

void readLast(target *aTarget);

void freeAllMemory();

#endif
