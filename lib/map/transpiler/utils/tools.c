#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

const char* normalize(const char *string){
    
    char *new_string = (char*) calloc(strlen(string), sizeof(char));
    int count = 0;
    
    for(int i = 0; string[i] != '\0'; i++)
    {    
        if(string[i] == '\t' || string[i] == ' ' || string[i] == '\n')
            continue;
        
        new_string[count] = string[i];
        count++;
    }
    
    new_string[count] = '\0';
    
    return new_string;
}


char **parse_string(const char *string)
{
    const int QUANTITY_OF_STRING  = 2;

    int itens[3][2] = {
        {-1, -1},
        {-1, -1},
        {-1, -1}
    };
    
    int start = 0;
    
    for(int j = 0; j < 3; j++){
        
        
        for(int i = start; i < strlen(string); i++)
        {
            if(string[i] == '\"' && itens[j][0] == -1)
            {
                start += i;
                itens[j][0] = i + 1;
            }
            else if(string[i] == '\"' && itens[j][1] == -1)
            {
                start = i + 1;
                itens[j][1] = i;
            }
        }
        
    }
    
    
    char *id    = (char* ) calloc(((itens[0][1] - itens[0][0]) +  1), sizeof(char));
    char *label = (char* ) calloc(((itens[1][1] - itens[1][0]) +  1), sizeof(char));
    char *src   = (char* ) calloc(((itens[2][1] - itens[2][0]) +  1), sizeof(char));

    // CONCATENAR   
    for(int j = 0; j < 3; j++)
    {   
        int k = 0;

        for(int i = itens[j][0]; i < itens[j][1]; i++)
        {
            if(j == 0)
            {
                id[k] = string[i]; k++;
            }
            else if(j == 1)
            {
                label[k] = string[i]; k++;
            }
            else
            {
                src[k] = string[i]; k++;
            }
        }
    }
    
    char **format_p = (char** ) calloc(QUANTITY_OF_STRING, sizeof(char* ));
    
    format_p[0] = id;
    format_p[1] = label;
    format_p[2] = src;
    
    return format_p;
}

char *get_filename(const char *string)
{   
    int size = 0;
    
    for(int i = strlen(string) - 1; i >= 0; i--)
    {
        if(string[i] == '/')
            break;
            
       size++;
    }
    
    char *filename = (char* ) calloc((size + 1), sizeof(char));
    
    for(int i = (strlen(string) - size), j = 0; i < strlen(string); i++, j++)
    {
        if(string[i] == '.')
            break;
        
        filename[j] = string[i];
    }
    
    
    return (char*)filename;
   
}


bool blank_line(const char *line)
{
    return !strlen(normalize(line));
}


void strcpy_i(PropsTranspiler *element, char **string_t)
{
    element->id     = (char* ) calloc(strlen(string_t[0]), sizeof(char ));
    element->label  = (char* ) calloc(strlen(string_t[1]), sizeof(char ));
    element->src    = (char* ) calloc(strlen(string_t[2]), sizeof(char ));
    
    element->id     = string_t[0];
    element->label  = string_t[1];
    element->src    = string_t[2];
    
}


void setting_tile_map(MapTranspiler *map, const char *string)
{
    static int line_number = 1;

    const char *string_n = normalize(string);

    if(!map->check)
    {
        map->rows = 0;
        map->cols = strlen(string_n);
        map->check = true;
        line_number = 1;
    }

    if(map->cols != strlen(string_n))
    {   
        fprintf(stderr, "\n\nError na declaracao do MAPA:");
        fprintf(stderr, "\n\tMapa com colunas diferentes | linha do mapa: %d |\n\n\n", line_number);
        exit(-1);
    }

    map->rows += 1;

    map->matriz = (char** ) realloc(map->matriz, ((map->rows) * sizeof(char* )));
    map->matriz[map->rows - 1] = calloc(map->cols, sizeof(char ));//calloc(map->cols, sizeof(char ));

    for(int i = 0; i < map->cols; i++)
        map->matriz[map->rows - 1][i] = string_n[i];


    line_number++;

}


void setting_tile_n(int id, MapTranspiler *element, FILE *file)
{
    size_t length;
    char *line = NULL;

    while(true)
    {   
        getline(&line, &length, file);
        const char *line_n = normalize(line);

        if(!strcmp(line_n, ("(stateless)")) ||!strcmp(line_n, ("(collision)"))
        || !strcmp(line_n, ("(especial)")) || !strcmp(line_n, ("(map)"))){
            continue;
        }

        if(blank_line(line) || !strcmp(line_n, "{"))
            continue;
        if(!strcmp(line_n, "}"))
            break;
        
        if(id == MAP)
        {
            setting_tile_map(element, line_n);
            continue;
        }

        char **format_line = parse_string(line_n);

        element->grid.props[id] = (PropsTranspiler *) realloc(element->grid.props[id], (element->grid.size[id] + 1) * sizeof(PropsTranspiler));
        strcpy_i(&element->grid.props[id][element->grid.size[id]], format_line);
        element->grid.size[id] += 1;


        if(format_line) free(format_line);
    }

    if(line) free(line); 

}


