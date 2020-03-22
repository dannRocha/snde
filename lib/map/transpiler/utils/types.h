#ifndef __TYPES_TRANSPILER_AUX_H__
#define __TYPES_TRANSPILER_AUX_H__

enum TILE_PROPS
{
    STATELESS = 0,
    COLLISION = 1,
    ESPECIAL  = 2,
    MAP       = 3
};

typedef struct PropsTranspiler
{
    char *id;
    char *label;
    char *src;

} PropsTranspiler;


typedef struct TileTranspiler
{
    PropsTranspiler **props;
    int *size;

} TileTranspiler;


typedef struct MapTranspiler
{
    TileTranspiler grid;
    char **matriz;
    int rows;
    int cols;
    bool check;

} MapTranspiler;


#endif