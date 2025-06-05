#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include "shader.h"
#include "camera.h"

//-What is project Akuma?
//Overall Goal: Create a renderer which will serve as a testbed for creating shaders
//This is a test of git

int main()
{
  unsigned int SCR_WIDTH = 1920;
  unsigned int SCR_HEIGHT = 1080;
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  
  SDL_Window *WINDOW = SDL_CreateWindow("Akuma", SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
  if (!WINDOW)
  {
    printf("Failed to create SDL Window\n");
    SDL_Quit();
    return -1;
  }

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

  Shader colorShader;
  create_shader(&colorShader, "shaders/vert.glsl" , "shaders/frag.glsl");
 
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
  SDL_Event e;

  while(running)
  {
    while(SDL_PollEvent(&e))
    {
      switch (e.type) 
      {
        case SDL_EVENT_QUIT:
        running = false;
        break;
      }
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.5f, 0.4f, 0.6f, 1.0f);

    glBindVertexArray(VAO);
    use_shader(&colorShader);

    glDrawArrays(GL_TRIANGLES, 0, 3);


    SDL_GL_SwapWindow(WINDOW);

  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  delete_shader(&colorShader);

  return 0;
}










