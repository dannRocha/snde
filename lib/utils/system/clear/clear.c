#include <stdlib.h>

void clear_log()
{
  #ifdef WINDOWS
    system("cls");
  #else
    system("clear");
  #endif
}
