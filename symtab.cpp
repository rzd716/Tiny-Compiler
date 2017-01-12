/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
in hash function  */
#define SHIFT 4

/* the hash function */
static int hash(char * key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

/* the list of line numbers of the source
* code in which a variable is referenced
*/
typedef struct LineListRec
{
	int lineno;
	struct LineListRec * next;
} *LineList;

/* The record in the bucket lists for
* each variable, including name,
* assigned memory location, and
* the list of line numbers in which
* it appears in the source code
*/
typedef struct BucketListRec
{
	char * name;
	TokenType type;
	LineList lines;
	int memloc; /* memory location for variable */
	struct BucketListRec * next;
} *BucketList;

/* the hash table */
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
* memory locations into the symbol table
* loc = memory location is inserted only the
* first time, otherwise ignored
*/
void st_first_insert(char * name, int lineno, int loc,TokenType type)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) /* variable not yet in table */
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->type = type;
		l->lines->next = NULL;
		l->next = hashTable[h];
		hashTable[h] = l;
	}
	else /* found in table, so just add line number */
	{
		printf("%s is already defined!!!\n", name);
	}
} /* st_insert */

void st_insert(char * name, int lineno, int loc)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) /* variable not yet in table */
	{
		printf("%s is not defined\n", name);
	}
	else /* found in table, so just add line number */
	{
		LineList t = l->lines;
		while (t->next != NULL) t = t->next;
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
} /* st_insert */

  /* Function st_lookup returns the memory
  * location of a variable or -1 if not found
  */
int st_lookup(char * name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) return -1;
	else return l->memloc;
}

ExpType st_lookupForType(char*name)
{
	ExpType et;
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l != NULL)
	{
		switch (l->type)
		{
		case Int:et = Integer; break;
		case Bool:et = Boolean; break;
		case String:et = StringET; break;
		default:
			break;
		}
	}
	else
	{
		printf("this ID is not difined");
	}
	return et;
}
/* Procedure printSymTab prints a formatted
* listing of the symbol table contents
* to the listing file
*/
void printSymTab(FILE * listing)
{
	int i;
	fprintf(listing, "Variable Name  Type     Location   Line Numbers\n");
	fprintf(listing, "-------------  ------   --------   ------------\n");
	for (i = 0; i<SIZE; ++i)
	{
		if (hashTable[i] != NULL)
		{
			BucketList l = hashTable[i];
			while (l != NULL)
			{
				LineList t = l->lines;
				fprintf(listing, "%-14s ", l->name);
				switch (l->type)
				{
				case String:
					fprintf(listing, "%-6s", "String"); break;
				case Int:
					fprintf(listing, "%-6s   ", "Int"); break;
				case Bool:
					fprintf(listing, "%-6s  ", "Bool  "); break;
				default:
					break;
				}

				fprintf(listing, "%-6d  ", l->memloc);
				while (t != NULL)
				{
					fprintf(listing, "%4d ", t->lineno);
					t = t->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
	}
} /* printSymTab */
