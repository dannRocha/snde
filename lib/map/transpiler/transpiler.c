#include "./utils/tools.h"
#include "../../types/types.h"


const char *transpiler_map(Map *reference_map, const char *filename)
{
    FILE *file = NULL;
    char *line = NULL;
    size_t length;
    MapTranspiler map;


    const int INIT_MEMORY = 1;
    const int TILES_SECTION = 3;


    map.matriz          = (char** ) calloc(INIT_MEMORY, sizeof(char*));
    map.grid.props      = (PropsTranspiler**) calloc(TILES_SECTION, sizeof(PropsTranspiler* ));
    map.grid.size       = (int* )   calloc(TILES_SECTION, sizeof(int ));
        map.grid.size[STATELESS]    = 0;
        map.grid.size[COLLISION]    = 0;
        map.grid.size[ESPECIAL]     = 0;
    


    if(map.grid.props == NULL)
        fprintf(stderr, "Erro ao alocar memoria para TILES");



    file = fopen(filename, "r");


    if(file == NULL)
    {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo \"%s\"\n", filename);
        exit(-1);
    }
  
    
    while(getline(&line, &length, file) != -1)
    {  
        const char *line_n = normalize(line);

        if(!strcmp(line_n, "(stateless)"))
            setting_tile_n(STATELESS, &map, file);
        if(!strcmp(line_n, "(collision)"))
            setting_tile_n(COLLISION, &map, file);
        if(!strcmp(line_n, "(especial)"))
            setting_tile_n(ESPECIAL, &map, file);
        if(!strcmp(line_n, "(map)"))
            setting_tile_n(MAP, &map, file);
    }
    

    fclose(file);

    if(file) free(file);
    if(line) free(line);

    reference_map->tiles      = (Tile** ) calloc(map.rows, sizeof(Tile* ));
    for(int i = 0; i < map.rows; i++){
        
        reference_map->tiles[i] = (Tile* ) calloc(map.cols, sizeof(Tile ));
        
        if(reference_map->tiles[i] == NULL)
        {
            fprintf(stderr, "Error ao alocar memoria para a MATRIZ de TILES\n");
            exit(-1);
        }
    }    

    

    reference_map->number_of_tiles_with_collision += map.grid.size[COLLISION] + map.grid.size[ESPECIAL];

    if(reference_map->number_of_tiles_with_collision)
        reference_map->there_is_a_collision_bitmap = true;

    const char *filename_transpiler = transpiler(filename, map, reference_map);
    
    
    //***MATRIZ
    for(int i = 0; i < map.rows; i++)
    {
        free(map.matriz[i]);
    }

    free(map.matriz);

    //***MATRIZ
    for(int i = 0; i  < 3; i++)
    {
        for(int j = 0; j < map.grid.size[i]; j++)
        {
            if(map.grid.props[i][j].id)  free(map.grid.props[i][j].id);
            if(map.grid.props[i][j].label)  free(map.grid.props[i][j].label);
            if(map.grid.props[i][j].src)  free(map.grid.props[i][j].src);
        }

        free(map.grid.props[i]);
    }
    if(map.grid.props)
        free(map.grid.props);
    if(map.grid.size)
        free(map.grid.size);

    return filename_transpiler;
    
} 
