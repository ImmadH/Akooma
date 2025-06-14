#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include <string.h>

typedef struct Model 
{
  Mesh* meshes;
  size_t meshCount;
  mat4 transform;
} Model;

Model* model_load(const char* filePath);
void model_delete(Model* model);
void model_draw(const Model* model);


#endif 
