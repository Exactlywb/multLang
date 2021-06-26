//
// Created by 1 on 18.12.2020.
//

#include "frontend.h"

#define STR_EQ(str1, str2, length) !strncmp(str1, str2, length)

Tree* langTree = nullptr;

Node* GetG (char* storage, Tree* mainTree) {
    printf ("Here work1!\n");
    assert (storage);

    langTree = mainTree; //передали указатель для увеличения размера
    expression = storage;

    ReadTokens  ();
    PrintTokens ();

    maxTokenNum = currentTokenNum;
    currentTokenNum = 0;

    Node* root = MakeSysOperNode (loader);
    TreeConstruct (langTree, root, "tree.dot");

    GetEndOfProgram ();

    printf ("Here work1!\n");
    return root;

}

Node* GetEndOfProgram () {

    Node* currentNode = nullptr;
    Node* prevNode = nullptr;

    while ( currentNode = GetFunc () ) {

        if (prevNode == nullptr)
            AddNode (langTree, langTree->root, currentNode, rightSide);
        else
            AddNode (langTree, prevNode, currentNode, rightSide);

        prevNode = currentNode;

    }

    return prevNode;

}

Node* GetFunc () {

    Node* decNode = GetDec ();
    if (decNode == nullptr)
        return nullptr;

    Node* idenNode  = GetIden ();
    Node* paramNode = GetParams ();
    Node* blockNode = GetBlock ();

    AddNode (langTree, decNode, idenNode, leftSide);
    AddNode (langTree, idenNode, paramNode, leftSide);

    if (blockNode)
        AddNode (langTree, idenNode, blockNode, rightSide);

    return decNode;

}

Node* GetDec () {
    printf ("Here work3!\n");
    if ( RequireSysOpers (func) ) {

        return MakeSysOperNode (func);

    }

    currentTokenNum--;
    printf ("Here work3!\n");
    return nullptr;

}
Node* GetParams () {

    if (RequireSysOpers(lcB)) {

        Node* argListFunc = MakeSysOperNode (lcB);
        Node* prevVarDec  = nullptr;

        while ( !RequireSysOpers(rcB) ) {

            currentTokenNum--;

            if ( prevVarDec == nullptr ) {

                Node* newNode = tokens [currentTokenNum];
                AddNode (langTree, argListFunc, newNode, rightSide);
                prevVarDec = newNode;
                continue;

            }

            if ( RequireSysOpers (comma) ) {

                Node* newNode = tokens [currentTokenNum];
                AddNode (langTree, prevVarDec, newNode, rightSide);
                prevVarDec = newNode;
                continue;

            }

        }

        return argListFunc;

    } else
        return nullptr;

}
Node* GetBlock () {

    if (RequireSysOpers(lqB)) {

        Node* concatNode = GetConcat ();

        RequireSysOpers (rqB);
        return concatNode;


    } else
        return nullptr;

}
Node* GetConcat () {

    Node* concatNode = MakeSysOperNode (concat);

    Node* leftStatement = GetStatement ();

    if ( leftStatement == nullptr ) {

        free (concatNode);

        return nullptr;

    }

    AddNode (langTree, concatNode, leftStatement, leftSide);

    Node* nextConcatNode = nullptr;
    if ( nextConcatNode = GetConcat () ) {

        AddNode (langTree, concatNode, nextConcatNode, rightSide);

    }

    return concatNode;

}
Node* GetStatement () {

    Node* firstNode = GetPrint ();

    if ( firstNode == nullptr )
        return nullptr;

    Node* statementNode = MakeSysOperNode (statement);

    AddNode (langTree, statementNode, firstNode, leftSide);

    return statementNode;

}
Node* GetIden () {

    Node* idenNode = tokens [currentTokenNum++];
    return idenNode;

}

#define GET_EXPR GetExpression () //написана
#define GET_VAR  GetVar        () //написана
#define GET_OP   tokens[currentTokenNum]->value.sysOper

