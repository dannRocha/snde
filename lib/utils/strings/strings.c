

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



char *get_path(const char *string)
{
    int length = 0;

    for(int i = strlen(string) - 1; i > 0; i--)
    {
        if(string[i] == '/')
        {
            length = i + 1;
            break;
        }
    }

    char *path = calloc(length + 1, sizeof(char ));

    for(int i = 0; i < length; i++)
        path[i] = string[i];
    
    path[length + 1] = '\0';

    return path;
}
