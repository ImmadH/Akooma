// gui.h
#ifndef GUI_H
#define GUI_H

#include <SDL3/SDL.h>


void gui_init(SDL_Window* window, SDL_GLContext context);


void gui_shutdown(void);


void gui_new_frame(void);


void gui_render(void);

bool gui_process_event(SDL_Event* e);

#endif // GUI_H

