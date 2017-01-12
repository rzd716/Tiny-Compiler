#include "GLOBALS.h"
#include "util.h"


static int indentno = 0;

#define INDENT indentno+=2
#define UNINDENT indentno-=2

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
	case Int:
	case Bool:
	case String:
	case While:
	case Do:
		fprintf(listing,
			"(KEY, %s)\n", tokenString);
		break;
	case And:fprintf(listing, "(KEY, and)\n"); break;
	case Or:fprintf(listing, "(KEY, or)\n"); break;
	case Not:fprintf(listing, "(KEY, not)\n"); break;
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

static void printSpaces(void)
{
	int i;
	for (i = 0; i<indentno; i++)
		fprintf(listing, " ");
}

void printTree(TreeNode * tree)
{
	int i;
	INDENT;
	while (tree != NULL) {
		printSpaces();
		if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt) {
			case IfK:
				fprintf(listing, "If\n");
				break;
			case RepeatK:
				fprintf(listing, "Repeat\n");
				break;
			case AssignK:
				fprintf(listing, "Assign to: %s\n", tree->attr.name);
				break;
			case ReadK:
				fprintf(listing, "Read: %s\n", tree->attr.name);
				break;
			case WriteK:
				fprintf(listing, "Write\n");
				break;
			case WhileK:
				fprintf(listing, "While\n");
				break;
			case StrDeclareK:
				fprintf(listing, "StrDeclare\n");
				break;
			case IntDeclareK:
				fprintf(listing, "IntDeclare\n");
				break;
			case BoolDeclareK:
				fprintf(listing, "BoolDeclare\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{
			switch (tree->kind.exp) {
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->attr.op, "\0");
				break;
			case ConstIntK:
			case ConstStringK:
			case ConstBoolK:
				fprintf(listing, "Const: %d\n", tree->attr.val);
				break;
			case IdK:
				fprintf(listing, "Id: %s\n", tree->attr.name);
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else fprintf(listing, "Unknown node kind\n");
		for (i = 0; i<MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}

TreeNode * newStmtNode(StmtKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

TreeNode * newExpNode(ExpKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

char * copyString(char * s)
{
	int n;
	char * t;
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t = (char *)malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}