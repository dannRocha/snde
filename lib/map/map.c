#include <stdio.h>
#include "../snde.h"
#include "../types/types.h"



char** path_bitmap(char* filename){
    
    FILE* file = fopen(filename, "r");
    
    int sprites = 0, row = 0, col = 0;
    int buffer_len = 256;
    char buffer[256];
    


    if(file == NULL)
        exit(-1);

    fscanf(file, "%d %d %d", &row, &col, &sprites);
    
    
    char** path;

    path =  malloc((sprites) * sizeof(char*));

    if(!path)
        message_error("Error ao alocar memoria para PATH ");

    for(int i = 0; i < sprites; i++)
        path[i] = malloc((255) * sizeof(char));
    
    int count = 0;    
    while(fgets(buffer, buffer_len, file)){
        if(strstr(buffer, ".bmp") != NULL || strstr(buffer, ".png") != NULL){
            copy(path[count], buffer);
            count++;
        }
    }


    fclose(file);

    if(!file)
        free(file);


    return path;
}




void configure_tiles(Map *map, char* filename){


    map->tile.src = malloc((map->tile.quantity) * sizeof(Image ));
    map->tile.coord = (Coord** ) malloc((map->rows) * sizeof(Coord *));
    map->tile.dimen = (Dimension** ) malloc((map->rows) * sizeof(Dimension* ));
    


    if(map->tile.src == NULL)
        message_error("Error ao alocar memoria");

    char** paths = path_bitmap(filename);


    for(int i = 0; i < map->tile.quantity; i++)
        map->tile.src[i] = (Image) malloc((map->tile.quantity) * sizeof(Image));


    // CARREGA TODOS OS TILES DO MAPA;
    for(int i = 0; i < map->tile.quantity; i++)
        map->tile.src[i] = load_image(paths[i]);

    // ALOCAR MEMORIA PARA A MATRIZ DE COORDENAS E DIMENSÕES DOS TILES
    for(int i = 0; i < map->rows; i++){
        map->tile.coord[i] = (Coord *) malloc ((map->cols) * sizeof(Coord));
        map->tile.dimen[i] = (Dimension *) malloc ((map->cols) * sizeof(Dimension));
    }

    // CONFIGURAR COORDENADAS DOS TILES
    for(int i = 0; i < map->rows; i++)
        for(int j = 0; j < map->cols; j++){
            for(int k = 0; k < map->tile.quantity; k++){
                if(map->source[i][j] == k){
                    map->tile.dimen[i][j].w = al_get_bitmap_width(map->tile.src[k]);
                    map->tile.dimen[i][j].h = al_get_bitmap_height(map->tile.src[k]);
                    map->tile.coord[i][j].x = j * map->tile.dimen[i][j].w;
                    map->tile.coord[i][j].y = i * map->tile.dimen[i][j].h;
                }

            }    
        }
    

    if(!paths)
        free(paths);
}



Map load_map(char* filename){

    Map maps;

    FILE *file = fopen(filename, "r");

    fscanf(file, "%d %d %d", &maps.rows, &maps.cols, &maps.tile.quantity);


    if(file == NULL){
        message_error("Error ao tentar lê o arquivo");
        free(file);
        exit(-1);
    }


    
    maps.source = (int** ) malloc((maps.rows) * sizeof(int* ));



    if(maps.source == NULL)
        message_error("Error ao alocar memoria para MAP");


    for(int i = 0; i < maps.rows; i++)
        maps.source[i] = (int* ) malloc ((maps.cols) * sizeof(int));


    for(int i = 0; i < maps.rows; i++)
        for(int j = 0; j < maps.cols; j++)
            maps.source[i][j] = 0;


    // PULA O FLUXO DE LEITURA ATÈ A MATRIZ;
    for(int i = 0; i <= maps.tile.quantity; i++){
        char _;
        while((_ = fgetc(file)) != EOF){
            if(_=='\n') break;
        }
    }
    
    // LER MATRIZ DE DADOS DO MAPA
    for(int i = 0; i < maps.rows; i++){
        for(int j = 0; j < maps.cols; j++)
            fscanf(file, "%d", &maps.source[i][j]);
    }
   
    fclose(file);


    configure_tiles(&maps, filename);

    return maps;

}



void free_map(Map *map){
    for(int i = 0; i < map->rows; i++)
        free(map->source[i]);        

    free(map->source);
}



void free_image_src(char** path, int row){
    for(int i = 0; i < row; i++)
        free(path[i]);
}



void draw_map(Map* map, double scale){
   

    for(int i = 0; i < map->rows; i++){
        for(int j = 0; j < map->cols; j++){        
            for(int k = 0; k < map->tile.quantity; k++){
                if(map->source[i][j] == k){

                    // MODIFICAR PROPRIEDADES COM BASE NA ESCALA DO DESENHO
                    map->tile.dimen[i][j].w = al_get_bitmap_width(map->tile.src[k])  * scale;
                    map->tile.dimen[i][j].h = al_get_bitmap_height(map->tile.src[k]) * scale;
                    map->tile.coord[i][j].x = j * map->tile.dimen[i][j].w;
                    map->tile.coord[i][j].y = i * map->tile.dimen[i][j].h;

                    draw_image(map->tile.src[k], map->tile.coord[i][j].x, map->tile.coord[i][j].y, scale, 0);
                }
            }
        }
    }
}



bool collision_map(Map* map, Actor *character,int start_tile, int end_tile){
    for(int i = 0; i < map->rows; i++){
        for(int j = 0; j < map->cols; j++){
            for(int tile = start_tile; tile < end_tile; tile++){
                if(map->source[i][j] == tile){
                    if( character->coord.x + character->size.w >= map->tile.coord[i][j].x &&
                        character->coord.x <= map->tile.coord[i][j].x + map->tile.dimen[i][j].w &&
                        character->coord.y <= map->tile.coord[i][j].y + map->tile.dimen[i][j].h &&
                        character->coord.y + character->size.h >= map->tile.coord[i][j].y
                    ){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


