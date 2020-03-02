#ifndef __MAP_SCENE__
#define __MAP_SCENE__

#include "map.c"


Map load_map(const char *filename, double scale);
void draw_map(Map *map, Window *screen);
void move_camera(Window *screen, Map *map, Actor *character);
// bool collision_map(Map *map, Actor *character,int start_tile, int end_tile);
bool collision_map(void (*callback(Tile *tile)), Map *map, Actor *character, int start_tile, int end_tile);



#endif //__MAP_SCENE__