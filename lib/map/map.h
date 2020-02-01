#ifndef __MAP_SCENE__
#define __MAP_SCENE__

#include "map.c"


Map load_map(char* filename);
void free_map(Map *map);
void free_image_src(char** path, int row);
void draw_map(Map* map, double scale);
bool collision_map(Map *map, Actor *character,int start_tile, int end_tile);



#endif //__MAP_SCENE__