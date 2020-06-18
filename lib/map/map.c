#include <stdio.h>
#include "transpiler/transpiler.h"
#include "../snde.h"
#include "../types/types.h"
#include "../utils/strings/strings.h"
#include "../utils/debug/debug.h"

enum 
{
    MARGIN = 2,
    MARGIN_VISIBLE = 2
};


static char **path_bitmap(const char *filename)
{
    
    FILE *file = fopen(filename, "r");
    
    int sprites = 0, row = 0, col = 0;
    const int buffer_len = 256;
    char buffer[buffer_len];
    


    if(file == NULL)
        exit(-1);

    fscanf(file, "%d %d %d", &row, &col, &sprites);
    
    
    char **path;

    path =  calloc(sprites, sizeof(char*));

    if(!path)
        message_error("Error ao alocar memoria para PATH ");

    for(int i = 0; i < sprites; i++)
        path[i] = calloc(255, sizeof(char));
    
    int i = 0;
    while(fgets(buffer, buffer_len, file))
    {        
        if(strstr(buffer, ".bmp") != NULL || strstr(buffer, ".png") != NULL)
        {
            copy(path[i], buffer);
            i++;
        }
    }


    fclose(file);

    // if(!file)
        // free(file);


    return path;
}



static void configure_tiles(Map *map, const char *filename, double scale)
{
    
    // map->tiles      = (Tile** ) malloc((map->rows) * sizeof(Tile* ));
    map->bitmap     = (Image* ) calloc(map->number_for_tiles, sizeof(Image ));


    // if(map->tiles == NULL)
    //     fprintf(stderr, "Error ao alocar memória para Matriz de Tiles\n");

    if(map->bitmap == NULL)
        fprintf(stderr, "Error ao alocar memória para Array de Bitmaps\n");


    char **paths = path_bitmap(filename);



    // CARREGA TODOS OS TILES DO MAPA;
    for(int i = 0; i < map->number_for_tiles; i++)
        map->bitmap[i] = load_image(paths[i]);
    

    // ALOCAR MEMORIA PARA A MATRIZ DE COORDENAS E DIMENSÕES DOS TILES
    // for(int i = 0; i < map->rows; i++)    
    //     map->tiles[i] = (Tile* ) malloc((map->cols) * sizeof(Tile ));


    // CONFIGURAR COORDENADAS DOS TILES

    const int EMPTY_TILE = -1;

    for(int i = 0; i < map->rows; i++)
        for(int j = 0; j < map->cols; j++){
            if(map->source[i][j] == EMPTY_TILE)
            {
                map->tiles[i][j].width  =  al_get_bitmap_width(map->bitmap[0]) * map->scale;
                map->tiles[i][j].height =  al_get_bitmap_height(map->bitmap[0]) * map->scale;

                map->tiles[i][j].x = j * map->tiles[i][j].width;
                map->tiles[i][j].y = i * map->tiles[i][j].height;
                
                continue;
            }   
            
            for(int k = 0; k < map->number_for_tiles; k++)
            {
                if(map->source[i][j] == k)
                {
                    map->tiles[i][j].width =  al_get_bitmap_width(map->bitmap[k]) * scale;
                    map->tiles[i][j].height = al_get_bitmap_height(map->bitmap[k]) * scale;
                    map->tiles[i][j].x = j * map->tiles[i][j].width;
                    map->tiles[i][j].y = i * map->tiles[i][j].height;
                }
            }
        }    

    map->width  = (map->cols - 1) * map->tiles[0][0].width;
    map->height = (map->rows - 1) * map->tiles[0][0].height;


    // if(!paths)
    //     free(paths);
    
}



