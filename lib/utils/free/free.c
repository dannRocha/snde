#include "../../types/types.h"




void free_map(Map *map)
{

    Tile tile;
        tile.id     = 0;
        tile.label  = 0;
        tile.width  = 0;
        tile.height = 0;
        tile.x      = 0;
        tile.y      = 0;

    for(int i = 0; i < map->rows; i++){
        for(int j = 0; i < map->cols; j++){
            map->source[i][j] = 0;
            map->tiles[i][j] = tile;
        }
        
        free(map->source[i]);        
        free(map->tiles[i]);
    }

    for(int i = 0; i < map->number_for_tiles; i++)
        al_destroy_bitmap(map->bitmap[i]);


    free(map->source);
}



void free_image_src(char **path, int row)
{
    for(int i = 0; i < row; i++)
        free(path[i]);
}



void destroy(void* element, const char* type)
{
    if(!strcmp(type, "Window"))
        al_destroy_display((Display) element);
    else if(!strcmp(type, "Events"))
        al_destroy_event_queue((Events) element);
    else if(!strcmp(type, "Image"))
        al_destroy_bitmap((Image) element);
    else if(!strcmp(type, "Map"))
        free_map((Map*) element);
    else{
        fprintf(stderr, "Error ao liberar memoria pra tipo: %s\n", type);
        exit(-1);
    }
    
}