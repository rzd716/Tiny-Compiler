#pragma once
/****************************************************/
/* File: util.h                                     */
/* Utility functions for the TINY compiler          */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

void printToken(TokenType, const char*);

TreeNode * newStmtNode(StmtKind);

TreeNode * newExpNode(ExpKind);

void printTree(TreeNode *);

char * copyString(char *);

#endif
