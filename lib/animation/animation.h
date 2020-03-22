#ifndef __SNDE_ANIMATION_ACTOR__
#define __SNDE_ANIMATION_ACTOR__

#include "animation.c"

Actor create_character(const char* filename, int width, int height, int offset_x, int offset_y, double scale);
void run_animation(Actor *character, double framesperseconds, int select_animation, AnimationControl control);
AnimationControl animation_control(int number_sprites, ...);


#endif //__SNDE_ANIMATION_ACTOR__
