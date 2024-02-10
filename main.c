#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"



/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 * 
 * @author Sumneet
 */
int main(int argc, char* argv[]) {
	// need to check if the appropriate arguments are given
	if (argc < 2) 
	{
		printf("Usage: ls2 <path> [exact-match-pattern]\n");
		return 1;
	}
	else 
	{
		// Mode 1 - only the path is given
		if (argv[1] != NULL && argv[2] == NULL)
		{
			printDir(argv[1], 0);
		}
		// Mode 2 - both the path and the exact match pattern are given
		else
		{
			printModDir(argv[1], argv[2], 0);
		}
	}
	return 0;
}
