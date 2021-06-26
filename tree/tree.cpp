//
// Created by 1 on 18.12.2020.
//

//
// Created by 1 on 16.12.2020.
//

#include "tree.h"

Node* CreateNode    (Node* parent, char type, Element* value, Node* leftNode, Node* rightNode) {

    assert (value);

    Node* newNode = (Node*) calloc (1, sizeof(Node));

    newNode->leftChild  = leftNode;
    newNode->rightChild = rightNode;

    newNode->parent = parent;

    newNode->type = type;

    newNode->value = *value;

    return newNode;

}
Node* TreeConstruct (Tree* mainTree, Node* root, const char* fileDumpName) {

    assert (mainTree);
    assert (root);

    mainTree->root = root;

    mainTree->size++;

    FILE* fileDump = fopen (fileDumpName, "w");
    assert (fileDump);

    mainTree->fileDump = fileDump;

    return root;

}
void PrintNodeIntoGraphviz (Tree* mainTree, Node* currentNode) {

    assert (mainTree);
    assert (currentNode);

    static size_t index = 0;

    size_t currentIndex = index;

    if (currentNode->type == FUNC || currentNode->type == VAR)
        fprintf (mainTree->fileDump, "\"box%zu\" [shape = \"record\", label = \"{%s}\"]\n", index, currentNode->value.nameStatement);

    if (currentNode->type == SYS_OPER)
        fprintf (mainTree->fileDump, "\"box%zu\" [shape = \"record\", label = \"{%s}\"]\n", index, nameSysOpers[currentNode->value.sysOper]);

    if (currentNode->type == CONST)
        fprintf (mainTree->fileDump, "\"box%zu\" [shape = \"record\", label = \"{%lg}\"]\n", index, currentNode->value.constValue);

    index++;

    if (currentNode->leftChild != nullptr) {

        size_t nextIndex = index;
        PrintNodeIntoGraphviz (mainTree, currentNode->leftChild);
        fprintf (mainTree->fileDump, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

    }

    if (currentNode->rightChild != nullptr) {

        size_t nextIndex = index;
        PrintNodeIntoGraphviz (mainTree, currentNode->rightChild);
        fprintf (mainTree->fileDump, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

    }


}
void TreeDump (Tree* mainTree) {

    assert (mainTree);

    fprintf (mainTree->fileDump, "digraph List {\n"
                                 "node [shape=\"circle\"]\n");

    PrintNodeIntoGraphviz (mainTree, mainTree->root);
    fprintf (mainTree->fileDump, "}\n");

    fclose (mainTree->fileDump);

    system ("dot -Tpng tree.dot -o tree.png");

}
Node* AddNode (Tree* mainTree, Node* parent, Node* newNode, AddSide whichSide) {

    assert (mainTree);
    assert (parent);
    assert (newNode);

    if (whichSide == leftSide) {

        parent->leftChild = newNode;

    } else if (whichSide == rightSide) {

        parent->rightChild = newNode;

    } else {

        printf ("unexpected side to add new node in function %s!\n", __FUNCTION__);
        return nullptr;

    }

    newNode->parent = parent;

    mainTree->size++;

    return newNode;

}
void  DeleteSubTree (Tree* mainTree, Node* currentNode) {

    assert (mainTree);
    assert (currentNode);

    if (currentNode->leftChild)
        DeleteSubTree (mainTree, currentNode->leftChild);

    if (currentNode->rightChild)
        DeleteSubTree (mainTree, currentNode->rightChild);

    if (currentNode)
        free (currentNode);

    mainTree->size--;

}
void  PrintNode (Node* currentNode) {
    assert (currentNode);

    printf ("%p\n", currentNode);
    printf ("Left child = %p\n", currentNode->leftChild);
    printf ("Right child = %p\n", currentNode->rightChild);
    printf ("Parent = %p\n", currentNode->parent);
    printf ("Type", currentNode->type);

    switch (currentNode->type) {

        case CONST:
            printf ("Value = %lg\n", currentNode->value.constValue);
            break;
        case VAR:
            printf ("Var = %s\n", currentNode->value.nameStatement);
            break;
        case FUNC:
            printf ("Func = %s\n", currentNode->value.nameStatement);
            break;
        case SYS_OPER:
            printf ("Sys oper = %s\n", nameSysOpers[currentNode->value.sysOper]);
            break;

    }

}

Node* MakeConstNode (double value) {

    Element valueNode = {};
    valueNode.constValue = value;

    Node* newNode = CreateNode (nullptr, CONST, &valueNode, nullptr, nullptr);

    return newNode;

}

Node* MakeFuncNode  (char* funcName) {

    Element valueNode = {};
    valueNode.nameStatement = funcName;

    Node* newNode = CreateNode (nullptr, FUNC, &valueNode, nullptr, nullptr);

    return newNode;

}

Node* MakeVarNode  (char* varName) {

    Element valueNode = {};
    valueNode.nameStatement = varName;

    Node* newNode = CreateNode (nullptr, VAR, &valueNode, nullptr, nullptr);

    return newNode;

}
Node* MakeSysOperNode (int sysOper) {

    Element valueNode = {};
    valueNode.sysOper = sysOper;

    Node* newNode = CreateNode (nullptr, SYS_OPER, &valueNode, nullptr, nullptr);

    return newNode;

}
