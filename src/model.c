#include "model.h"

Model* model_load(const char* filePath)
{
    MeshList* meshList = mesh_load(filePath);
    if (!meshList)
        return NULL;

    Model* model = malloc(sizeof(Model));
    model->meshCount = meshList->meshCount;
    model->meshes = malloc(sizeof(Mesh) * model->meshCount);

    // Copy the meshes from MeshList into the Model
    memcpy(model->meshes, meshList->meshes, sizeof(Mesh) * model->meshCount);

    // initialize model transform to identity
    glm_mat4_identity(model->transform);

    mesh_list_delete(meshList);     
    return model;
}

void model_delete(Model* model)
{
    if (!model) return;
    for (size_t i = 0; i < model->meshCount; ++i)
        mesh_delete(&model->meshes[i]);
    free(model->meshes);
    free(model);
}

void model_draw(const Model* model)
{
    for (size_t i = 0; i < model->meshCount; ++i)
        mesh_draw(&model->meshes[i]); 
}