Map load_map(const char *filename, double scale)
{
    if(scale <= 0)
    {
        fprintf(stderr, "\"load_map\": scale nao pode ser negativo ou zero\n");
        exit(-1);
    }
    if(filename == NULL)
    {
        fprintf(stderr, "\n\"load_map\": filename não pode ser nulo\n");
        exit(-1);
    }

    Map maps;
        maps.number_of_tiles_with_collision = 0;

    const char *filename_transpiler = transpiler_map(&maps, filename);
    

    FILE *file = fopen(filename_transpiler, "r");

    if(file == NULL)
    {   
        fprintf(stderr, "Error ao ler arquivo: %s\n", filename_transpiler);
        free(file);
        exit(-1);
    }

    fscanf(file, "%d %d %d", &maps.rows, &maps.cols, &maps.number_for_tiles);
    

    maps.source = (int** ) calloc(maps.rows, sizeof(int* ));
    maps.scale = scale;


    if(maps.source == NULL)
        fprintf(stderr, "Error ao alocar memoria para MAP: SOURCE\n");


    for(int i = 0; i < maps.rows; i++)
        maps.source[i] = (int* ) calloc (maps.cols, sizeof(int));

    // RESETAR A MEMORIA ALOCADA
    for(int i = 0; i < maps.rows; i++)
        for(int j = 0; j < maps.cols; j++)
            maps.source[i][j] = 0;


    // PULA O FLUXO DE LEITURA ATÈ A MATRIZ;
    for(int i = 0; i <= maps.number_for_tiles; i++)
    {
        char _;
        while((_ = fgetc(file)) != EOF)
            if(_=='\n') break;
    }
    
    // LER MATRIZ DE DADOS DO MAPA
    for(int i = 0; i < maps.rows; i++)
        for(int j = 0; j < maps.cols; j++)
            fscanf(file, "%d", &maps.source[i][j]);
   

    fclose(file);


    configure_tiles(&maps, filename_transpiler, scale);


    maps.visible_tile_height_max = maps.rows;
    maps.visible_tile_height_min = 0;
    maps.visible_tile_width_max = maps.cols;
    maps.visible_tile_width_min = 0;



    return maps;

}



static void draw_map(Map *map, Window *screen)
{  

    for(int i = map->visible_tile_height_min; i < map->visible_tile_height_max; i++)
        for(int j = map->visible_tile_width_min; j < map->visible_tile_width_max; j++)
            for(int k = 0; k < map->number_for_tiles; k++)
            {   
                if(map->source[i][j] == k)
                {                    
                    draw_image(map->bitmap[k], map->tiles[i][j].x, map->tiles[i][j].y, map->scale, 0);
                    break;
                }
            }
}



bool collision_map(void (*callback(Tile *tile, Actor *character)), Map *map, Actor *character)
{
    
    int row_e = (character->coord.y + character->dimen.h) / map->tiles[0][0].height;
    int col_e = (character->coord.x + character->dimen.w) / map->tiles[0][0].width;
    int row_s = (character->coord.y / map->tiles[0][0].height) ;
    int col_s = (character->coord.x / map->tiles[0][0].width); 

    const int MARGIN = 1;

    int row_start   = row_s;
    int row_end     = row_e + 2;
    int col_start   = col_s; 
    int col_end     = col_e + 2; 
    if(row_end > map->rows)
        row_end = map->rows;
    
    if(col_end > map->cols)
        col_end = map->cols;

    if(row_start < 0) row_start = 0; 
    else if(row_end > map->rows) row_end = map->rows;
    
    printf("%d x %d\n", row_end-row_start, col_end - col_start);

    if(col_start < 0) col_start = 0;
    else if(col_end > map->cols) col_end = map->cols;

    if(map->tiles[0][0].width > character->dimen.w){
        col_end     = 1;
        col_start   = 0;
    }
    if(map->tiles[0][0].height > character->dimen.h){
        row_end     = 1;
        row_start   = 0;
    }

    bool stage = false;
    
    for(int i = row_start; i < row_end; i++)
        for(int j = col_start; j < col_end; j++)
        {
            
            grid_collision_debug(*map, *character, i, j);

            if(map->source[i][j] >= 0 && map->source[i][j] < map->number_of_tiles_with_collision)
            {
                if( 
                    character->coord.x + character->dimen.w >= map->tiles[i][j].x &&
                    character->coord.x <= map->tiles[i][j].x + map->tiles[i][j].width &&
                    character->coord.y <= map->tiles[i][j].y + map->tiles[i][j].height &&
                    character->coord.y + character->dimen.h >= map->tiles[i][j].y 
                ){

                    if(callback != NULL)
                        callback(&map->tiles[i][j], character);
                    
                                    
                    #ifdef __DEBUGGER_COLLISION_MAP__
                                        
                        int border = 4;

                        al_draw_rectangle(
                            character->coord.x, 
                            character->coord.y, 
                            character->dimen.w + character->coord.x, 
                            character->dimen.h + character->coord.y, 
                            al_map_rgb(0, 255, 0), 
                            border
                        );


                        al_draw_rectangle(
                            map->tiles[i][j].x,
                            map->tiles[i][j].y, 
                            map->tiles[i][j].width + map->tiles[i][j].x, 
                            map->tiles[i][j].height + map->tiles[i][j].y, 
                            al_map_rgb(255, 255, 0), 
                            border
                        );
                    #endif //__DEBUGGER_COLLISION_MAP__ 

                    stage = true;
                }
            }
        }

    return stage;
}



