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

void free_actor(Actor *character)
{   
    if(character->event_queue)
        al_destroy_event_queue(character->event_queue);
    
    if(character->spritesheet)
        al_destroy_bitmap(character->spritesheet);
    
    if(character->mapping){
        for(int i = 0; i < character->animation.number_of_animations; i++)
        {
            free(character->mapping[i]);
        }

        free(character->mapping);
    }

    if(character->animation.control.sequence_of_sprites)
    {
        free(character->animation.control.sequence_of_sprites);
    }
}



void destroy(void* element, const char* type)
{
    if(!strcmp(type, "Window"))
    {
        Window *window = (Window* ) element;
        al_destroy_display(window->display);
        window->display = NULL;
        
    }
    else if(!strcmp(type, "Events"))
    {
        Events event = (Events) element;
        al_destroy_event_queue(event);
        event = NULL;
    }
    else if(!strcmp(type, "Image"))
    {
        Image image = (Image) element;
        al_destroy_bitmap(image);
        image = NULL;
    }
    else if(!strcmp(type, "Map"))
        free_map((Map*) element);
    else if(!strcmp(type, "Actor"))
        free_actor((Actor*) element);
    else {
        fprintf(stderr, "Error ao liberar memoria pra tipo: %s\n", type);
        exit(-1);
    }
    
}