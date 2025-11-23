#include <stdio.h>
#include <stdlib.h>
#include "file.h"
int main() {
    char *buffer;
    size_t length;
    read_file("test_input.txt", &buffer, &length);
    printf("%p\n", buffer);
    printf("%s\n", buffer);
    free(buffer);
    return 0;
}
