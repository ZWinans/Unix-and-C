#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Author: Zachary Winans
 * Class: CS352 - Fall 21
 * Assignment: linked.c
 * Purpose: The purpose of this program is to, when given directions such as:
 * 	@addPages - to add pages to the linked list
 * 	@addLinks - to add links between pages in the linked list
 * 	@isConnected - to verify if two pages are connected or not and print 1 if connected and 0 if not
 */

// create struct to account for a page
typedef struct page {
	char *name;
	int visited;
	struct link *head;
	struct page *next;
} page;

// create struct to account for a link
typedef struct link {
	page *linked;
	struct link *next;
} link;

int exitCode = 0;
page *front = NULL;

/*
 * Purpose: To search for a connection between a fromPage to a toPage by iterating through all given links
 *          Using depth first search. Heavily reliant on the DFS pseudocode given in the spec.
 *
 */
int DFS(page *fromPage, page *toPage)
{
	// if (fromPage == toPage) return 1
	if (fromPage == toPage) {
		return 1;
	} 
	
	// if fromPage is visited return 0
	if (fromPage -> visited == 1) {
		return 0;
	}
	
	// mark fromPage as visited
	fromPage -> visited = 1;

	// for each page midpage linked to by frompage do
	// 	if (dfs(midPage, toPage)) return 1
	link *midPage;

	for ( midPage = fromPage->head; midPage != NULL; midPage = midPage -> next) {
		if (DFS(midPage->linked, toPage) == 1) {
			return 1;
		}

	}

	// return 0
	return 0;
}

/*
 * Purpose: To throw memory errors as needed
 */
void throwMemoryError()
{
	fprintf(stderr, "Out of memory!\n");
	exit(1);
}

/*
 * Purpose: To build a link node and link the parameter page to connect to the link node
 * Parameter: 
 * 	toConnect - the given page to connect the link node to 
 *
 */
link *buildLink(page *toConnect)
{
	link *toBuild = malloc(sizeof(struct link) * 2);
	if (toBuild == NULL) {
		throwMemoryError();
	}
	
	toBuild -> linked = toConnect;
	toBuild -> next = NULL;

	return toBuild;

}



/*
 * Purpose: To build a page with a parameter representing the name of the page
 * 	    From here we will build a link between the head and the new page
 * Parameter:
 * 		nameOfPage - a char str to name the page
 */
page *buildPage(char *nameOfPage)
{
	page *toBuild = malloc(sizeof(struct link) * 2);
	if (toBuild == NULL) {
		throwMemoryError();
	} 

	toBuild -> name = strdup(nameOfPage);
	// check strdup
	if (toBuild -> name == NULL) {
		throwMemoryError();
	}
	toBuild -> head = buildLink(toBuild);
	toBuild -> next = NULL;

	return toBuild;
	
}

/*
 * Purpose: To add a page to the linked list
 * Parameter:
 * 		str - the name to use for page
 */
void addPages(char *str)
{
	page *ptr = front;

	for ( ; ptr->next != NULL; ptr=ptr->next) {
		if (strcmp(str, ptr->name) == 0) {
			fprintf(stderr, "This page already exists: %s\n", str);
			exitCode = 1;
			return;
		}

	}

	// at this point we are at the last node in the list so we will check the final node
	// and if it does not match we will create the new page
	if (strcmp(str, ptr->name) == 0) {
		fprintf(stderr, "This page already exists: %s\n", str);
		exitCode = 1;
		return;
	} 
	
	// because we have not returned at this point, we will build a new page because it does not
	// already exist	
	ptr->next = buildPage(str);
	
	
}

/*
 * Purpose: to, when given a name, search through the page links for that name
 * Parameter:
 * 		name - a char * str to search for in the list
 *
 */
page *findPageByName(char *name)
{
	page *ptr;

	for (ptr = front; ptr != NULL; ptr=ptr->next) {
		if (strcmp(ptr->name, name) == 0) {
			return ptr;
		}
	}

	return ptr;


}

/*
 * Purpose: To add a link from a given source page to a linkTo page
 * Parameter:
 * 	source - the source page
 * 	linkTo - the page to link to the source page
 *
 */
void addLinks(page *source, page *linkTo)
{
	link *ptr = source->head;

	// iterate through list 
	for ( ; ptr->next != NULL; ptr=ptr->next) {
		if (ptr->linked == linkTo) {
			return;
		}
	}

	// check last
	
	if (ptr->linked == linkTo) {
		return;
	}

	// else we will add the link
	ptr->next = buildLink(linkTo);
	

}

/*
 * Purpose: to set all the visited links to not visited
 *
 */
void clear() 
{
	page *ptr;

	for (ptr = front; ptr != NULL; ptr=ptr->next) {
		ptr->visited = 0;
	}
}


