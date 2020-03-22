#ifndef __GAME_TYPES_LIB_H__
#define __GAME_TYPES_LIB_H__



typedef ALLEGRO_DISPLAY     *Display;
typedef ALLEGRO_BITMAP      *Image;
typedef ALLEGRO_EVENT_QUEUE *Events;
typedef ALLEGRO_EVENT        Event;
typedef ALLEGRO_TIMEOUT      TimerOut;
typedef ALLEGRO_TIMER        Timer;
typedef ALLEGRO_TRANSFORM    Camera;
typedef ALLEGRO_COLOR        Color;



typedef struct Coord
{
    
    double x;
    double y;

} Coord;



typedef struct Dimension
{
    int w;
    int h;

} Dimension;



typedef struct Sprites
{
    double x;
    double y;
    double w;
    double h;

} Sprites;



typedef struct Attributes
{
    bool alive;

} Attributes;


typedef struct AnimationControl
{
    int *sequence_of_sprites;
    int len;
    int next;
    bool enable;
    
} AnimationControl;

typedef struct  StatusAnimation
{
    u_int8_t current_sprite;
    u_int8_t current_animation;
    bool configured;
    bool running;

} StatusAnimation;



typedef struct Animation
{
    StatusAnimation status;
    int number_of_animations;
    int number_of_sprites_per_animation;
    Timer *time;
    double framesperseconds;
    AnimationControl control;

} Animation;



typedef struct  Character
{
    Coord coord;
    Dimension dimen;
    void *attr;
    // Attributes attr;
    Sprites **mapping;
    Image spritesheet;
    Animation animation;
    Events event_queue;
    int8_t speed;
    double scale;


} Actor;



typedef struct Tile{
    u_int64_t x;
    u_int64_t y;
    u_int32_t width;
    u_int32_t height;
    char *label;
    char *id;

} Tile;

typedef struct Map
{
    int **source;
    
    int rows;
    int cols;

    int width;
    int height;
    
    int visible_tile_width_max;
    int visible_tile_height_max;

    int visible_tile_width_min;
    int visible_tile_height_min;

    int number_for_tiles;
    int number_of_tiles_with_collision;

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