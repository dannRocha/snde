#ifndef __ALLEGRO_ABST__H__
#define __ALLEGRO_ABST__H__


#include "functions.c"


#ifndef __INIT_SNDE_H__
#define __INIT_SNDE_H__

bool init_all_lib(void);

#endif // __INIT_SNDE_H__


#ifndef __WINDOW_SNDE_H__
#define __WINDOW_SNDE_H__

Window create_window(int width, int height, const String title);

#endif //__WINDOW_SNDE_H__


#ifndef __IMAGE_SNDE_H__
#define __IMAGE_SNDE_H__

Image load_image(const String src);
void draw_image(Image img, float x,float y, double scale, int flags );

#endif //__IMAGE_SNDE_H__


#ifndef __EVENT_SNDE_H__
#define __EVENT_SNDE_H__

Events add_event_listener(Window screen);
Event get_event(Events events);
Event getEventKeyboard(Events events);

#endif //__EVENT_SNDE_H__


#ifndef __SCREEN_SNDE_H__
#define __SCREEN_SNDE_H__

void draw_color_background(char red, char green, char blue);
void update_screen(double frameperseconds);

#endif //__SCREEN_SNDE_H__


#ifndef __UTILS_SNDE_H__
#define __UTILS_SNDE_H__

void destroy(void* element, const String type);
void keyboard(Event* event, int* sense_x, int* sense_y);

#endif //__UTILS_SNDE_H__


#endif //__ALLEGRO_ABST__H__



