//
// Created by 1 on 18.12.2020.
//

#ifndef MULTLANG_FRONTEND_H
#define MULTLANG_FRONTEND_H

#include "../tree/tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <stdarg.h>

enum ErrorType {

    UnexpectedTokenAtStart

};

static char* expression = nullptr;

const size_t MAX_IDENTIFIER_NAME = 100;
const size_t MAX_TOKENS_SIZE = 1000;
static Node* tokens [MAX_TOKENS_SIZE] = {nullptr};
static char currentTokenStatus = NOTYPE;

static size_t currentTokenNum = 0;
static size_t maxTokenNum     = 0;

Node* GetG            (char* storage, Tree* mainTree);
Node* GetEndOfProgram ();
Node* GetFunc         ();
Node* GetDec          ();
bool  RequireSysOpers (int sysOperNum);
Node* GetIden         ();
Node* GetParams       ();
Node* GetBlock        ();
Node* GetConcat       ();
Node* GetStatement    ();
Node* GetPrint        ();
Node* GetScan         ();
Node* GetIf           ();
Node* GetWhile        ();
Node* GetAssign       ();
Node* GetLogicOp      ();
Node* GetReturn       ();

//=====================
//~~~~~Калькулятор~~~~~

Node* GetExpression          ();
Node* GetVar                 ();
Node* GetMul                 ();
Node* GetBrack               ();
Node* GetSin                 ();
Node* GetCos                 ();
Node* GetNum                 ();
Node* GetFuncExpression      ();
Node* ReadFuncArgsExpression ();
Node* GetSqrt                ();

//=====================
//~~~~~~~~~~~~~~~~~~~~~

//====================
//~Лексический анализ~
void   ReadTokens            ();
Node*  MakeConstToken        (double value);
Node*  MakeVarToken          (char* varName);
Node*  MakeSysOperToken      (int operName);
Node*  MakeFuncToken         (char* funcName);
bool   ReadSysOperToken      ();
void   SyntaxError           (size_t errNum, char* required);
bool   ReadNextCell          ();
double ReadNumFromExpression ();
bool   ReadStatementToken    ();
char*  ReadStatementName     (size_t listSize, int toRequire, ...);
void   PrintTokens           ();
//~~~~~~~~~~~~~~~~~~~~
//====================

//=====================
//~~~Удобные функции~~~
size_t SkipUseless       ();
void   PrintTreeIntoFile (Node* currentNode, FILE* output, size_t deep);
//~~~~~~~~~~~~~~~~~~~~~
//=====================


#endif //MULTLANG_FRONTEND_H
