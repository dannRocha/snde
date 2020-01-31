#ifndef __MAP_SCENE__
#define __MAP_SCENE__

#include "map.c"


Map load_map(String filename);
void free_map(Map map);
void free_image_src(String* path, int row);
void draw_map(Map map, double scale);
bool collision_map(Map *map, Actor *character,int start_tile, int end_tile);



#endif //__MAP_SCENE__