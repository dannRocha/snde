

char* copy(char* destination, char* source){

    char* start = destination;
 
    while(*source != '\0' && *source != '\n')
    {
        *destination = *source;
        destination++;
        source++;
    }
 
    *destination = '\0'; 
    

    return start;
}