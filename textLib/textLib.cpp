//
// Created by 1 on 18.12.2020.
//

//
// Created by 1 on 16.12.2020.
//

#include "textLib.h"

size_t GetFileSize (FILE* origFile) {

    assert (origFile);

    fseek (origFile, 0, SEEK_END);
    size_t result = ftell (origFile);
    fseek (origFile, 0, SEEK_SET);

    return result;

}

size_t ReadIntoStorage (const char* fileName, char** storage) {

    assert (fileName);
    assert (storage);

    FILE* fileInput = fopen (fileName, "rt");

    assert (fileInput);

    size_t fileSize = GetFileSize (fileInput);

    *storage = (char*)calloc (fileSize + 1, sizeof(*storage));
    assert (*storage);

    fread (*storage, fileSize, sizeof(char), fileInput);
    assert (*storage);

    (*storage)[fileSize + 1] = '\0';

    fclose (fileInput);

    return fileSize;

}

