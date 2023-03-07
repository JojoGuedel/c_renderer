#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
#include "glad/gl.h"

typedef const char *string;

float vertices[] = {
    -0.3f, -0.3f, 0.0f, 
    -0.3f, 0.3f, 0.0f, 
    0.3f, 0.3f, 0.0f,
};

unsigned int vertex_shader;
unsigned int fragment_shader;

unsigned int shader_program;

unsigned int vbo;
unsigned int vao;

char* load_shader_src(const char* path) {
    char* buffer;
    FILE* file;
    long size;

    if(fopen_s(&file, path, "rb"))
        printf("[Error]: Could not open '%s'.\n", path);

    fseek(file, 1, SEEK_END);
    size = ftell(file);
    buffer = malloc(size);
    fread(buffer, 1, size, file);
    fclose(file);

    return buffer;
}

void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
    printf("[Info]: keypress (%c,%i,%i,%i)\n", key, scancode, action, mods);
}

void glfw_error_cb(int error_code, const char* description) {
    printf("[Error]: (glfw%i) %s", error_code, description);
}

void GLAPIENTRY gl_error_cb(
    GLenum source, 
    GLenum type, 
    GLuint id, 
    GLenum severity, 
    GLsizei length, 
    const GLchar *message, 
    const void *userParam
    ) {
    printf(
        "[Error]: (opengl) %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
        message
    );
}

void init() {
    /* setup vertex shader */
    const char* vertex_shader_src = load_shader_src("main.vert");
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("[Error]: vertexshader could not be compiled.\n%s\n", infoLog);
    }

    /* setup fragment shader */

    // TODO: continue...
    
    free((char*)vertex_shader_src);

    /* setup vertex buffer*/
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, sizeof(vertices) / sizeof(float) / 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    /* setup vertex array */
    glGenVertexArrays(1, &vao);
}

void  render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, string args[]) {
    GLFWwindow *window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_cb);
    glfwSetErrorCallback(glfw_error_cb);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_error_cb, 0);

    while (!glfwWindowShouldClose(window)) {
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}