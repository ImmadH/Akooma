#ifndef MESH_H
#define MESH_H
#include <cglm.h>
#include <cgltf.h>
#include <stddef.h>
#include <glad.h>
typedef struct Vertex 
{
  vec3 position;
  vec3 normal;
  vec2 texCoord;

} Vertex;

typedef struct Mesh 
{
  Vertex* vertices;
  uint32_t* indices;
  size_t vertexCount;
  size_t indexCount;

  uint32_t VAO;
  uint32_t VBO;
  uint32_t EBO;

} Mesh;

typedef struct MeshList
{
  Mesh* meshes;
  size_t meshCount;
} MeshList;




MeshList* mesh_load(const char* filePath);
void mesh_gpu_upload(Mesh* mesh);
void mesh_delete(Mesh* mesh);
void mesh_list_delete(MeshList* list);
//After we store all primitive data into mesh 
//we store mesh-> meshLists and that will be used in our Model 
//System!

#endif



