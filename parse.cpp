#include "parse.h"
#include "scan.h"
#include "util.h"
#include "symtab.h"

static TokenType token; /* holds current token */

						/* function prototypes for recursive calls */
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * exp(void);
static TreeNode * arithmetic_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);
static TreeNode * while_stmt(void);
static TreeNode * or_exp(void);
static TreeNode * and_exp(void);
static TreeNode * not_exp(void);
static TreeNode * program(void);
static void declarations(void);
static TreeNode * relational_exp(void);
static bool isDeclare(TokenType);

static void syntaxError(char * message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

static void match(TokenType expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "      ");
	}
}

TreeNode * statement(void)
{
	TreeNode * t = NULL;
	switch (token) {
	case IF: t = if_stmt(); break;
	case REPEAT: t = repeat_stmt(); break;
	case ID: t = assign_stmt(); break;
	case READ: t = read_stmt(); break;
	case WRITE: t = write_stmt(); break;
	case While:t = while_stmt(); break;
	default: syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	} /* end case */
	return t;
}

TreeNode * stmt_sequence(void)
{
	TreeNode * t = statement();
	TreeNode * p = t;
	while ((token != ENDFILE) && (token != END) &&
		(token != ELSE) && (token != UNTIL))
	{
		TreeNode * q;
		match(SEMI);
		q = statement();
		if (q != NULL) {
			if (t == NULL) t = p = q;
			else /* now p cannot be NULL either */
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode * parse(void)
{
	TreeNode * t;
	token = getToken();
	t = program();
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}

TreeNode * program(void)
{
	declarations();
	return stmt_sequence();
}
void declarations(void)
{
	while (isDeclare(token))
	{
		TokenType type = token;
		match(token);
		while (true)
		{
			if (token == ID)
			{
				st_first_insert(copyString(tokenString), lineno, location++, type);
				match(token);
				if (token == COMMA)match(token);
				if (token == SEMI)break;
			}
			else
			{
				printf("Declare need ID token!!\n");
			}
		}
		match(SEMI);
	}
}
bool isDeclare(TokenType token)
{
	if (token == Int || token == Bool || token == String)return true;
	return false;
}
TreeNode * if_stmt(void)
{
	TreeNode * t = newStmtNode(IfK);
	match(IF);
	if (t != NULL) t->child[0] = exp();
	match(THEN);
	if (t != NULL) t->child[1] = stmt_sequence();
	if (token == ELSE) {
		match(ELSE);
		if (t != NULL) t->child[2] = stmt_sequence();
	}
	match(END);
	return t;
}

TreeNode *exp(void)
{
	return or_exp();
}


TreeNode * or_exp(void)
{
	TreeNode * t = and_exp();
	while(token == Or)
	{
		TreeNode *p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if(t!=NULL)
			t->child[1] = and_exp();
	}
	return t;
}

TreeNode * and_exp(void)
{
	TreeNode *t = not_exp();
	while(token == And)
	{
		TreeNode *p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t != NULL)
			t->child[1] = not_exp();
	}
	return t;
}

TreeNode * not_exp(void)
{
	TreeNode * t=NULL;
	if(token==Not)
	{
		TreeNode* p = newExpNode(OpK);
		if (p != NULL) {
			t = not_exp();
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			match(token);
		}
	}
	else
	{
		t = relational_exp();
	}
	return t;
}

TreeNode * relational_exp(void)
{
	TreeNode * t = arithmetic_exp();
	if ((token == LT) || (token == EQ || token == GT || token == LTE || token == GTE)) {
		TreeNode * p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t != NULL)
			t->child[1] = arithmetic_exp();
	}
	return t;
}

TreeNode * arithmetic_exp(void)
{
	TreeNode * t = term();
	while ((token == PLUS) || (token == MINUS))
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			match(token);
			t->child[1] = term();
		}
	}
	return t;
}

TreeNode * term(void)
{
	TreeNode * t = factor();
	while ((token == TIMES) || (token == OVER))
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			match(token);
			p->child[1] = factor();
		}
	}
	return t;
}


TreeNode * factor(void)
{
	TreeNode * t = NULL;
	switch (token) {
	case NUM:
		t = newExpNode(ConstIntK);
		if ((t != NULL) && (token == NUM))
			t->attr.val = atoi(tokenString);
		match(NUM);
		break;
	case ID:
		t = newExpNode(IdK);
		if ((t != NULL) && (token == ID))
			t->attr.name = copyString(tokenString);
		match(ID);
		break;
	case LPAREN:
		match(LPAREN);
		t = exp();
		match(RPAREN);
		break;
	case String:
		t = newExpNode(ConstStringK);
		if (t != NULL)t->attr.valStr = copyString(tokenString);
		match(String);
		break;
	case True:
		t = newExpNode(ConstBoolK);
		if (t != NULL)t->attr.valBool=true;
		match(True);
		break;
	case False:
		t = newExpNode(ConstBoolK);
		if (t != NULL)t->attr.valBool =false;
		match(False);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode * repeat_stmt(void)
{
	TreeNode * t = newStmtNode(RepeatK);
	match(REPEAT);
	if (t != NULL) t->child[0] = stmt_sequence();
	match(UNTIL);
	if (t != NULL) t->child[1] = exp();
	return t;
}

TreeNode * assign_stmt(void)
{
	TreeNode * t = newStmtNode(AssignK);
	if ((t != NULL) && (token == ID))
		t->attr.name = copyString(tokenString);
	match(ID);
	match(ASSIGN);
	if (t != NULL) t->child[0] = exp();
	return t;
}

TreeNode * read_stmt(void)
{
	TreeNode * t = newStmtNode(ReadK);
	match(READ);
	if ((t != NULL) && (token == ID))
		t->attr.name = copyString(tokenString);
	match(ID);
	return t;
}

TreeNode * write_stmt(void)
{
	TreeNode * t = newStmtNode(WriteK);
	match(WRITE);
	if (t != NULL) t->child[0] = exp();
	return t;
}

TreeNode * while_stmt(void)
{
	TreeNode * t = newStmtNode(WhileK);
	match(While);
	if (t != NULL) t->child[0] = or_exp();
	match(Do);
	if (t != NULL) t->child[1] = stmt_sequence();
	match(END);
	return t;
}