Node* GetPrint () {

    if ( tokens[currentTokenNum]->value.sysOper == print_ ) {

        currentTokenNum++;

        Node* printNode = MakeSysOperNode (print_);
        if ( RequireSysOpers (lcB) )
            AddNode (langTree, printNode, GET_EXPR, rightSide);
        else {

            free (printNode);
            return nullptr;

        }

        if ( !RequireSysOpers (rcB) ) {

            free (printNode);
            return nullptr;

        }

        if ( !RequireSysOpers(endline) ) {

            free (printNode);
            return nullptr;

        }

        return printNode;

    } else
        return GetScan ();

}

Node* GetScan () {

    if ( tokens[currentTokenNum]->value.sysOper == scan_ ) {

        currentTokenNum++;

        Node* scanNode = MakeSysOperNode (scan_);
        if ( RequireSysOpers (lcB) )
            AddNode (langTree, scanNode, GET_EXPR, rightSide);
        else {

            free (scanNode);
            return nullptr;

        }

        if ( !RequireSysOpers (rcB) ) {

            free (scanNode);
            return nullptr;

        }

        if ( !RequireSysOpers(endline) ) {

            free (scanNode);
            return nullptr;

        }

        return scanNode;

    } else
        return GetIf ();

}

Node* GetIf () {

    if ( tokens[currentTokenNum]->value.sysOper == if_ ) {

        currentTokenNum++;

        Node* getLogicExpr = GetLogicOp ();

        Node* ifNode = MakeSysOperNode (if_);
        AddNode (langTree, ifNode, getLogicExpr, leftSide);

        RequireSysOpers (lqB);
        Node* concatNode = GetConcat ();

        if (concatNode)
            AddNode (langTree, ifNode, concatNode, rightSide);

        RequireSysOpers (rqB);

        return ifNode;

    } else
        return GetWhile ();

}

Node* GetWhile () {

    if ( tokens[currentTokenNum]->value.sysOper == while_ ) {

        currentTokenNum++;

        Node* getLogicExpr = GetLogicOp ();

        Node* whileNode = MakeSysOperNode (while_);
        AddNode (langTree, whileNode, getLogicExpr, leftSide);

        RequireSysOpers (lqB);
        Node* concatNode = GetConcat ();

        if (concatNode)
            AddNode (langTree, whileNode, concatNode, rightSide);

        RequireSysOpers (rqB);

        return whileNode;

    } else
        return GetReturn ();

}

Node* GetReturn () {

    if ( tokens [currentTokenNum]->value.sysOper == return_ ) {

        currentTokenNum++;

        Node* paramToReturn = GetExpression ();
        assert (paramToReturn);

        RequireSysOpers (endline);

        Node* returnInSelf  = MakeSysOperNode (return_);
        AddNode (langTree, returnInSelf, paramToReturn, rightSide);

        return returnInSelf;

    } else
        GetAssign ();

}

Node* GetLogicOp () {

    if ( !RequireSysOpers(lcB) )
        return nullptr;

    Node* firstTerm = GetExpression ();

    Node* newLogicalExpr = MakeSysOperNode ( tokens [currentTokenNum++]->value.sysOper );

    Node* secondTerm = GetExpression ();

    AddNode (langTree, newLogicalExpr, firstTerm, leftSide);
    AddNode (langTree, newLogicalExpr, secondTerm, rightSide);

    if ( !RequireSysOpers(rcB) ) {

        free (firstTerm);
        free (newLogicalExpr);
        free (secondTerm);
        return nullptr;

    }

    return newLogicalExpr;

}

