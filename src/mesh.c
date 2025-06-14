#include "mesh.h"
#include <stdlib.h>
//MAKE SURE TO FREE THE LOADED DATA!!!
// Parse gltf
// Extract vertices/indices
// Upload to GPU
// Return Mesh*
MeshList* mesh_load(const char* filePath)
{
  cgltf_options options = {0};
  cgltf_data* data = NULL;
  cgltf_result result = cgltf_parse_file(&options, filePath, &data);
  if(result != cgltf_result_success)
  {
    //Might log auto? If not add a sanity check here
    return NULL;
  }

  result = cgltf_load_buffers(&options, data, filePath);
  if (result != cgltf_result_success)
  {
    return NULL;
  }

  //log the prim count
  size_t primitiveCount = 0;
  for (size_t i = 0; i < data->meshes_count; ++i)
  {
    primitiveCount += data->meshes[i].primitives_count;
  }
  
  MeshList* meshList = malloc(sizeof(MeshList));
  meshList->meshes = malloc(sizeof(Mesh) * primitiveCount);
  meshList->meshCount = 0;


  //now loaded -> extract vertices and indices etc.
  for (int i = 0; i < data->meshes_count; ++i)
  {
    cgltf_mesh* mesh = &data->meshes[i];
    for (int j = 0; j < mesh->primitives_count; ++j)
    {
      cgltf_primitive* primitives = &mesh->primitives[j];
      //extract data of prim and populate mesh struct  
      Mesh mesh;
      const cgltf_accessor* position_accessor = NULL;
      const cgltf_accessor* normal_accessor = NULL;
      const cgltf_accessor* texcoord_accessor = NULL;

      for (size_t k = 0; k < primitives->attributes_count; ++k)
      {
        cgltf_attribute* attr = &primitives->attributes[k];
        if (attr->type == cgltf_attribute_type_position) position_accessor = attr->data;
        if (attr->type == cgltf_attribute_type_normal) normal_accessor = attr->data;
        if (attr->type == cgltf_attribute_type_texcoord) texcoord_accessor = attr->data;
      }
      

      mesh.vertexCount = position_accessor->count;
      mesh.vertices = malloc(sizeof(Vertex) * mesh.vertexCount);
      

      for (size_t v = 0; v < mesh.vertexCount; ++v) 
      {
        cgltf_accessor_read_float(position_accessor, v, mesh.vertices[v].position, 3);
        if (normal_accessor)
           cgltf_accessor_read_float(normal_accessor, v, mesh.vertices[v].normal, 3);
        if (texcoord_accessor)
           cgltf_accessor_read_float(texcoord_accessor, v, mesh.vertices[v].texCoord, 2);
      }
      
      if (primitives->indices) 
      {
        const cgltf_accessor* index_accessor = primitives->indices;
        mesh.indexCount = index_accessor->count;
        mesh.indices = malloc(sizeof(uint32_t) * mesh.indexCount);

        for (size_t i = 0; i < index_accessor->count; ++i) 
        {
          cgltf_uint index;
          cgltf_accessor_read_uint(index_accessor, i, &index, 1);
          mesh.indices[i] = (uint32_t)index;
        }
      } 
      else 
      {
        mesh.indexCount = 0;
        mesh.indices = NULL;
      }
      
      mesh_gpu_upload(&mesh);
      meshList->meshes[meshList->meshCount++] = mesh;
    }
  }
  
  cgltf_free(data);
  return meshList;
}

void mesh_gpu_upload(Mesh *mesh)
{
  glGenVertexArrays(1, &mesh->VAO);
  glGenBuffers(1, &mesh->VBO);
  glGenBuffers(1, &mesh->EBO);

  glBindVertexArray(mesh->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertexCount, mesh->vertices, GL_STATIC_DRAW);

  if (mesh->indexCount > 0) 
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh->indexCount, mesh->indices, GL_STATIC_DRAW);
  }

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  // Normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  // TexCoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  free(mesh->vertices);
  free(mesh->indices);
  mesh->vertices = NULL;
  mesh->indices = NULL;
}

void mesh_delete(Mesh* mesh) 
{
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
    glDeleteVertexArrays(1, &mesh->VAO);
    free(mesh->vertices);
    free(mesh->indices);
}

void mesh_list_delete(MeshList* list) 
{
    for (size_t i = 0; i < list->meshCount; ++i)
        mesh_delete(&list->meshes[i]);
    free(list->meshes);
    free(list);
}
