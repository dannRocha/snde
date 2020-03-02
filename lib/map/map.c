#include <stdio.h>
#include "../snde.h"
#include "../types/types.h"


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

    path =  malloc((sprites) * sizeof(char*));

    if(!path)
        message_error("Error ao alocar memoria para PATH ");

    for(int i = 0; i < sprites; i++)
        path[i] = malloc((255) * sizeof(char));
    
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

    if(!file)
        free(file);


    return path;
}



static void configure_tiles(Map *map, const char *filename, double scale)
{
    
    map->tiles      = (Tile** ) malloc((map->rows) * sizeof(Tile* ));
    map->bitmap     = (Image* ) malloc((map->number_for_tiles * sizeof(Image )));


    if(map->tiles == NULL)
        fprintf(stderr, "Error ao alocar memória para Matriz de Tiles\n");

    if(map->bitmap == NULL)
        fprintf(stderr, "Error ao alocar memória para Array de Bitmaps\n");


    char **paths = path_bitmap(filename);



    // CARREGA TODOS OS TILES DO MAPA;
    for(int i = 0; i < map->number_for_tiles; i++)
        map->bitmap[i] = load_image(paths[i]);
    

    // ALOCAR MEMORIA PARA A MATRIZ DE COORDENAS E DIMENSÕES DOS TILES
    for(int i = 0; i < map->rows; i++)    
        map->tiles[i] = (Tile* ) malloc((map->cols) * sizeof(Tile ));


    // CONFIGURAR COORDENADAS DOS TILES

    const int EMPTY_TILE = -1;

    for(int i = 0; i < map->rows; i++)
        for(int j = 0; j < map->cols; j++){
            if(map->source[i][j] == EMPTY_TILE)
            {
                
                map->tiles[i][j].width = al_get_bitmap_width(map->bitmap[0]) * scale;
                map->tiles[i][j].height = al_get_bitmap_height(map->bitmap[0]) * scale;
                map->tiles[i][j].x = j * map->tiles[i][j].width;
                map->tiles[i][j].y = i * map->tiles[i][j].height;
                
                continue;
            }   
            
            for(int k = 0; k < map->number_for_tiles; k++)
            {
                if(map->source[i][j] == k)
                {
                    map->tiles[i][j].width = al_get_bitmap_width(map->bitmap[k]) * scale;
                    map->tiles[i][j].height = al_get_bitmap_height(map->bitmap[k]) * scale;
                    map->tiles[i][j].x = j * map->tiles[i][j].width;
                    map->tiles[i][j].y = i * map->tiles[i][j].height;
                }
            }
        }    

    map->width  = (map->cols - 1) * map->tiles[0][0].width;
    map->height = (map->rows - 1) * map->tiles[0][0].height;


    if(!paths)
        free(paths);
    
}



Map load_map(const char *filename, double scale)
{

    Map maps;

    FILE *file = fopen(filename, "r");

    if(file == NULL)
    {   fprintf(stderr, "Error ao ler arquivo: %s\n", filename);
        free(file);
        exit(-1);
    }

    fscanf(file, "%d %d %d", &maps.rows, &maps.cols, &maps.number_for_tiles);

    maps.source = (int** ) malloc((maps.rows) * sizeof(int* ));
    maps.scale = scale;


    if(maps.source == NULL)
        fprintf(stderr, "Error ao alocar memoria para MAP: SOURCE\n");


    for(int i = 0; i < maps.rows; i++)
        maps.source[i] = (int* ) malloc ((maps.cols) * sizeof(int));

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


    configure_tiles(&maps, filename, scale);


    maps.visible_tile_height_max = maps.rows;
    maps.visible_tile_height_min = 0;
    maps.visible_tile_width_max = maps.cols;
    maps.visible_tile_width_min = 0;



    return maps;

}



void draw_map(Map *map, Window *screen)
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


bool collision_map(void (*callback(Tile *tile)), Map *map, Actor *character, int start_tile, int end_tile)
{
    
    int row = (character->coord.y / map->tiles[0][0].height);
    int col = (character->coord.x / map->tiles[0][0].width); 
    
    int row_start   = row - MARGIN;
    int row_end     = row + MARGIN;
    int col_start   = col - MARGIN; 
    int col_end     = col + MARGIN; 


    if(row_start <= 0) row_start = 0;
    else if(row_end >= map->rows) row_end = map->rows;
            
    if(col_start <= 0) col_start = 0;
    else if(col_end >= map->cols) col_end = map->cols;
    

    for(int i = row_start; i < row_end; i++)
        for(int j = col_start; j < col_end; j++)           
            for(int tile = start_tile; tile < end_tile; tile++)
            {
                if(map->source[i][j] == tile)
                {
                    if( character->coord.x + character->dimen.w >= map->tiles[i][j].x &&
                        character->coord.x <= map->tiles[i][j].x + map->tiles[i][j].width &&
                        character->coord.y <= map->tiles[i][j].y + map->tiles[i][j].height &&
                        character->coord.y + character->dimen.h >= map->tiles[i][j].y
                    ){

                        if(callback != NULL)
                            callback(&map->tiles[i][j]);

                        return true;
                    }
                }
            }

    return false;
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



static void set_scroll_camera(Window *screen, Map *map, Actor *character){
 

    // CONFIGURAR E CENTRALIZAR A CAMERA
    map->scroll.x = -(screen->width / 2) + (character->coord.x + character->dimen.w);
    map->scroll.y = -(screen->height / 2) + (character->coord.y + character->dimen.h);

   

    // LIMITAR O MOVIMENTO DA CAMERA NA ALTURA
    if(character->coord.y - character->dimen.h  +  screen->height / 2 >= map->height)
    {
        map->scroll.y = map->height - screen->height + map->tiles[0][0].height;
    }

    // LIMITAR O MOVIMENTO DA CAMERA NA LARGURA
    if(character->coord.x - character->dimen.w+ screen->width  / 2 >= map->width)
    {
        map->scroll.x = map->width - screen->width + map->tiles[0][0].width;
    }


    if(map->scroll.x < 0) map->scroll.x = 0;
    if(map->scroll.y < 0) map->scroll.y = 0;
}

void move_camera(Window *screen, Map *map, Actor *character)
{
    static Camera camera;

    set_scroll_camera(screen, map, character);
    set_visible_tiles(screen, map);
    
    al_identity_transform(&camera);
    al_translate_transform(&camera, -map->scroll.x, -map->scroll.y);
    al_use_transform(&camera); 

}