char *transpiler(const char*filename, MapTranspiler map, Map *referece_map)
{
    const char *path   = get_path(filename);
    const char *prefix = get_filename(filename);
    const char *sufix  = "_transpiler";

    char *filename_t = calloc((strlen(path) + strlen(prefix) + strlen(sufix)), sizeof(char ));
    
    strcpy(filename_t, path);
    strcat(filename_t, prefix);
    strcat(filename_t, sufix);


    FILE *file_transpiler = fopen(filename_t, "w");
    

    int  qtd_tile = (map.grid.size[STATELESS] + map.grid.size[COLLISION] + map.grid.size[ESPECIAL]);
    fprintf(file_transpiler, "%d %d %d\n", map.rows, map.cols, qtd_tile);

    

    // ADICIONAR OS CAMINHOS DOS TILES
    for(int index = 0; index < map.grid.size[COLLISION]; index++)
    {
        fprintf(file_transpiler, "%s\n", normalize(map.grid.props[COLLISION][index].src));
    }
    
    for(int index = 0; index < map.grid.size[ESPECIAL]; index++)
    {
        fprintf(file_transpiler, "%s\n", map.grid.props[ESPECIAL][index].src);
    }

    for(int index = 0; index < map.grid.size[STATELESS]; index++)
    {
        fprintf(file_transpiler, "%s\n", map.grid.props[STATELESS][index].src);

    }
  

    char *id = (char*) calloc(2, sizeof(char));
        id[1] = '\0';
    for(int i = 0; i < map.rows; i++){
        for(int j = 0; j < map.cols; j++){
            
            id[0] = map.matriz[i][j];
            
            if(map.matriz[i][j] == '.')
            {
                fprintf(file_transpiler, "-1");
                if(j < map.cols - 1)
                    fprintf(file_transpiler, " ");
                
                referece_map->tiles[i][j].id = (char*)calloc(2, sizeof(char));
                referece_map->tiles[i][j].label = (char*)calloc(5, sizeof(char));
                referece_map->tiles[i][j].id = ".\0";
                referece_map->tiles[i][j].label = "void\0";
                
                continue;
            }

            for(int k = 0; k < map.grid.size[COLLISION]; k++){
                if(!strcmp(id, map.grid.props[COLLISION][k].id))
                {
                    if(k < 10)
                        fprintf(file_transpiler, "0%d", k);
                    else
                        fprintf(file_transpiler, "%d", k);
                    
                    referece_map->tiles[i][j].id    = map.grid.props[COLLISION][k].id;
                    referece_map->tiles[i][j].label = map.grid.props[COLLISION][k].label;

                    break;
                }    
            }
            for(int k = 0; k < map.grid.size[ESPECIAL]; k++){
                if(!strcmp(id, map.grid.props[ESPECIAL][k].id)){
                    if((k + map.grid.size[COLLISION]) < 10)
                        fprintf(file_transpiler, "0%d", (k + map.grid.size[COLLISION]));
                    else
                        fprintf(file_transpiler, "%d", (k + map.grid.size[COLLISION]));
                    
                    
                    referece_map->tiles[i][j].id    = map.grid.props[ESPECIAL][k].id;
                    referece_map->tiles[i][j].label = map.grid.props[ESPECIAL][k].label;
                    
                    break;
                }
            }

            for(int k = 0; k < map.grid.size[STATELESS]; k++){
                if(!strcmp(id, map.grid.props[STATELESS][k].id))
                {
                    if((map.grid.size[COLLISION] + map.grid.size[ESPECIAL] + k) < 10)
                        fprintf(file_transpiler, "0%d", k + (map.grid.size[COLLISION] + map.grid.size[ESPECIAL]));
                    else
                        fprintf(file_transpiler, "%d", k + (map.grid.size[COLLISION] + map.grid.size[ESPECIAL]));

                    referece_map->tiles[i][j].id    = map.grid.props[STATELESS][k].id;
                    referece_map->tiles[i][j].label = map.grid.props[STATELESS][k].label;

                    break;
                }
            }

            if(j < map.cols - 1)
                        fprintf(file_transpiler, " ");      
        }
        if(i < map.rows - 1)
            fprintf(file_transpiler, "\n");

    }

    
    fclose(file_transpiler);
    if(file_transpiler) free(file_transpiler);
    if(id) free(id);

    return (char*) filename_t;
}
