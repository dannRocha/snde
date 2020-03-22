#ifndef __TOOLS_TRANSPILER_H__
#define __TOOLS_TRANSPILER_H__

#include "./tools.c"


const char* normalize(const char *string);
char **parse_string(const char *string);
char *get_filename(const char *string);
void strcpy_i(PropsTranspiler *element, char **string_t);
bool blank_line(const char *line);

void setting_tile_map(MapTranspiler *map, const char *string);
void setting_tile_n(int id, MapTranspiler *element, FILE *file);
char *transpiler(const char*filename, MapTranspiler map, Map *reference);

#endif