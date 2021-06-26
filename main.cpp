#include "frontend/frontend.h"
#include "textLib/textLib.h"
#include "backend/backend.h"

int main () {

    setlocale (LC_ALL, "Russian");

    Tree mainTree = {};

    char* storage = nullptr;
    ReadIntoStorage ("../data/input.txt", &storage);

    GetG (storage, &mainTree);
    assert (mainTree.root);

    TreeDump (&mainTree);

    Backend (mainTree.root);

    FILE* printTreeInto = fopen ("printedTree.txt", "w");
    PrintTreeIntoFile (mainTree.root, printTreeInto, 1);
    fclose (printTreeInto);
    DeleteSubTree (&mainTree, mainTree.root);

    return 0;

}