#define __DEBUGGER_BOX_COLLISION__
#define __DEBUGGER_COLLISION_MAP__
#define __DEBUGGER_GRID_COLLISION_CHECK_
#include "../../lib/snde.h"
#include <math.h>

Window screen;
Events events;
Map scenarios;
   
Actor person;

int animacao  = 0;
int sense_x;
int sense_y;

void loop(void(*)());
void jumping(double);
void game(double);




int main(int argc, char** argv){
    if(!init_all_lib())
        return -1;

    screen   = create_window(1240, 480, "LIBRARY");
    events   = add_event_listener(&screen);
    scenarios   = load_map("world/word_1_1.map", 2.0);
   
    person = create_character("src/bmp/animation/character2.png", 32, 32, 0,0, 2);
        person.coord.x = 0;
        //person.coord.y = 352;
        person.coord.y = 0;
        person.speed   = 30;
        al_convert_mask_to_alpha(person.spritesheet, al_map_rgb(255,0, 255));
    
    loop(game);

    destroy(&screen, "Window");
    destroy(events, "Events");
    destroy(&person, "Actor");
    destroy(&scenarios, "Map");
    
    return 0;
}

void loop(void(*game)()){
    double t = 0.0;
    double dt = 0.01;
    //double dt = 1/60;

    double currentTime = clock();
    double accumulator = 0.0;

    while ( true )
    {
        double newTime = clock();
        double frameTime = newTime - currentTime;
        if ( frameTime > 0.25 )
            frameTime = 0.25;
        currentTime = newTime;

        accumulator += frameTime;

        while ( accumulator >= dt )
        {
            t += dt;
            //calcLogic()
            accumulator -= dt;
        }
            //system("clear");
            //printf("DeltaTime: %f\n", dt);
            // render();
            game(dt);
            al_flip_display();

    }
}

void jumping(double dt)
{
    system("clear");
    printf("Y: %f\n", person.coord.y);
    //person.coord.y += pow(person.speed* 1.2, 2) * dt * sense_y ;
    if(person.coord.y + person.dimen.h < 352.0 + person.dimen.w){
        person.coord.y += (pow(person.speed, 2) + 10)* dt;
        sense_y = 0;
    }

    person.coord.y += (pow(person.speed, 2) + 90)* dt * ( sense_y * 11);
}
void game(double dt){
    
        Event event = get_event(events);

        keyboard(&event, &sense_x, &sense_y);
       
        if(event.type == WINDOW_CLOSE)
            exit(1);
        draw_color_background(66, 66, 255);
        dynamic_camera(&screen, &scenarios, &person);
    
        
        double fps = 8;

        person.coord.x += dt * sense_x * pow(person.speed, 2);
        jumping(dt);


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

            run_animation(&person, fps, animacao, auto_play);
            
        }
        else
        {
            run_animation( &person, fps, animacao,
                animation_control(1, 0));
        }
        
        person.animation.status.running = false;
        
        collision_map(NULL, &scenarios, &person);
}