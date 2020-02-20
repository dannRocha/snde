#include "lib/snde.h"
#include <regex.h>

//gcc -Wall main.c -o teste -lallegro -lallegro_image -lallegro_primitives
// Regular expression
//((?<=\")[^:"\s]+(?=\")|(?<=\')[^:'\s]+(?=\'))   -> id: "0" label: "ground" src: "src/asset/png/tileset/00.png"
//(?<=\[)[^]]+(?=\]) ->[map] || [collision] etc
//((\".*?)[^::\"\s](.*?\")|(\'.*?)[^\'](.*?\'))


int main(int argc, char** argv)
{
    if(!init_all_lib())
        return -1;

    Window screen   = create_window(840, 480, "LIBRARY");
    Events events   = add_event_listener(&screen);
    Map scenarios   = load_map("map/map_example2.map", 2);
    Actor obj;
    obj.test        = load_image("src/bmp/map/water.bmp");
    
    int speed = 20;
    int sense_x = 0;
    int sense_y = 0;
    
    obj.coord.x = 32;
    obj.coord.y = 60;
    obj.dimen.w = 32;
    obj.dimen.h = 32;

    
    
    while (true)
    {

        
        Event event = get_event(events);

        keyboard(&event, &sense_x, &sense_y);
       
        if(event.type == WINDOW_CLOSE)
            break;
        
        draw_color_background(66, 66, 255);
        draw_map(&scenarios, &screen);
        draw_image(obj.test, obj.coord.x, obj.coord.y, 1, 0);
        update_screen(60.0);

        
        obj.coord.x += speed * sense_x;
        obj.coord.y += speed * sense_y;

        if(collision_map(&scenarios, &obj, 1, 2) || scenarios.tile.coord[scenarios.rows - 1][0].y < obj.coord.y){
            // obj.coord.x = 32;
            // obj.coord.y = 110;

        }



        move_camera(screen, &scenarios, &obj);
    }
    
    destroy(&screen, "Window");
    destroy(events, "Events");
    destroy(obj.test, "Image");
    free_map(&scenarios);
    
  return 0;

}

