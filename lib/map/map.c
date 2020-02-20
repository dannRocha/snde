#include <stdio.h>
#include "../snde.h"
#include "../types/types.h"


enum 
{
    MARGIN = 2,
    MARGIN_VISIBLE_WIDTH = 2
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
    
    while(fgets(buffer, buffer_len, file))
    {
        
        static int i = 0;    
        
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
    
    map->tile.src = malloc((map->tile.quantity) * sizeof(Image ));
    map->tile.coord = (Coord** ) malloc((map->rows) * sizeof(Coord *));
    map->tile.dimen = (Dimension** ) malloc((map->rows) * sizeof(Dimension* ));
    


    if(map->tile.src == NULL)
        message_error("Error ao alocar memoria");

    char **paths = path_bitmap(filename);


    for(int i = 0; i < map->tile.quantity; i++)
        map->tile.src[i] = (Image) malloc((map->tile.quantity) * sizeof(Image));


    // CARREGA TODOS OS TILES DO MAPA;
    for(int i = 0; i < map->tile.quantity; i++)
        map->tile.src[i] = load_image(paths[i]);

    // ALOCAR MEMORIA PARA A MATRIZ DE COORDENAS E DIMENSÕES DOS TILES
    for(int i = 0; i < map->rows; i++)
    {
        map->tile.coord[i] = (Coord *) malloc ((map->cols) * sizeof(Coord));
        map->tile.dimen[i] = (Dimension *) malloc ((map->cols) * sizeof(Dimension));
    }

    // CONFIGURAR COORDENADAS DOS TILES
    for(int i = 0; i < map->rows; i++)
        for(int j = 0; j < map->cols; j++)
            for(int k = 0; k < map->tile.quantity; k++)
            {
                if(map->source[i][j] == k){
                    map->tile.dimen[i][j].w = al_get_bitmap_width(map->tile.src[k]) * scale;
                    map->tile.dimen[i][j].h = al_get_bitmap_height(map->tile.src[k])  * scale;
                    map->tile.coord[i][j].x = j * map->tile.dimen[i][j].w;
                    map->tile.coord[i][j].y = i * map->tile.dimen[i][j].h;
                }
            }    

    map->width  = (map->cols - 1) * map->tile.dimen[0][0].w;
    map->height = (map->rows - 1) * map->tile.dimen[0][0].h;


    if(!paths)
        free(paths);
}



Map load_map(const char *filename, double scale)
{

    Map maps;

    FILE *file = fopen(filename, "r");

    fscanf(file, "%d %d %d", &maps.rows, &maps.cols, &maps.tile.quantity);


    if(file == NULL)
    {
        message_error("Error ao tentar lê o arquivo");
        free(file);
        exit(-1);
    }


    
    maps.source = (int** ) malloc((maps.rows) * sizeof(int* ));
    maps.scale = scale;


    if(maps.source == NULL)
        message_error("Error ao alocar memoria para MAP");


    for(int i = 0; i < maps.rows; i++)
        maps.source[i] = (int* ) malloc ((maps.cols) * sizeof(int));

    // RESETAR A MEMORIA ALOCADA
    for(int i = 0; i < maps.rows; i++)
        for(int j = 0; j < maps.cols; j++)
            maps.source[i][j] = 0;


    // PULA O FLUXO DE LEITURA ATÈ A MATRIZ;
    for(int i = 0; i <= maps.tile.quantity; i++)
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

    return maps;

}



void free_map(Map *map)
{
    for(int i = 0; i < map->rows; i++)
        free(map->source[i]);        

    free(map->source);
}



void free_image_src(char **path, int row)
{
    for(int i = 0; i < row; i++)
        free(path[i]);
}



void draw_map(Map *map, Window *screen)
{

    int size_block_width    = screen->width  / map->tile.dimen[0][0].w;
    int size_block_height   = screen->height / map->tile.dimen[0][0].h;
    
    if(!map->check_visible)
    {
        map->visible_tile_width     = size_block_width  + 1;
        map->visible_tile_height    = size_block_height + 1;
        map->check_visible          = true;
    }


    for(int i = map->visible_tile_height - size_block_height; i < map->visible_tile_height; i++)
        for(int j = map->visible_tile_width - size_block_width - MARGIN_VISIBLE_WIDTH; j < map->visible_tile_width + MARGIN_VISIBLE_WIDTH; j++)
            for(int k = 0; k < map->tile.quantity; k++)
            {
                if(map->source[i][j] == k)
                {                    
                    draw_image(map->tile.src[k], map->tile.coord[i][j].x, map->tile.coord[i][j].y, map->scale, 0);
                    
                    break;
                }
            }
  
}



bool collision_map(Map *map, Actor *character, int start_tile, int end_tile)
{
    
    int row = (character->coord.y / map->tile.dimen[0][0].h);
    int col = (character->coord.x / map->tile.dimen[0][0].w); 
    
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
                    if( character->coord.x + character->dimen.w >= map->tile.coord[i][j].x &&
                        character->coord.x <= map->tile.coord[i][j].x + map->tile.dimen[i][j].w &&
                        character->coord.y <= map->tile.coord[i][j].y + map->tile.dimen[i][j].h &&
                        character->coord.y + character->dimen.h >= map->tile.coord[i][j].y
                    ){
                        return true;
                    }
                }
            }

    return false;
}


void move_camera(Window screen, Map *map, Actor *character)
{


    static Camera camera;
    static Coord scroll;
   

    // CONFIGURAR E CENTRALIZAR A CAMERA
    scroll.x = -(screen.width / 2) + (character->coord.x + character->dimen.w);
    scroll.y = -(screen.height / 2) + (character->coord.y + character->dimen.h);

    
    // LIMITAR O MOVIMENTO DA CAMERA NA ALTURA
    if(character->coord.y + screen.height / 2 >= map->height)
        scroll.y = map->height - screen.height + map->tile.dimen[0][0].h;
    
    // LIMITAR O MOVIMENTO DA CAMERA NA LARGURA
    if(character->coord.x + screen.width  / 2 >= map->width)
        scroll.x = map->width - screen.width + map->tile.dimen[0][0].w;
    
    // AUTUALIZAR OS TILES VISIVEIS DA COLUNA
    if(scroll.x >= 0 )
        map->visible_tile_width   = ((screen.width / map->tile.dimen[0][0].w ) + (scroll.x / map->tile.dimen[0][0].w));
    
    // AUTUALIZAR OS TILES VISIVEIS DA LINHA
    if(scroll.y >= 0)
        map->visible_tile_height  = ((screen.height / map->tile.dimen[0][0].h ) + (scroll.y / map->tile.dimen[0][0].h));


    if(scroll.x < 0) scroll.x = 0;
    if(scroll.y < 0) scroll.y = 0;

    

    al_identity_transform(&camera);
    al_translate_transform(&camera, -scroll.x, -scroll.y);
    al_use_transform(&camera); 

}
