//
// Created by 1 on 19.12.2020.
//

#include "backend.h"

#define R_TYPE  root->type
#define R_VALUE_SYS root->value.sysOper
#define R_VALUE_STATE root->value.nameStatement
#define STR_EQ(str1, str2) !strcmp(str1, str2)

void Backend (Node* root) {

    FILE* asmOutput = fopen (OutputAsmFile, "wb");
    assert (asmOutput);

    MainTable = (varTable*) calloc (1, sizeof(varTable));

    for (size_t indexVar = 0; indexVar < maxVarCount; indexVar++) {

        MainTable [indexVar].varName = (char*) calloc (MAX_IDENTIFIER_NAME, sizeof(char));

    }

    TransformFromTreeIntoAsm (root, asmOutput);

    fclose (asmOutput);

    for (size_t indexVar = 0; indexVar < maxVarCount; indexVar++) {

        free ( MainTable [indexVar].varName );

    }

    free (MainTable);

}

void TransformFromTreeIntoAsm (Node* root, FILE* asmOutput) {

    assert (root);
    assert (asmOutput);

    CheckNodeItSelf (root, asmOutput);

}

bool CheckNodeItSelf (Node* root, FILE* asmOutput) {

    assert (root);
    assert (asmOutput);

    switch (R_TYPE) {

        case VAR:
            AsmHandleVar (root, asmOutput);
            break;
        case CONST:
            AsmHandleConsts (root, asmOutput);
            break;
        case SYS_OPER:
            AsmHandleSysOpers (root, asmOutput);
            break;
        case FUNC:
            AsmHandleFuncs (root, asmOutput);
            break;
        default:
            printf ("Unexpected type of Node* root in function %s!\n", __FUNCTION__);
            return false;

    }

    return true;

}

#define TRANS_LEFT_START TransformFromTreeIntoAsm (root->leftChild, asmOutput);
#define TRANS_RIGHT_START TransformFromTreeIntoAsm (root->rightChild, asmOutput);

bool AsmHandleSysOpers (Node* root, FILE* asmOutput) {

    assert (root);
    assert (asmOutput);

    switch (R_VALUE_SYS) {

        case lcB: //!TODO тут надо аккуратно ещё раз посмотреть
            if (root->leftChild)
                TRANS_LEFT_START
            break;
        case statement:
            if (root->leftChild)
                TRANS_LEFT_START
            if (root->rightChild)
                TRANS_RIGHT_START
            break;
        case concat:
            if (root->leftChild)
                TRANS_LEFT_START
            if (root->rightChild)
                TRANS_RIGHT_START
            break;
        case assignment:
            if (root->leftChild)
                TRANS_LEFT_START
            break;
        case print_:
            if (root->rightChild)
                TRANS_RIGHT_START
            fprintf (asmOutput, "OUT\n");
            break;
        case scan_:
            fprintf (asmOutput, "IN\n");
            if (root->rightChild)
                TRANS_RIGHT_START
            break;
        case add_:
            if (root->leftChild)
                TRANS_LEFT_START
            if (root->rightChild)
                TRANS_RIGHT_START
            fprintf (asmOutput, "ADD\n");
            break;
        case mul_:
            if (root->leftChild)
                TRANS_LEFT_START
            if (root->rightChild)
                TRANS_RIGHT_START
            fprintf (asmOutput, "MUL\n");
            break;
        case sqrt_:
            if (root->rightChild)
                TRANS_RIGHT_START
            fprintf (asmOutput, "SQRT\n");
            break;
        case div_:
            if (root->leftChild)
                TRANS_LEFT_START
            if (root->rightChild)
                TRANS_RIGHT_START
            fprintf (asmOutput, "DIV\n");
            break;
        case while_:
            fprintf (asmOutput, "WHILE_%d:\n", ++whileNum);
            TransformFromTreeIntoAsm ( root->leftChild->leftChild, asmOutput);
            TransformFromTreeIntoAsm ( root->rightChild->rightChild, asmOutput);
            TransformFromTreeIntoAsm ( root->leftChild, asmOutput);
            whileNum++;
            TransformFromTreeIntoAsm (root->rightChild, asmOutput);
            fprintf (asmOutput, "JMP : WHILE_%d\n", whileNum);
            fprintf (asmOutput, "WHILE_%d:\n", whileNum - 1);
            break;
        case sub_:
            if (root->leftChild)
                TRANS_LEFT_START
            if (root->rightChild)
                TRANS_RIGHT_START
            fprintf (asmOutput, "SUB\n");
            break;
        case return_:
            if (root->rightChild)
                TRANS_RIGHT_START

            fprintf (asmOutput, "RET\n");
            break;
        case func:
            if (root->leftChild)
                TRANS_LEFT_START
            if (root->rightChild)
                TRANS_RIGHT_START
            if (FirstFunc) {

                fprintf (asmOutput, "HLT\n");
                FirstFunc = false;

            }
            break;
        case loader:
            if (root->rightChild)
                TRANS_RIGHT_START
            if (root->leftChild)
                TRANS_LEFT_START
            break;
        case more_:
            fprintf (asmOutput, "JBE : ");
            break;
        case less_:
            fprintf (asmOutput, "JAE : ");
            break;
        case eq:
            fprintf (asmOutput, "JNE : ");
            break;
        case if_:

            TransformFromTreeIntoAsm (root->leftChild->leftChild, asmOutput);
            TransformFromTreeIntoAsm (root->leftChild->rightChild, asmOutput);

            AsmHandleSysOpers (root->leftChild, asmOutput);

            fprintf (asmOutput, "IF_%zu\n", ++ifNum);
            TransformFromTreeIntoAsm (root->rightChild, asmOutput);
            fprintf (asmOutput, "IF_%zu:\n", ifNum);
            break;
        default:
            printf ("Got it : %s\n", nameSysOpers [root->value.sysOper]);
            printf ("error! unexpected sys oper type!\n");
            return false;

    }

    return true;

}

