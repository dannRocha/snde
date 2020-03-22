
void grid_collision_debug(Map map, Actor character, int i, int j)
{
    #ifdef __DEBUGGER_GRID_COLLISION_CHECK__
        al_draw_rectangle(
            j * map.tiles[i][j].width, 
            i * map.tiles[i][j].height,
            character.coord.x + map.tiles[i][j].width, 
            character.coord.y + map.tiles[i][j].height,  
            al_map_rgb(0, 255, 0), 
            2
        );
    #endif //__DEBUGGER_GRID_COLLISION_CHECK__

}


