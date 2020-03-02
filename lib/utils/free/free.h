#ifndef __SNDE_FREE_H__
#define __SNDE_FREE_H__

#include "./free.c"

void free_map(Map *map);
void free_image_src(char **path, int row);
void destroy(void* element, const char* type);

#endif