bool AsmHandleConsts (Node* root, FILE* asmOutput) {

    fprintf (asmOutput, "PUSH %lg\n", root->value.constValue);
    return true;

}

bool AsmHandleFuncs (Node* root, FILE* asmOutput) {

    if (root->parent->value.sysOper == func) { //если это лишь определение функции, а не её вызов

        fprintf (asmOutput, "%s:\n", root->value.nameStatement);

        if (root->leftChild)
            TransformFromTreeIntoAsm (root->leftChild, asmOutput);

        if (root->rightChild)
            TransformFromTreeIntoAsm (root->rightChild, asmOutput);

        return true;

    }

    //далее если это вызов функции
    if (root->leftChild)
        TransformFromTreeIntoAsm (root->leftChild, asmOutput); // эта херь будет просто пушить перед новым bp (переставляя sp, конечно же)
    ShiftReg (asmOutput); //переставляем bp и sp (перед bp в оперативке у нас будет bpl - предыдущий base pointer, чтобы вернуть bp)
    fprintf (asmOutput, "CALL : %s\n", root->value.nameStatement); // вызов нашей функции в процессоре.

    return true;

}

void ShiftReg (FILE* asmOutput) {

    // * RCX - sp
    // * RBX - bp
    // * RAX - bp_l (предыдущий bp)

    // Считаем, что sp - индекс на первый свободный элемент (все за ним свободные)

    // 1) Данная функция сохраняет изначальный bp в RAX
    // 2) Затем она передвигает bp на sp + 1
    // 3) Потом она записывает в оперативную ячейку под номером sp изначальный bp
    // 4) Передвигаем sp на bp-ю ячейку

    fprintf (asmOutput,
             "PUSH rbx\n"
             "POP rax\n" //сохранили bp в RAX
             "PUSH rcx + 1\n"
             "POP rbx\n" //передвинули bp на sp + 1
             "PUSH rax\n"
             "POP [ rcx ]\n" //убрали bp_l на sp-ю ячейку
             "PUSH rbx\n"
             "POP rcx\n"); //передвинули sp

}

bool IsMathSysOper (Node* root) {

    if (root->type == SYS_OPER) {

        switch ( root->value.sysOper ) {

            case add_ :
                return true;
            case sub_ :
                return true;
            case mul_ :
                return true;
            case div_ :
                return true;
            case sin_ :
                return true;
            case cos_ :
                return true;
            case more_ :
                return true;
            case less_ :
                return true;
            case eq    :
                return true;
            case  sqrt_ :
                return true;
            default :
                return false;

        }

    } else
        return false;

}

bool AsmHandleVar (Node* root, FILE* asmOutput) {

    int indexInTable = -1;

    for (int index = 0; index < maxVarCount; index++) { //ищем переменную с таким же названием, чтобы не создавать для каждой функции отдельную табличку

        if ( STR_EQ(root->value.nameStatement, MainTable[index].varName) ) {

            indexInTable = index;

            //далее перебираем случаи использования переменных

            if ( root->parent->type == SYS_OPER && root->parent->value.sysOper == print_ ) //случай вывода
            {

                fprintf (asmOutput, "PUSH [ %d ]\n", MainTable [indexInTable].index);

                break;

            }

            if ( root->parent->type == SYS_OPER && root->parent->value.sysOper == scan_ ) //случай сканирования
            {

                fprintf (asmOutput, "POP [ %d ]\n", MainTable [indexInTable].index);

                break;

            }

            if ( root->parent->type == SYS_OPER && ( root->parent->value.sysOper == assignment )
                                                && ( root->parent->leftChild == root           ) ) { //случай присваивания

                if (root->parent->rightChild)
                    TransformFromTreeIntoAsm (root->parent->rightChild, asmOutput);
                fprintf (asmOutput, "POP [ %d ]\n", MainTable [indexInTable].index);
                break;

            }

            if ( root->parent->type == SYS_OPER && IsMathSysOper (root->parent) ) { //если у нас переменная в мат. выражении

                fprintf (asmOutput, "PUSH [ %d ]\n", MainTable [indexInTable].index);
                break;

            }

            if ( root->parent->type == VAR || ( root->parent->type == SYS_OPER && root->parent->value.sysOper == lcB) ) { //если переменная в перечислении для функции

                fprintf (asmOutput, "PUSH [ %d ]\n", MainTable[indexInTable].index);
                break;

            }

            if ( root->parent->type == SYS_OPER && root->parent->value.sysOper == return_ ) {

                fprintf (asmOutput, "PUSH [ %d ]\n", MainTable[indexInTable].index);
                break;

            }

        }

    }

    if (indexInTable == -1) { //если переменной с таким названием ещё нету

        MainTable [freeCellInTable].varName = root->value.nameStatement;
        MainTable [freeCellInTable++].index = sp;
        printf ("Curr index = %d, curr name = %s, indexInTable %d\n", sp, root->value.nameStatement, indexInTable);

        if (root->parent->rightChild)
            TransformFromTreeIntoAsm (root->parent->rightChild, asmOutput);


        fprintf (asmOutput, "POP [ %d ]\n"
                            "PUSH rcx + 1\n"
                            "POP rcx\n", sp);

        sp++;

    }

    return true;

}
