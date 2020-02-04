#ifndef __GAME_TYPES_LIB_H__
#define __GAME_TYPES_LIB_H__



// typedef char* String;



typedef ALLEGRO_DISPLAY*     Window;
typedef ALLEGRO_BITMAP*      Image;
typedef ALLEGRO_EVENT_QUEUE* Events;
typedef ALLEGRO_EVENT        Event;
typedef ALLEGRO_TIMEOUT      Time;
typedef ALLEGRO_TRANSFORM    Camera;


typedef struct {
    int x;
    int y;

} Coord;

typedef struct{
    int w;
    int h;

} Dimension;

typedef struct{
    Image sprite[10];

} Sprites;

typedef struct{
    bool alive;

} Attributes;



typedef struct  Character{
    
    Coord coord;
    Dimension dimen;
    Attributes attr;
    Sprites img;
    Image test;

} Actor;



typedef struct {
    Coord **coord;
    Dimension **dimen;
    Image* src;
    int quantity;

} Tiles;



typedef struct Map
{
    int** source;
    int rows;
    int cols;
    Tiles tile;

} Map;



#endif //__GAME_TYPES_LIB_H__