static void set_visible_tiles(Window *screen, Map *map)
{
    int size_block = screen->width / map->tiles[0][0].width;

    map->visible_tile_width_min     = 0;
    map->visible_tile_height_min    = 0;
    
    map->visible_tile_width_max     = size_block + MARGIN_VISIBLE;
    map->visible_tile_height_max    = size_block + MARGIN_VISIBLE;

    // AUTUALIZAR OS TILES VISIVEIS DA COLUNA
    if(map->scroll.x >= 0 )
    {
        map->visible_tile_width_max   = ((screen->width / map->tiles[0][0].width ) + (map->scroll.x / map->tiles[0][0].width)) + MARGIN_VISIBLE;
        map->visible_tile_width_min   = map->visible_tile_height_max - size_block - MARGIN_VISIBLE;
        
        
        if(map->visible_tile_width_min < 0) 
            map->visible_tile_width_min = 0;
        if(map->visible_tile_width_max > map->cols)
            map->visible_tile_width_max = map->cols;

    }
    
    // AUTUALIZAR OS TILES VISIVEIS DA LINHA
    if(map->scroll.y >= 0)
    {
        map->visible_tile_height_max    = ((screen->height / map->tiles[0][0].height ) + (map->scroll.y / map->tiles[0][0].height)) + MARGIN_VISIBLE;
        map->visible_tile_height_min    = map->visible_tile_height_max - size_block - MARGIN_VISIBLE;

        if(map->visible_tile_height_min < 0) 
            map->visible_tile_height_min = 0;
        if(map->visible_tile_height_max > map->rows)
            map->visible_tile_height_max = map->rows;
    }
}



static void set_scroll_camera(Window *screen, Map *map, Actor *character)
{
 

    // CONFIGURAR E CENTRALIZAR A CAMERA
    map->scroll.x = -(screen->width / 2) + (character->coord.x + character->dimen.w);
    map->scroll.y = -(screen->height / 2) + (character->coord.y + character->dimen.h);

   

    // LIMITAR O MOVIMENTO DA CAMERA NA ALTURA
    if(character->coord.y + character->dimen.h + screen->height / 2 >= map->height + map->tiles[0][0].height)
    {
        map->scroll.y = map->height - screen->height + map->tiles[0][0].height;
    }

    // LIMITAR O MOVIMENTO DA CAMERA NA LARGURA
    if(character->coord.x + character->dimen.w + screen->width  / 2 >= map->width + map->tiles[0][0].width)
    {
        map->scroll.x = map->width - screen->width + map->tiles[0][0].width;
    }


    if(map->scroll.x < 0) map->scroll.x = 0;
    if(map->scroll.y < 0) map->scroll.y = 0;


    if(map->height < screen->height)
        map->scroll.y = 0;


}



void dynamic_camera(Window *screen, Map *map, Actor *character)
{

    if(!screen || !map || !character)
    {
        fprintf(stderr, "\nError: Paramentros não podem ser nulos \"dynamic_camera\"\n");
        exit(-1);
    }

    static Camera camera;

    set_scroll_camera(screen, map, character);
    set_visible_tiles(screen, map);
    
    al_identity_transform(&camera);
    al_translate_transform(&camera, -map->scroll.x, -map->scroll.y);
    al_use_transform(&camera); 

    draw_map(map, screen);
}



void static_camera(Window *screen, Map *map)
{
    draw_map(map, screen);
}

