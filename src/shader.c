#include <stdio.h>
#include <stdlib.h>
#include "glad/gl.h"
#include "shader.h"

char* _load_src(const char* path) {
    FILE* file;

    if(fopen_s(&file, path, "rb")) {
        printf("[Error]: File could not be opened ('%s').\n", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    int size = ftell(file); 
    char* buffer = malloc(size + 1);
    buffer[size] = '\0';

    rewind(file);
    fread(buffer, 1, size, file);
    fclose(file);

    printf("[Info] - loading shader: \n%s\n", buffer);

    return buffer;
}

void _check_shader_error(GLuint shader) {
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("[Error] - opengl shader: %s\n", infoLog);
    }
}

void _check_program_error(GLuint program) {
    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("[Error] - opengl shader program: %s\n", infoLog);
    }
}

shader* shader_create(const char* vert_path, const char* frag_path) {
    shader* s = malloc(sizeof(shader));

    /* setup vertex shader */
    const char* vert_src = _load_src(vert_path);
    int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_src, NULL);
    glCompileShader(vert);
    free((char*)vert_src);
    _check_shader_error(vert);

    /* setup fragment shader */
    const char* frag_src = _load_src(frag_path);
    int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_src, NULL);
    glCompileShader(frag);
    free((char*) frag_src);
    _check_shader_error(frag);

    /* setup shader program */
    s->id = glCreateProgram();
    glAttachShader(s->id, vert);
    glAttachShader(s->id, frag);

    glLinkProgram(s->id);
    _check_program_error(s->id);
    
    glDeleteShader(vert);
    glDeleteShader(frag);

    return s;
}

void shader_destroy(shader *shader) {
    free(shader);
}

void shader_use(shader* shader) {
    glUseProgram(shader->id);
}

void shader_set_bool(shader* shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value); 
}

void shader_set_int(shader* shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_float(shader* shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}