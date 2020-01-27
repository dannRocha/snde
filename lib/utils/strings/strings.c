#include "../../types/types.h"


String copy(String destination, String source){

    char *start = destination;
 
    while(*source != '\0' && *source != '\n')
    {
        *destination = *source;
        destination++;
        source++;
    }
 
    *destination = '\0'; 
    

    return start;
}