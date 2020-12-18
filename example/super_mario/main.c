#define __DEBUGGER_BOX_COLLISION__
#define __DEBUGGER_COLLISION_MAP__
#define __DEBUGGER_GRID_COLLISION_CHECK_
#define __DEBUGGER_LOG_BAR__ 


#include "../../lib/snde.h"
// if(IsKeyPressed(KEY_C))
// https://www.raylib.com/examples.html

/*
 if (IsKeyPressed('R'))    // Reset physics input
        {
            // Reset movement physics body position, velocity and rotation
            body->position = (Vector2){ screenWidth/2, screenHeight/2 };
            body->velocity = (Vector2){ 0, 0 };
            SetPhysicsBodyRotation(body, 0);
        }

 if (IsKeyDown(KEY_RIGHT)) body->velocity.x = VELOCITY;
         else if (IsKeyDown(KEY_LEFT)) body->velocity.x = -VELOCITY;
 
         // Vertical movement input checking if player physics body is grounded
         if (IsKeyDown(KEY_UP) && body->isGrounded) body->velocity.y = -VELOCITY*4;

   DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
               DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);
   
               DrawText("Physac", logoX, logoY, 30, WHITE);
               DrawText("Powered by", logoX + 50, logoY - 7, 10, WHITE);
*/

#include <math.h>
#include <stdbool.h>

Window screen;
Events events;
Map scenarios;
   
Actor person;

int animacao  = 0;
int sense_x;
int sense_y;
int jumping = 0;
int aux_x = 0;

void game(double);


int main(int argc, char** argv){
   
    if(!init_all_lib())
        return -1;

    screen   = create_window(1240, 480, "LIBRARY");
    events   = add_event_listener(&screen);
    scenarios   = load_map("world/word_1_1.map", 2.0);
   
    person = create_character("src/bmp/animation/character2.png", 32, 32, 0,0, 2);
        person.coord.x = 0;
        person.coord.y = 352;
        person.speed   = 20;

        al_convert_mask_to_alpha(person.spritesheet, al_map_rgb(255,0, 255));
    
    update(60.0, true, game);

    destroy(&screen, "Window");
    destroy(events, "Events");
    destroy(&person, "Actor");
    destroy(&scenarios, "Map");
    
    return 0;
}


void game(double dt){
    
    Event event = get_event(events);

    keyboard(&event, &sense_x, &sense_y);

    if(event.type == WINDOW_CLOSE)
        exit(1);
        
    draw_color_background(66, 66, 255);
    dynamic_camera(&screen, &scenarios, &person);

    double fps = 8;

    double gravity = -1;
 


    if(sense_y)
    	jumping = 1;
		if(sense_x && aux_x == 0)
			aux_x = sense_x;
    
     if( person.coord.y > 352){
    	 person.coord.y = 352;
    	 person.speed = 20;
    	 jumping = 0;
    	 aux_x = 0;
     }

    if(jumping){
			person.coord.x += ( 700 * dt) * aux_x;
      person.coord.y += person.speed * dt * -39;
  	  person.speed += gravity;
    }
    else {
    	aux_x = 0;	
    }

    person.coord.x = (!jumping) ? person.coord.x + (400 * dt) * sense_x  : person.coord.x;
    //person.coord.x += dt * sense_x * pow(person.speed, 2);

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
