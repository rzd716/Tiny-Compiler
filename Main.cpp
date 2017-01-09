/****************************************************/
/* File: main.c                                     */
/* Main program for TINY compiler                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
* generate code
*/
#define NO_CODE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#endif

/* allocate global variables */
int lineno = 0;  
int columno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int Error = FALSE;
int TraceParse = TRUE;

int main(int argc, char * argv[])
{
	TreeNode * syntaxTree;
	char pgm[120]; /* source code file name */
	/*if (argc != 2)
	{
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	strcpy(pgm, argv[1]);*/
	strcpy(pgm, "E:\\ctest\\experiment2_test1_in.txt");
	if (strchr(pgm, '.') == NULL)
		strcat(pgm, ".tny");
	source = fopen(pgm, "r");
	if (source == NULL)
	{
		fprintf(stderr, "File %s not found\n", pgm);
		exit(1);
	}
	listing = stdout; /* send listing to screen */
	fprintf(listing, "\nTINY+ COMPILATION: %s\n", pgm);

#if NO_PARSE
	while (getToken() != ENDFILE);
#else
	syntaxTree = parse();
	if (TraceParse) {
		fprintf(listing, "\nSyntax tree:\n");
		printTree(syntaxTree);
	}
#endif
	fclose(source);

	return 0;
}

