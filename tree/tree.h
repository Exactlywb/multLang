//
// Created by 1 on 18.12.2020.
//

#ifndef MULTLANG_TREE_H
#define MULTLANG_TREE_H

//
// Created by 1 on 16.12.2020.
//

#ifndef LANGUAGE_TREE_H
#define LANGUAGE_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

enum NodeTypes {

    CONST,
    VAR,
    SYS_OPER,
    FUNC,
    NOTYPE

};

enum AddSide {

    leftSide,
    rightSide

};

const char nameSysOpers[30][15] = {

#define DEF_OPER(nameInLang, nameAfterFrontend) #nameAfterFrontend,
#include "../data/sysOpers.txt"
#undef DEF_OPER

};

enum SysOpers {

#define DEF_OPER(nameInLang, nameAfterFrontend) nameAfterFrontend,
#include "../data/sysOpers.txt"
#undef DEF_OPER

};

union Element {

    int sysOper;
    double constValue;
    char* nameStatement;

};

struct Node;

struct Tree {

    Node* root;
    size_t size;

    FILE* fileDump;

};

struct Node {

    Node* leftChild;
    Node* rightChild;

    Node* parent;

    char type;

    Element value;

};

void  PrintNode     (Node* currentNode);
Node* CreateNode    (Node* parent, char type, Element* value, Node* leftNode, Node* rightNode);
Node* TreeConstruct (Tree* mainTree, Node* root, const char* fileDumpName);
void  TreeDump      (Tree* mainTree);
Node* AddNode       (Tree* mainTree, Node* parent, Node* newNode, AddSide whichSide);
void  DeleteSubTree (Tree* mainTree, Node* currentNode);
Node* MakeConstNode (double value);
Node* MakeFuncNode  (char* funcName);
Node* MakeVarNode   (char* varName);
Node* MakeSysOperNode (int sysOper);

#endif //LANGUAGE_TREE_H


#endif //MULTLANG_TREE_H
