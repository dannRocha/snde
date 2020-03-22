#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "../types/types.h"



Actor create_character(const char* filename, int width, int height, int offset_x, int offset_y, double scale)
{

    if(width <= 0 || height <= 0 || scale <= 0)
    {
        fprintf(stderr, "\nError: Largura, Altura e \"Scale\" nao pode ser menor ou igual a 0 (zero)\n");
        exit(-1);
    }

    Actor character;

    character.spritesheet = load_image(filename);
    
    scale = (scale) ? scale : 1.0;

    character.dimen.w = width * scale;
    character.dimen.h = height * scale;
    character.coord.x = 0;
    character.coord.y = 0;
    character.scale = scale;


    character.animation.number_of_animations              = al_get_bitmap_height(character.spritesheet) / height;
    character.animation.number_of_sprites_per_animation   = al_get_bitmap_width(character.spritesheet) / width + 1;

    character.mapping = (Sprites** ) calloc(character.animation.number_of_animations, sizeof(Sprites *));

    if(character.mapping == NULL)
        fprintf(stderr, "Error ao mapear SPRITESHEET: %s\n", filename);
    
    for(int i = 0; i < character.animation.number_of_animations; i++)
        character.mapping[i] = (Sprites* ) calloc(character.animation.number_of_sprites_per_animation , sizeof(Sprites ));



    for(int i = 0; i < character.animation.number_of_animations; i++)
        for(int j = 0; j < character.animation.number_of_sprites_per_animation; j++)
        {
            character.mapping[i][j].w = width;
            character.mapping[i][j].h = height;
            character.mapping[i][j].x = j * width;
            character.mapping[i][j].y = i * height;
        }

    character.animation.status.current_animation = 0;
    character.animation.status.current_sprite = 0;


    return character;

}



void run_animation(Actor *character, double framesperseconds, int select_animation, AnimationControl control)
{
    if(select_animation >= character->animation.number_of_animations || select_animation < 0)
    {
        fprintf(
            stderr,
            "\nError: Animacao selecionada nao existe| Voce pode selecionar de 0 a %d | voce selecionou %d\n\n",
            character->animation.number_of_animations - 1,
            select_animation
         );
        exit(-1);
    }

    if(framesperseconds <= 0.0)
    {
        fprintf( stderr, "\nError: Frames por segundos não pode ser menor ou igual 0.0\n\n");
        exit(-1);
    }

    if(control.len > character->animation.number_of_sprites_per_animation)
    {
        fprintf(
            stderr,
            "\nNumero de Sprites em animation_control execede as animações do spritesheet: Selecionado: %d - Maximo: %d\n",
                control.len, character->animation.number_of_sprites_per_animation
            );
        exit(-1);
    }

    character->animation.status.current_animation = select_animation;
 

    if(!character->animation.status.configured)
    {    
        character->event_queue = al_create_event_queue();
        character->animation.time = al_create_timer(1.0 / framesperseconds);
        character->animation.status.configured = true;
        character->animation.status.running = true;
        character->animation.control = control;
        al_register_event_source(character->event_queue, al_get_timer_event_source(character->animation.time));
        al_start_timer(character->animation.time);

        for(int i = 0; i < character->animation.control.len; i++){
            
            if(character->animation.control.len > character->animation.number_of_sprites_per_animation
            || character->animation.control.sequence_of_sprites[i] > character->animation.number_of_sprites_per_animation)
            {
                fprintf(
                    stderr,
                    "\nError: Quantidade de sprite no \"controle\":|%d| execede os sprites \"armazenados\": |%d|\n",
                    character->animation.control.len, character->animation.number_of_sprites_per_animation
                );
                exit(-1);
            }
        }

    }
    
    Event event = get_event(character->event_queue);

   
    if(event.type == ALLEGRO_EVENT_TIMER && character->animation.status.running)
    {
        if((intptr_t) event.timer.source == (intptr_t) character->animation.time)
        {   
            enum animation_control
            {
                starting_sprite,
                next_sprite 
            };
            
            if(character->animation.control.enable)
            {
                character->animation.status.current_sprite = character->animation.control.sequence_of_sprites[character->animation.control.next];
                character->animation.control.next += next_sprite;

                if(character->animation.control.next >= character->animation.control.len)
                    character->animation.control.next = starting_sprite;

            } else {
                
                character->animation.status.current_sprite += next_sprite;
                
                if(character->animation.status.current_sprite >= character->animation.number_of_sprites_per_animation)
                    character->animation.status.current_sprite = starting_sprite;    
            }
        }
    }
   

    al_draw_scaled_bitmap(
        character->spritesheet, 
        character->mapping[character->animation.status.current_animation][character->animation.status.current_sprite].x, 
        character->mapping[character->animation.status.current_animation][character->animation.status.current_sprite].y,
        character->mapping[character->animation.status.current_animation][character->animation.status.current_sprite].w, 
        character->mapping[character->animation.status.current_animation][character->animation.status.current_sprite].h, 
        character->coord.x, 
        character->coord.y, 
        character->dimen.w, 
        character->dimen.h,
        0);
}



AnimationControl animation_control(int number_sprites, ...)
{
    enum {
        min_number_of_sprites = 0
    };
    
    va_list parameter_list;
    
    va_start(parameter_list, number_sprites);
    AnimationControl control;
    
    control.sequence_of_sprites = (int* ) calloc(number_sprites, sizeof(int));
    control.len = number_sprites;
    control.next = 0;
    control.enable = true;
    
    for(int i = 0; i < control.len; i++){
        control.sequence_of_sprites[i] = va_arg(parameter_list, int);
        
        if(control.sequence_of_sprites[i] < min_number_of_sprites)
        {
            fprintf(stderr, "\nError: Sprite nao existe ou numero de animacoes incorreta\n");
            exit(-1);
        }
    }
    
    va_end(parameter_list);
    
    return control;
}