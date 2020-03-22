// #define __DEBUGGER_BOX_COLLISION__
#define __DEBUGGER_COLLISION_MAP__
#define __DEBUGGER_GRID_COLLISION_CHECK__
#include "lib/snde.h"
#include <pthread.h>



/*
    Error ao adicionar exatamente 136 colunas no mapa acessando memoria nao alocada

    ideia:
        carregar mais de um mapa por arquivo
*/

Color rgba(int red, int green, int blue, int alpha){
    return al_map_rgba(red, green, blue, alpha);    
}

void draw_rect(float x1, float y1, float x2, float y2, Color color)
{
    al_draw_rectangle(x1, y1, x2, y2, color, 0);
}

typedef struct Attr 
{
    bool live;
} Attr;


int main(int argc, char** argv)
{

    if(!init_all_lib())
        return -1;

    Window screen   = create_window(1240, 480, "LIBRARY");
    Events events   = add_event_listener(&screen);
    Map scenarios   = load_map("map/map_example3.txt", 0.5);
   
    Actor person = create_character("src/bmp/animation/character2.png", 32, 32, 0,0, 6.5);
        person.coord.x = 0;
        person.coord.y = 0;
        person.speed   = 30;
        al_convert_mask_to_alpha(person.spritesheet, al_map_rgb(255,0, 255));

    Attr attr;
    person.attr = &attr;
    
    // person.attr = (Attr*) person.attr;
    
    ((Attr*) person.attr)->live = true;
    // Attr *attr2 = (Attr* ) person.attr;
    // printf("%p == %p\n", person.attr, &attr);
    
    int animacao  = 0;
    int sense_x;
    int sense_y;

    while (true)
    {
        Event event = get_event(events);

        keyboard(&event, &sense_x, &sense_y);
       
        if(event.type == WINDOW_CLOSE)
            break;
        draw_color_background(66, 66, 255);
        // draw_color_background(255, 0, 255);
        dynamic_camera(&screen, &scenarios, &person);
        // static_camera(&screen, &scenarios);
        
        // person.coord.x += 10 * sense;
        
        double fps = 5;

        person.coord.x += person.speed * sense_x;
        person.coord.y += person.speed * sense_y;
        
        if(sense_x > 0){
            animacao = 2;
            person.animation.status.running = true;
        }
        else if(sense_x < 0){
            person.animation.status.running = true;
            animacao = 1;
        }

        if(!person.animation.status.running)
        {   
            animacao = 0;
            int sprite_stop = 1;

            run_animation(&person, fps, animacao, auto_play);
            // run_animation( &person, fps, animacao,
            //     animation_control(3, 0, 1, 3));
        }
        else
        {
            run_animation( &person, fps, animacao,
                animation_control(0, 0));
            // run_animation(&person, fps, animacao, auto_play);
        }
        
        person.animation.status.running = false;
        
       
        collision_map(NULL, &scenarios, &person);
    
    

        
        
        // draw_rect(person.coord.x, person.coord.y, person.dimen.w + person.coord.x, person.dimen.h + person.coord.y, rgba(255,255,255,1));
        update_screen(60.0);
        
    }

    destroy(&screen, "Window");
    destroy(events, "Events");
    destroy(&person, "Actor");
    // destroy(&scenarios, "Map"); 
   
  return 0;

}
