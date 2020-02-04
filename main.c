#include "lib/snde.h"


//gcc -Wall main.c -o teste -lallegro -lallegro_image -lallegro_primitives




int main(int argc, char** argv)
{
    if(!init_all_lib())
        return -1;

    Window screen   = create_window(840, 480, "LIBRARY");
    Events events   = add_event_listener(screen);
    Map scenarios   = load_map("map/map_example1.map");
    Actor obj;
    obj.test        = load_image("water_00.bmp");
    
    int speed = 10;
    int sense_x = 0;
    int sense_y = 0;
    
    obj.coord.x = 32;
    obj.coord.y = 60;
    obj.size.w = 32;
    obj.size.h = 32;

    
    
    while (true)
    {


        Event event = get_event(events);

        keyboard(&event, &sense_x, &sense_y);
       
        if(event.type == WINDOW_CLOSE)
            break;
        
        draw_color_background(255, 255, 255);
        draw_map(&scenarios, 3);
        draw_image(obj.test, obj.coord.x, obj.coord.y, 1, 0);
        update_screen(60.0);

        
        obj.coord.x += speed * sense_x;
        obj.coord.y += speed * sense_y;


        if(collision_map(&scenarios, &obj, 1, 2)){
            obj.coord.x = 32;
            obj.coord.y = 110;
            
        }



        move_camera(screen, &scenarios, &obj);
    }
    
    destroy(screen, "Window");
    destroy(events, "Events");
    destroy(obj.test, "Image");
    free_map(&scenarios);
    
  return 0;

}

