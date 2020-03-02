#ifndef __GAME_TYPES_LIB_H__
#define __GAME_TYPES_LIB_H__



typedef ALLEGRO_DISPLAY     *Display;
typedef ALLEGRO_BITMAP      *Image;
typedef ALLEGRO_EVENT_QUEUE *Events;
typedef ALLEGRO_EVENT        Event;
typedef ALLEGRO_TIMEOUT      Time;
typedef ALLEGRO_TRANSFORM    Camera;



typedef struct Coord
{
    int x;
    int y;

} Coord;



typedef struct Dimension
{
    int w;
    int h;

} Dimension;



typedef struct
{
    Image sprite[10];

} Sprites;



typedef struct Attributes
{
    bool alive;

} Attributes;



typedef struct  Character
{
    Coord coord;
    Dimension dimen;
    Attributes attr;
    Sprites img;
    Image test;

} Actor;



typedef struct Tile{
    int x;
    int y;
    int width;
    int height;
    char *label;
    char *id;

} Tile;

typedef struct Map
{
    int **source;
    
    int rows;
    int cols;

    long int width;
    long int height;
    
    int visible_tile_width_max;
    int visible_tile_height_max;

    int visible_tile_width_min;
    int visible_tile_height_min;

    int number_for_tiles;
    
    Coord scroll;

    bool set_scroll;
    bool there_is_a_collision_bitmap;
    bool check_visible;
    
    double scale;
    
    Tile **tiles;
    Image *bitmap; 

} Map;



typedef struct Window
{    
    int width;
    int height;
    const char *title;
    Display display;

} Window;



#endif //__GAME_TYPES_LIB_H__