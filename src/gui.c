#include "gui.h"
#include <cimgui.h>
#include <cimgui_impl.h>

void gui_init(SDL_Window* window, SDL_GLContext context)
{
    igCreateContext(NULL);
    igStyleColorsDark(NULL);

    ImGui_ImplSDL3_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void gui_new_frame(void)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    igNewFrame();


    igBegin("Akuma", NULL, 0);               
    igText("Hello Text!");
    igEnd();
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

