#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <glad/glad.h>
#include <string.h>

typedef struct Shader 
{
  uint32_t ID;

} Shader;


//in main: Shader myShader = createShader()
//pass file paths return the shader program to ID with compiled shaders
uint32_t create_shader(const char* vertexPath, const char* fragmentPath);
void delete_shader(uint32_t shader);
uint32_t compile_shader(uint32_t shaderType, const char* shaderSource);
void checkErrors(uint32_t shader, const char* type);
void use_shader(uint32_t ID);
char* read_file(const char* filepath);


