#include <stdio.h>
#include "../../types/types.h"


void message_error(const char* message){
    fprintf(stderr, "*file: %s\n*function: %s\n*line: %d\n\t - %s\n", __FILE__, __FUNCTION__, __LINE__, message);
}