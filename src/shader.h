#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

typedef struct {
    int id;
} Shader;

Shader shader_create(const char* vert_path, const char* frag_path);
void shader_destroy(Shader* shader);

void shader_use(Shader* shader);

#endif