Node* GetAssign () {

    if ( GET_OP == var_ ) {

        currentTokenNum++;

        Node* varName = GetVar ();
        assert (varName);
        if ( RequireSysOpers (endline) ) {

            Node* decNode = MakeSysOperNode (vardec);
            AddNode (langTree, decNode, varName, leftSide);

            return decNode;

        } else {

            currentTokenNum--;

            if ( RequireSysOpers (assignment) ) {

                Node* assignmentNode = MakeSysOperNode (assignment);
                AddNode (langTree, assignmentNode, varName, leftSide);

                Node* rightExpression = GetExpression ();
                assert (rightExpression);

                if ( RequireSysOpers (endline) ) {

                    AddNode (langTree, assignmentNode, rightExpression, rightSide);
                    return assignmentNode;

                }

                free (assignmentNode);

            }

        }

    } else if ( tokens [currentTokenNum]->type == VAR ) {

        Node* firstTerm = GetVar ();

        if ( RequireSysOpers (assignment) ) {

            Node* secondTerm = GetExpression ();

            if ( RequireSysOpers (endline) ) {

                Node* assignmentNode = MakeSysOperNode (assignment);
                AddNode (langTree, assignmentNode, firstTerm, leftSide);
                AddNode (langTree, assignmentNode, secondTerm, rightSide);

                return assignmentNode;

            }

        }

    }

    return nullptr;

}

bool RequireSysOpers (int sysOperNum) {

    if ( tokens[currentTokenNum++]->value.sysOper == sysOperNum )
        return true;

    return false;

}

//=====================================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Калькулятор~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Node* GetExpression () {

    Node* firstTerm = GetMul ();
    assert (firstTerm);
    Node* operatorNode = nullptr;

    while (GET_OP == add_ || GET_OP == sub_ ) {

        if (GET_OP == add_)
            operatorNode = MakeSysOperNode (add_);
        if (GET_OP == sub_)
            operatorNode = MakeSysOperNode (sub_);

        currentTokenNum++;

        Node* secondTerm = GetMul ();
        assert (secondTerm);

        AddNode (langTree, operatorNode, firstTerm, leftSide);
        AddNode (langTree, operatorNode, secondTerm, rightSide);

        firstTerm = operatorNode;

    }

    return firstTerm;

}

Node* GetMul () {

    Node* firstTerm = GetBrack ();

    while (GET_OP == mul_ || GET_OP == div_ ) {

        Node* operatorNode = nullptr;

        if (GET_OP == mul_)
            operatorNode = MakeSysOperNode (mul_);
        if (GET_OP == div_)
            operatorNode = MakeSysOperNode (div_);

        currentTokenNum++;

        Node* secondTerm = GetBrack ();

        AddNode (langTree, operatorNode, firstTerm, leftSide);
        AddNode (langTree, operatorNode, secondTerm, rightSide);

        firstTerm = operatorNode;

    }

    return firstTerm;

}

Node* GetBrack () {

    if ( GET_OP == lcB ) {

        currentTokenNum++;
        Node* value = GetExpression ();

        RequireSysOpers (rcB);

        return value;

    } else
        return GetSin ();

}

Node* GetSin () {

    if ( GET_OP == sin_ ) {

        currentTokenNum++;

        Node* value = GetBrack ();

        Node* sinNode = MakeSysOperNode (sin_);
        AddNode (langTree, sinNode, value, rightSide);

        return sinNode;

    } else
        return GetCos ();

}

Node* GetCos () {

    if ( GET_OP == cos_ ) {

        currentTokenNum++;

        Node* value = GetBrack ();

        Node* cosNode = MakeSysOperNode (cos_);
        AddNode (langTree, cosNode, value, rightSide);

        return cosNode;

    } else
        return GetSqrt ();

}

Node* GetSqrt () {

    if ( GET_OP == sqrt_ ) {

        currentTokenNum++;

        Node* value = GetBrack ();

        Node* sqrtNode = MakeSysOperNode (sqrt_);
        AddNode (langTree, sqrtNode, value, rightSide);

        return sqrtNode;

    } else
        return GetFuncExpression ();

}

