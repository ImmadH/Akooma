#ifndef CAMERA_H
#define CAMERA_H
#include <stdint.h>
#include "cglm.h"

typedef enum
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
} cameraMovement;


typedef struct Camera
{
  vec3 position;
  vec3 front;
  vec3 up;
  vec3 right;
  vec3 world_up;

  float yaw;
  float pitch;
  float speed;
  float sensitivity;

} Camera;


void update_camera_vectors(Camera* camera);
void camera_init(Camera* camera, vec3 camPos, vec3 up, float yaw, float pitch);
void camera_process_keyboard(Camera* camera, cameraMovement direction, float deltaTime);
void camera_process_mouse(Camera* camera, float xoffset, float yoffset);
void camera_get_view_matrix(Camera* camera, mat4 dest);




#endif
