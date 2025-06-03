#include "shader.h"


uint32_t create_shader(const char* vertexPath, const char* fragmentPath)
{
  char* vertexSource = read_file(vertexPath);
  char* fragmentSource = read_file(fragmentPath);
  if (!vertexSource || !fragmentSource)
  {
    printf("Failed to load shaders from file\n");
    return 0;
  }
 
  
  uint32_t VS, FS, ID;
  VS = compile_shader(GL_VERTEX_SHADER, vertexSource);
  FS = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);

  ID = glCreateProgram();
  glAttachShader(ID, VS);
  glAttachShader(ID, FS);
  glLinkProgram(ID);
  checkErrors(ID, "PROGRAM");
  
  
  glDeleteShader(VS);
  glDeleteShader(FS);

  free(vertexSource);
  free(fragmentSource);


  return ID;
}

void delete_shader(uint32_t shader)
{
  glDeleteProgram(shader);
}


uint32_t compile_shader(uint32_t shaderType, const char* shaderSource)
{
  uint32_t shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  checkErrors(shader, "SHADER");
  return shader;
}

void checkErrors(uint32_t shader, const char* type)
{
  int success;
  char infoLog[1024];

  if (strcmp(type, "SHADER") == 0)
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); 
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      printf("Failed to compile shader%s\n", infoLog);
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      printf("Failed to Link Program%s\n" , infoLog);
    }
  }
}


void use_shader(uint32_t ID)
{
  glUseProgram(ID);
}


char* read_file(const char* filepath)
{
    FILE* file = fopen(filepath, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = malloc(length + 1); // +1 for null terminator
    if (!buffer) return NULL;

    fread(buffer, 1, length, file);
    buffer[length] = '\0'; 

    fclose(file);
    return buffer;
}

