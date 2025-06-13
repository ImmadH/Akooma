#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>
#include <glad.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "shader.h"
#include "camera.h"
#include "gui.h"

void camera_handle_input(Camera* camera, float deltaTime);

int main()
{
  unsigned int SCR_WIDTH = 2560;
  unsigned int SCR_HEIGHT = 1440;
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  
  SDL_Window* WINDOW = SDL_CreateWindow("Akuma", SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
  if (!WINDOW)
  {
    printf("Failed to create SDL Window\n");
    SDL_Quit();
    return -1;
  }
  
  SDL_SetWindowMouseGrab(WINDOW, true);
  SDL_SetWindowRelativeMouseMode(WINDOW, true);
  SDL_HideCursor();

  SDL_GLContext CONTEXT = SDL_GL_CreateContext(WINDOW);
  if (!CONTEXT)
  {
    printf("Failed to create an OPENGL CONTEXT\n");
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
    return -1;
  }

  SDL_GL_MakeCurrent(WINDOW, CONTEXT);

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    printf("Failed to load GLAD\n");
    SDL_GL_DestroyContext(CONTEXT);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
  }

  gui_init(WINDOW, CONTEXT);

  Camera camera;
  vec3 startPos = {0.0f, 0.0f, 3.0f};
  vec3 upVector = {0.0f, 1.0f, 0.0f};
  camera_init(&camera, startPos, upVector, -90.0f, 0.0f);
  mat4 projection;
  glm_perspective(glm_rad(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f, projection);

  Shader colorShader;
  shader_create(&colorShader, "shaders/vert.glsl" , "shaders/frag.glsl");
 
  float vertices[] =
  {
    -0.5f, -0.5f, 0.0f, // left  
     0.5f, -0.5f, 0.0f, // right 
     0.0f,  0.5f, 0.0f  // top   
  };

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //unbind here
  
  bool running = true;
  bool guiActive = false;
  bool lastGuiState = guiActive;
  SDL_Event e;
  uint64_t lastTime = SDL_GetTicks();

  while(running)
  {
    while(SDL_PollEvent(&e))
    {
      gui_process_event(&e);
      switch (e.type) 
      {
        case SDL_EVENT_QUIT:
        running = false;
        break;
        case SDL_EVENT_KEY_DOWN:
          switch(e.key.key)
          {
            case SDLK_ESCAPE:
                 running = false;
                 break;
            case SDLK_INSERT:
                 guiActive = !guiActive;
                 break;
          }
      }

    }

    if (guiActive != lastGuiState) 
    {
      gui_active(WINDOW, guiActive);
      lastGuiState = guiActive;
    }

    //delta time 
    uint64_t currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f; // seconds
    lastTime = currentTime;
    
    if (!guiActive)
    {
      camera_handle_input(&camera, deltaTime);
    }
  
    gui_new_frame(guiActive);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    mat4 view;
    camera_get_view_matrix(&camera, view);

    shader_use(&colorShader); 
    glUniformMatrix4fv(glGetUniformLocation(colorShader.ID, "view"), 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(glGetUniformLocation(colorShader.ID, "projection"), 1, GL_FALSE, (float*)projection);


    glBindVertexArray(VAO);
    

    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    
    gui_render();
    
    
    SDL_GL_SwapWindow(WINDOW);

  }
  SDL_GL_DestroyContext(CONTEXT);
	SDL_DestroyWindow(WINDOW);
	SDL_Quit();
  gui_shutdown();
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  shader_delete(&colorShader); 

  return 0;
}


void camera_handle_input(Camera *camera, float deltaTime) 
{
    const bool* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W]) camera_process_keyboard(camera, FORWARD, deltaTime);
    if (keys[SDL_SCANCODE_S]) camera_process_keyboard(camera, BACKWARD, deltaTime);
    if (keys[SDL_SCANCODE_A]) camera_process_keyboard(camera, LEFT, deltaTime);
    if (keys[SDL_SCANCODE_D]) camera_process_keyboard(camera, RIGHT, deltaTime);
    if (keys[SDL_SCANCODE_SPACE]) camera_process_keyboard(camera, UP, deltaTime);
    if (keys[SDL_SCANCODE_LCTRL]) camera_process_keyboard(camera, DOWN, deltaTime);

    float dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    camera_process_mouse(camera, dx, -dy);  
}










