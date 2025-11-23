#include "file.h"
#include <stdlib.h>
#include <stdio.h>

void read_file(const char* path, char** buffer, size_t* length)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        *buffer = NULL;
        *length = 0;
        return;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = (char*)malloc(*length + 1);
    fread(*buffer, 1, *length, file);
    (*buffer)[*length] = '\0';

    fclose(file);
}