#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

typedef struct {
    int id;
} shader;

shader* shader_create(const char* vert_path, const char* frag_path);
void shader_destroy(shader* shader);

void shader_use(shader* shader);
void shader_set_bool(shader* shader, const char* name, bool value);
void shader_set_int(shader* shader, const char* name, int value);
void shader_set_float(shader* shader, const char* name, float value);

#endif