/*
 * Purpose: To process the line input for actions such as @addPages, @addLinks, or @isConnected
 * Parameter: 
 * 	line - the line to scrub/process
 */
void readLine(char *line)
{
	char buf[65];
	char *ptr = line;
	int n = 0;
	
	// this will be 0 if the line is blank
	if (sscanf(ptr, "%64s%n", buf, &n) != 0) {
		ptr += n;
		// check @addPages
		if (strcmp(buf, "@addPages") == 0) {
			while (sscanf(ptr, "%64s%n", buf, &n) == 1) {
				ptr += n;
				addPages(buf);
			}
		}
	
		// check @addLinks
		else if (strcmp(buf, "@addLinks") == 0) {
			if (sscanf(ptr, "%64s%n", buf, &n) != 1) {
				fprintf(stderr, "No page could be read.\n");
				exitCode = 1;
			} else {
			
				ptr += n;

				page *source = findPageByName(buf);
			
				if (source == NULL) {
					fprintf(stderr, "The source page %s, does not exist in the list.\n", buf);
					exitCode = 1;
				} else {
					while (sscanf(ptr, "%64s%n", buf, &n) == 1) {
						ptr += n;
						page *linkTo = findPageByName(buf);
						
						// check if linkTo is null
						if (linkTo == NULL) {
							fprintf(stderr, "The linked to destination does not exist: %s.\n", buf);
							exitCode = 1;
						} else {
							// else we have a proper link and can add it
							addLinks(source, linkTo);
						}
					}
				}

			}
		

		}

		// check @isConnected
		else if (strcmp(buf, "@isConnected") == 0) {
			// create a string for the first arg
			char first[65];
			// create a string for the second arg
			char second[65];		
			// if there is a third arg or more we will throw an error
			char third[65];
		
			// if sscanf == anything but 2 we have too many arguments
			if (sscanf(ptr, "%64s %64s %64s", first, second, third) != 2) {
				fprintf(stderr, "Too many arguments for @isConnected\n");
				exitCode = 1;
			} else {
				// else get the source and linkTo page to search for using DFS
				page *source = findPageByName(first);
				page *linkTo = findPageByName(second);

				if (source == NULL || linkTo == NULL) {
					fprintf(stderr, "One of the links you entered does not exist\n");
					exitCode = 1;
				} else {
					clear();
					int areConnected = DFS(source, linkTo);
					printf("%d\n", areConnected);
			
				}

			}
		} else { 
			
			fprintf(stderr, "You entered an illegal operation.\n");
			exitCode = 1;
		}

	// else we will return because we have a blank line and need to ignore it
	} else {
		return;
	}

}


/*
 * Purpose: to iterate through the pages and the inner linked lists to free all memory
 *
 */
void freeAllMemory()
{

	page *ptr = front;
	page *nextPage;
	link *linkPtr;
	link *nextLink;
	
	// iterate through pages
	for ( ; ptr != NULL ; ) {

		nextPage = ptr->next;
		linkPtr = ptr->head;
		nextLink = linkPtr->next;

		free(linkPtr);

		linkPtr = nextLink;
		
		// iterate through links of the given page
		for ( ; linkPtr != NULL ; ) {
			nextLink = linkPtr->next;
			free(linkPtr);
			linkPtr = nextLink;

		}

		free(ptr->name);
		free(ptr);
		ptr = nextPage;
	}
}

/*
 * Purpose: to build the head of our linked list
 *
 */
void buildHead()
{
	front = malloc(sizeof(struct page)*2);
	if (front == NULL) {
		throwMemoryError();
	} else {
		front -> next = NULL;
		front -> name = strdup("front");
		if (front -> name == NULL) {
			throwMemoryError();
		}
		front -> head = buildLink(front);
	}

}

int main (int argc, char *argv[])
{	
	// initialize our line to get
	char *line = NULL;
	size_t size = 0;

	// initialize our filePointer
	FILE *fPtr = stdin;

	// initialize and build the head of our LL
	buildHead();
	
	// get input from either stdin or command-line
	
	// create a flag so we know if we are using command line or not
	int flag = 0;
	if (argc > 2 ) {
		fprintf(stderr, "Too many command-line arguments.\n");
		exitCode = 1;
	}

	// if argc is 1, then we have a command line file
	if (argc > 1) {

		char *fileName = argv[1];
		flag = 1;
		fPtr = fopen(fileName, "r");

		if (fPtr == NULL) {
			fprintf(stderr, "There was an error opening the given file.\n");
			exit(1);
		}
	}
	
	// process the lines
	while (getline(&line, &size, fPtr) != EOF) {
		readLine(line);
	}

	if (flag == 1) {
		fclose(fPtr);
	}
	
	// free everything here
	free(line);
	freeAllMemory();
	
	return exitCode;

}