Node* GetFuncExpression () {

    if ( tokens [currentTokenNum]->type == FUNC ) {

        Node* funcNode = MakeFuncNode (tokens [currentTokenNum]->value.nameStatement);

        currentTokenNum++;

        if ( RequireSysOpers (lcB) ) {

            Node* funcArgsExpression = ReadFuncArgsExpression ();

            if ( !RequireSysOpers (rcB) ) {

                free (funcArgsExpression);
                return nullptr;

            }

            AddNode (langTree, funcNode, funcArgsExpression, rightSide);

            return funcNode;

        } else {

            free (funcNode);
            return nullptr;

        }

    } else
        return GetVar ();

}

Node* ReadFuncArgsExpression () {

    Node* argListFunc = MakeSysOperNode (lcB);
    Node* prevVarDec  = nullptr;

    while ( !RequireSysOpers(rcB) ) {

        currentTokenNum--;

        if ( prevVarDec == nullptr ) {

            Node* newNode = GetExpression ();
            assert (newNode);
            AddNode (langTree, argListFunc, newNode, rightSide);
            prevVarDec = newNode;
            continue;

        }

        if ( RequireSysOpers (comma) ) {

            Node* newNode = GetExpression ();
            assert (newNode);
            AddNode (langTree, prevVarDec, newNode, rightSide);
            prevVarDec = newNode;
            continue;

        }

    }

    currentTokenNum--;

    return argListFunc;

}

Node* GetVar () {

    if ( tokens [currentTokenNum]->type == VAR ) {

        Node* varNode = MakeVarNode ( tokens [currentTokenNum]->value.nameStatement );
        currentTokenNum++;
        return varNode;

    } else
        return GetNum ();

}

Node* GetNum () {

    if ( tokens [currentTokenNum]->type == CONST ) {

        Node* constNode = MakeConstNode ( tokens [currentTokenNum]->value.constValue );

        currentTokenNum++;
        return constNode;

    } else
        return nullptr;

}

//=====================================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//=====================================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Лексический анализ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void PrintTokens () {

    for (size_t i = 0; i < currentTokenNum; i++) {

        if (tokens[i]->type == SYS_OPER)
            printf ("Sys oper = %s\n", nameSysOpers[tokens[i]->value.sysOper]);

        if (tokens[i]->type == FUNC)
            printf ("Func = %s\n", tokens[i]->value.nameStatement);

        if (tokens[i]->type == VAR)
            printf ("Var = %s\n", tokens[i]->value.nameStatement);

        if (tokens[i]->type == CONST)
            printf ("Const = %lg\n", tokens[i]->value.constValue);

    }

}

void  ReadTokens () {

    while ( *expression ) {

        SkipUseless();

        if ( !(*expression) )
            break;

        if ( !ReadNextCell () )
            break;

    }

    PrintTokens ();

}

double ReadNumFromExpression () {

    double value = 0;

    while ( isdigit (*expression) ) {

        value *= 10;
        value += (*expression) - '0';
        expression++;

    }

    return value;

}

bool ReadConstToken () {

    if ( isdigit (*expression) ) {

        double value = ReadNumFromExpression ();
        MakeConstToken (value);

        return true;

    } else return false;

}

bool MultiEqCheck (char toCheck, va_list charList, size_t countChar) {

    char toCmpWith = 0;

    for (size_t charInd = 0; charInd < countChar; charInd++) {

        toCmpWith = va_arg (charList, int);
        if (toCmpWith == toCheck)
            return true;

    }

    return false;

}

char* ReadStatementName (size_t listSize, int toRequire, ...) {

    char* statementName = (char*) calloc(MAX_IDENTIFIER_NAME, sizeof(char));
    size_t symbNum = 0;

    va_list argsToRequireWith = 0;
    argsToRequireWith = (va_list)&toRequire;

    while ( !(MultiEqCheck (*expression, argsToRequireWith, listSize)) ) {

        statementName[symbNum++] = *expression;
        expression++;

    }

    va_end(argsToRequireWith);

    return statementName;

}

