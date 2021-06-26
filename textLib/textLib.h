//
// Created by 1 on 18.12.2020.
//

#ifndef MULTLANG_TEXTLIB_H
#define MULTLANG_TEXTLIB_H


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <clocale>

size_t ReadIntoStorage   (const char* fileName, char** storage);
size_t GetFileSize       (FILE* origFile);

#endif //MULTLANG_TEXTLIB_H
