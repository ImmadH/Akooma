#ifndef GUI_H
#define GUI_h

#ifdef __cplusplus
extern "C" {
#endif
#include <SDL3/SDL.h>

void gui_init(SDL_Window* window, SDL_GLContext context); 
void gui_new_frame(void);
void gui_render(void);
void gui_shutdown(void);
void gui_process_event(SDL_Event* event);

#ifdef __cplusplus
}
#endif

#endif