bool ReadStatementToken () {

    assert (currentTokenNum != 0);

    char* statementName = nullptr;

    if ( tokens [currentTokenNum - 1]->type == SYS_OPER && tokens[currentTokenNum - 1]->value.sysOper == func ) {

        statementName = ReadStatementName (1, '(');

        MakeFuncToken (statementName);

        return true;

    }

    statementName = ReadStatementName (12, '==', '=', ';', '/', '*', '+', '-', ')', ',', '(', '>', '<');

    if ( *expression == '(' ) {

        printf ("here\n");
        MakeFuncToken (statementName);
        return true;

    }

    MakeVarToken (statementName);

    return true;

}

bool ReadNextCell () {

    SkipUseless ();

    if ( ReadSysOperToken () )
        return true;

    if ( ReadConstToken () )
        return true;

    ReadStatementToken ();

    return true;

}

bool ReadSysOperToken () {

    size_t commandLength = 0;
#define DEF_OPER(nameInLanguage, identifier)      \
                     commandLength = strlen (nameInLanguage);    \
                     if ( STR_EQ(nameInLanguage, expression, commandLength) ) { \
                                                                 \
                        expression += commandLength;             \
                        MakeSysOperToken(identifier);            \
                        return true;                             \
                                                                 \
                     }                                           \
                                                                 \

#include "../data/sysOpers.txt"
#undef DEF_OPER

    return false;

}
//!TODO стоит дополнить списком синтаксических ошибок
void SyntaxError (size_t errNum, char* required) {

    printf ("Syntax error %zu required %s!\n", errNum, required);

}

size_t SkipUseless () {

    size_t resultSkippedChar = 0;

    while ( isspace (*expression) ) {

        expression++;
        resultSkippedChar++;

    }

    if ( *expression == '%' ) {

        while (*expression != '\n' && *expression != '\0') {

            expression++;
            resultSkippedChar++;

        }

        resultSkippedChar++;

        expression++;

    }

    return resultSkippedChar;

}

Node* MakeConstToken (double value) {

    Node* newNode = MakeConstNode (value);

    tokens [currentTokenNum++] = newNode;
    currentTokenStatus = VAR;

    return newNode;

}

Node* MakeVarToken (char* varName) {

    Node* newNode = MakeVarNode (varName);

    tokens [currentTokenNum++] = newNode;
    currentTokenStatus = VAR;

    return newNode;

}

Node* MakeSysOperToken (int operName) {

    Node* newNode = MakeSysOperNode (operName);

    tokens [currentTokenNum++] = newNode;
    currentTokenStatus = SYS_OPER;

    return newNode;

}

Node* MakeFuncToken (char* funcName) {

    Node* newNode = MakeFuncNode (funcName);

    tokens [currentTokenNum++] = newNode;
    currentTokenStatus = FUNC;

    return newNode;

}

//=====================================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void PrintTreeIntoFile (Node* currentNode, FILE* output, const size_t deep) {

    assert (output);

    if (currentNode == nullptr) return;

    for (size_t i = 0; i < deep - 1; i++) fprintf (output, "\t");

    fprintf (output, "{\n");

    for (size_t i = 0; i < deep; i++) fprintf (output, "\t");

    printf ("%d\n", currentNode->type);

    if (currentNode->type == CONST) {

        fprintf (output, "%lg\n", currentNode->value.constValue);

    } else if ( currentNode->type == VAR ) {

        fprintf (output, "%s\n", currentNode->value.nameStatement);

    } else if ( currentNode->type == SYS_OPER ) {

        fprintf (output, "%s\n", nameSysOpers [ currentNode->value.sysOper ] );

    } else {

        fprintf (output, "%s\n", currentNode->value.nameStatement);

    }

    PrintTreeIntoFile (currentNode->leftChild, output, deep + 1);
    PrintTreeIntoFile (currentNode->rightChild, output, deep + 1);

    for (size_t i = 0; i < deep - 1; i++)
        fprintf (output, "\t");

    fprintf (output, "}\n");

}
