#ifndef CAMERA_H
#define CAMERA_H
#include <stdint.h>
#include <cglm/cglm.h>

typedef enum
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
} camera_movement;


typedef struct Camera
{
  vec3 position;
  vec3 front;
  vec3 up;
  vec3 right;
  vec3 world_up;

  float yaw;
  float pitch;
  float movement_speed;
  float mouse_sensitivity;

} Camera;

//TODO:
//calculate delta time in main 
//-camera initialization // we need to define default values like cameraPos, yaw, pitch.
//and pass delta time 
//
//-Within init we update camera vectors?
//
//-process keyboard input
//-process mouse movement 
//-need a function to calculate view matrix


//Misc but dont forget!:
//-Rewrite shaders
//-Projection and Model matrix!!!
//-Make SDL handle the input ->WASD (is pressed?) -> call process functions



#endif
