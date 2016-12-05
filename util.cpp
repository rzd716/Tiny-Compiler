/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "GLOBALS.h"
#include "util.h"

/* Procedure printToken prints a token
* and its lexeme to the listing file
*/
void printToken(TokenType token, const char* tokenString)
{
	switch (token)
	{
	case IF:
	case THEN:
	case ELSE:
	case END:
	case REPEAT:
	case UNTIL:
	case READ:
	case WRITE:
	case True:
	case False:
	case Or:
	case And:
	case Not:
	case Int:
	case Bool:
	case String:
	case While:
	case Do:
		fprintf(listing,
			"(KEY, %s)\n", tokenString);
		break;
	case ASSIGN: fprintf(listing, "(SYM, :=)\n"); break;
	case LT: fprintf(listing, "(SYM, <)\n"); break;
	case EQ: fprintf(listing, "(SYM, =)\n"); break;
	case LPAREN: fprintf(listing, "(SYM, ()\n"); break;
	case RPAREN: fprintf(listing, "(SYM, ))\n"); break;
	case SEMI: fprintf(listing, "(SYM, ;)\n"); break;
	case PLUS: fprintf(listing, "(SYM, +)\n"); break;
	case MINUS: fprintf(listing, "(SYM, -)\n"); break;
	case TIMES: fprintf(listing, "(SYM, *)\n"); break;
	case OVER: fprintf(listing, "(SYM, /)\n"); break;
	case GT: fprintf(listing, "(SYM, >)\n"); break;
	case GTE: fprintf(listing, "(SYM, >=)\n"); break;
	case LTE: fprintf(listing, "(SYM, <=)\n"); break;
	case COMMA: fprintf(listing, "(SYM, ,)\n"); break;
	case ENDFILE: fprintf(listing, "EOF\n"); break;
	case NUM:
		fprintf(listing,
			"(NUM, %s)\n", tokenString);
		break;
	case ID:
		fprintf(listing,
			"(ID, %s)\n", tokenString);
		break;
	case STR:
		fprintf(listing,
			"(STR, %s)\n", tokenString);
		break;
	case ERROR:
		fprintf(listing,
			"column%d: ERROR: %s\n",columno, tokenString);
		break;
	case STRERROR:
		fprintf(listing,
			"column%d: ERROR: '\n", columno);
		break;
	default: /* should never happen */
		fprintf(listing, "Unknown token: %d\n", token);
	}
}


