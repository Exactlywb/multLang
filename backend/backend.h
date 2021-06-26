//
// Created by 1 on 19.12.2020.
//

#ifndef MULTLANG_BACKEND_H
#define MULTLANG_BACKEND_H

#include "../tree/tree.h"
#include "../frontend/frontend.h"

struct varTable;

const size_t maxVarCount   = 10;
static const char*  OutputAsmFile = "asmFile.txt";

static size_t ifNum     = 0;
static size_t whileNum  = 0;

static varTable* MainTable = nullptr;

static size_t freeCellInTable = 0;
static size_t sp = 0;

static bool FirstFunc = true;

struct varTable {

    char* varName   = nullptr;
    int   index     = -1;

};

void Backend                  (Node* root);
void TransformFromTreeIntoAsm (Node* root, FILE* asmOutput);
bool CheckNodeItSelf          (Node* root, FILE* asmOutput);

void ShiftReg                 (FILE* asmOutput);

bool AsmHandleSysOpers        (Node* root, FILE* asmOutput);
bool AsmHandleConsts          (Node* root, FILE* asmOutput);
bool AsmHandleFuncs           (Node* root, FILE* asmOutput);
bool AsmHandleVar             (Node* root, FILE* asmOutput);

#endif //MULTLANG_BACKEND_H
