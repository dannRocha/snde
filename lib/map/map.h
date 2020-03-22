#ifndef __MAP_SCENE__
#define __MAP_SCENE__

#include "map.c"


Map load_map(const char *filename, double scale);
void dynamic_camera(Window *screen, Map *map, Actor *character);
void static_camera(Window *screen, Map *map);
bool collision_map(void (*callback(Tile *tile, Actor *character)), Map *map, Actor *character);



#endif //__MAP_SCENE__