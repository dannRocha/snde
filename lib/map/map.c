#include <stdio.h>
#include "../snde.h"
#include "../types/types.h"



int AUTO_DETECT_ROW = 0;
int AUTO_DETECT_COL = 0;
Tiles tiles;



String* path_bitmap(String filename){
    
    FILE* file = fopen(filename, "r");
    
    int sprites = 0, row = 0, col = 0;
    int buffer_len = 256;
    char buffer[256];
    


    if(file == NULL)
        exit(-1);

    fscanf(file, "%d %d %d", &row, &col, &sprites);
    
    
    String* path;

    path =  malloc((sprites) * sizeof(String));

    if(!path)
        message_error("Error ao alocar memoria");

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




void configure_tiles(Tiles* tile, String filename){


    tile->src = malloc((tiles.quantity) * sizeof(Image ));
    tile->coord = malloc((tiles.quantity) * sizeof(Coord ));
    tile->dimen = malloc((tiles.quantity) * sizeof(Dimension ));


    if(tile->src == NULL)
        message_error("Error ao alocar memoria");

    String* paths = path_bitmap(filename);


    for(int i = 0; i < tile->quantity; i++)
        tile->src[i] = (Image) malloc((tile->quantity) * sizeof(Image));


    // CARREGA TODOS OS TILES DO MAPA;
    for(int i = 0; i < tile->quantity; i++)
        tile->src[i] = load_image(paths[i]);


    if(!paths)
        free(paths);
}




Map load_map(String filename){


    FILE *file = fopen(filename, "r");

    fscanf(file, "%d %d %d", &AUTO_DETECT_ROW, &AUTO_DETECT_COL, &tiles.quantity);
    Map map;


    if(file == NULL){
        message_error("Error ao tentar lê o arquivo");
    

        free(file);
        exit(-1);
    }
    

    map = (Map ) malloc ((AUTO_DETECT_ROW) * sizeof(int *));
    
    if(map == NULL)
        message_error("Error ao alocar memoria");

    
    for(int i = 0; i < AUTO_DETECT_ROW; i++)
        map[i] = (int *) malloc ((AUTO_DETECT_COL) * sizeof(int));
    


    // INICIALIZAR MATRIZ COM VALOR
    for(int i = 0; i < AUTO_DETECT_ROW; i++)
        for(int j = 0; j < AUTO_DETECT_COL; j++)
            map[i][j] = 0;


    configure_tiles(&tiles, filename);


    // PULA O FLUXO DE LEITURA ATÈ A MATRIZ;
    for(int i = 0; i <= tiles.quantity; i++){
        char _;
        while((_ = fgetc(file)) != EOF){
            if(_=='\n') break;
        }
    }
   

    for(int i = 0; i < AUTO_DETECT_ROW ; i++){
        for(int j = 0; j < AUTO_DETECT_COL; j++)
            fscanf(file, "%d", &map[i][j]);
    } 

   
    fclose(file);

    return map;

}



void free_map(Map map){
    for(int i = 0; i < AUTO_DETECT_ROW; i++)
        free(map[i]);        

    free(map);
}



void free_image_src(String* path, int row){
    for(int i = 0; i < row; i++)
        free(path[i]);
}



void draw_map(Map map, double scale){
    
    int x = 0, y = 0;
    int w = 32, h = 32;
   

    for(int i = 0; i < AUTO_DETECT_ROW; i++){
        for(int j = 0; j < AUTO_DETECT_COL; j++){
            for(int k = 0; k < tiles.quantity; k++){
                if(map[i][j] == k){
                    draw_image(tiles.src[k], x,y, scale,0);
                }
            }
            x += w * scale;
        }

        x = 0;
        y += h * scale;
    }
}


