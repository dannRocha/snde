#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include "../utils/tools.h"
#include "../types/types.h"



enum {
    WINDOW_CLOSE    = ALLEGRO_EVENT_DISPLAY_CLOSE,
    KEY_UP          = ALLEGRO_KEY_UP, 
    KEY_DOWN        = ALLEGRO_KEY_DOWN, 
    KEY_LEFT        = ALLEGRO_KEY_LEFT, 
    KEY_RIGHT       = ALLEGRO_KEY_RIGHT,
    KEY_ESCAPE      = ALLEGRO_KEY_ESCAPE,
    KEY_W           = ALLEGRO_KEY_W,
    KEY_S           = ALLEGRO_KEY_S,
    KEY_A           = ALLEGRO_KEY_A,
    KEY_D           = ALLEGRO_KEY_D,
};



bool init_all_lib(void){

    /*
        Retorna falso caso alguma inicializacao tenha falhado;
        Retorna true if todas as inicializacoes foram bem sucedidas;
    */

    if(!al_init()){
        message_error("Erro ao inicializar ALLEGRO");
        return false;
    }

    if (!al_install_keyboard()){
        message_error("Error ao inicializar o TECLADO");
        return false;
    }

    if(!al_init_image_addon()){
        message_error("Error ao inicializar ADDON DE IMAGEM");
        return false;
    }

    if(!al_init_primitives_addon()){
        message_error("Error ao inicializar ADDON DE PRIMITIVOS");
        return false;
    }

    // if(!al_init_font_addon()){
    //     fprintf(stderr, "Error ao inicializar ADDON de FONT");
    //     return false;
    // }


    // if(!al_init_ttf_addon()){
    //     fprintf(stderr, "Error ao inicializar ADDON de TTF");
    //     return false;
    // }

    return true;
}


Window create_window(u_int16_t width, u_int16_t height, const char* title)
{   
       
    if(width <= 0 || height <= 0)
    {
        fprintf(stderr, "\nError: Largura ou altura da janela nao pode ser menor ou igual a 0 (zero)\n");
        exit(-1);
    }

    if(title == NULL)
    {
        fprintf(stderr, "\nError: Titulo da janela nao pode ser NULL\n");
        exit(-1);
    }

    Window screen;
    
    screen.display  = al_create_display(width, height);
    screen.width    = width;
    screen.height   = height;
    screen.title    = title;

    if(!screen.display){
        message_error("Erro ao criar janela");
        al_destroy_display(screen.display);
        exit(-1);
    }
    
    al_set_window_title(screen.display, title);



    return screen;

}



Image load_image(const char* src){

    Image img = NULL;
 
    img = al_load_bitmap(src);
    
    if(!img){
        fprintf(stderr, "Error ao carregar imagem %s\n", src);
        al_destroy_bitmap(img);
        exit(-1);
    }

    return img;
}



void draw_color_background(char red, char green, char blue){
    al_clear_to_color(al_map_rgb(red, green, blue));
}



void draw_image(Image img, float x,float y, double scale, int flags ){
    // al_draw_bitmap(img, x, y, flags);
    int w = al_get_bitmap_width(img);
    int h = al_get_bitmap_height(img);

    al_draw_scaled_bitmap(img, 0, 0, w, h, x, y, w * scale, h * scale, 0);
    // al_draw_scaled_bitmap(
    //     img,
    //      0, // POSIÇAO DO SPRITE NO ARQUIVO DE SPRITES;
    //      0, // POSIÇAO DO SPRITE NO ARQUIVO DE SPRITES;
    //      32,// LARGURA DA SPRITE NO ARQUIVO DE SPRITES;
    //      32,// ALTURA  DA SPRITE NO ARQUIVO DE SPRITES;
    //      40,// POSIÇÂO X DO SPRITE EM TELA
    //      40,// POSIÇÂO Y DO SPRITE EM TELA
    //      50,// LARGURA DA SPRITE A SER DESENHADO - PODENDO AUMENTAR OU DIMINUIR;
    //      50,// ALTURA  DA SPRITE A SER DESENHADO - PODENDO AUMENTAR OU DIMINUIR;
    //     0); // FLAG;
}



void update_screen(double frameperseconds){
      
    al_rest(1.0 / frameperseconds);
    al_flip_display();
}



Events add_event_listener(Window *screen){
    
    // static Window screen_ = screen;
    
    Events event_queue = NULL;
    
    event_queue = al_create_event_queue();
    
    
    if(!event_queue){
        fprintf(stderr, "FILE: %s ROW: %d\n\tError ao carregar a fila de eventos", __FILE__, __LINE__);

        al_destroy_event_queue(event_queue);
        exit(-1);
    }


    al_register_event_source(event_queue, al_get_display_event_source(screen->display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    

    return event_queue;

}



Event get_event(Events events){
    
    const double timeout_clock = 0.005;

    Event event;
    TimerOut timeout;
    al_init_timeout(&timeout, timeout_clock);

    al_wait_for_event_until(events, &event, &timeout);
    
    return event;
}



Event getEventKeyboard(Events events){
    
    const double timeout_clock = 0.005;

    ALLEGRO_EVENT event;
    ALLEGRO_TIMEOUT timeout;
    al_init_timeout(&timeout, timeout_clock);

    al_wait_for_event_until(events, &event, &timeout);
    
    return event;
}



void keyboard(Event* event, int* sense_x, int* sense_y){
    if(event->type == ALLEGRO_EVENT_KEY_DOWN){
        
        switch (event->keyboard.keycode){
            case KEY_UP:
                *sense_y = -1;
                break;
            case KEY_DOWN:
                *sense_y = 1;
                break;
            case KEY_LEFT:
                *sense_x = -1;
                break;
            case KEY_RIGHT:
                *sense_x = 1;
                break;
            case KEY_ESCAPE:
                exit(0);
                break;
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_D:
                break;
        
        }
    }
    if(event->type == ALLEGRO_EVENT_KEY_UP){
        switch (event->keyboard.keycode){
            case KEY_UP:
                *sense_y = 0;
                break;
            case KEY_DOWN:
                *sense_y = 0;
                break;
            case KEY_LEFT:
                *sense_x = 0;
                break;
            case KEY_RIGHT:
                *sense_x = 0;
                break;
            case KEY_W:
            case KEY_S:
            case KEY_A:
            case KEY_D:
                break;

        }
    }
}
