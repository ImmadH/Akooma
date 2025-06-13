#include "gui.h"
#include <cimgui.h>
#include <cimgui_impl.h>
#include <SDL3/SDL.h>
void gui_init(SDL_Window* window, SDL_GLContext context)
{
    igCreateContext(NULL);
    igStyleColorsDark(NULL);
    
    ImGui_ImplSDL3_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 460");

    ImGuiIO* io = igGetIO_Nil();
    ImFontAtlas_AddFontDefault(io->Fonts, NULL);
    io->FontGlobalScale = 2.5f;
}

void gui_new_frame(bool active)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    igNewFrame();
    if(active)
    {
      igBegin("Akuma", NULL, 0);               
      igText("Hello Text!");
      igEnd();
    }
}
    

void gui_render(void)
{
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void gui_shutdown(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    igDestroyContext(NULL);
}

bool gui_process_event(SDL_Event* event)
{
  return ImGui_ImplSDL3_ProcessEvent(event);
}


void gui_active(SDL_Window* window, bool active)
{
      SDL_SetWindowMouseGrab(window, !active);
      SDL_SetWindowRelativeMouseMode(window, !active);
      if(active)
         SDL_ShowCursor();
      else
      {
         SDL_HideCursor();
         float dx, dy;
         SDL_GetRelativeMouseState(&dx, &dy);
      }
}
