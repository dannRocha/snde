#include "lib/snde.h"


//gcc -Wall main.c -o teste -lallegro -lallegro_image -lallegro_primitives


int main(int argc, char** argv)
{
    if(!init_all_lib())
        return -1;

    Window screen   = create_window(840, 480, "LIBRARY");
    Events events   = add_event_listener(screen);
    Map scenarios   = load_map("map/map_example1.map");


    int x = 0;
    int y = 0;
    int speed = 10;
    int sense_x = 0;
    int sense_y = 0;
    int fake_time = 0;
    int toggle = 0;

    while (true)
    {   
        Event event = get_event(events);

        keyboard(&event, &sense_x, &sense_y);
       
        if(event.type == WINDOW_CLOSE)
            break;
        
        draw_color_background(255, 255, 255);
        draw_map(scenarios, 1.5);
        update_screen(60.0);

        x += speed * sense_x;
        y += speed * sense_y;
        fake_time++;

        if(fake_time >= 100){
            if(toggle == 1) toggle = 0;
            else toggle = 1;

            fake_time = 0;
        }
    }
    
    destroy(screen, "Window");
    destroy(events, "Events");
    free_map(scenarios);
    
  return 0;

}

