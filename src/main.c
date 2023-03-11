#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

typedef const char *string;

float vertices[] = {
    -0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.3f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.3f, 0.3f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
    0, 1, 2,
};

shader* shader_program;

unsigned int vbo;
unsigned int vao;
unsigned int ebo;

void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
    printf("[Info] - keypress: (%c,%i,%i,%i)\n", key, scancode, action, mods);
}

void glfw_error_cb(int error_code, const char* description) {
    printf("[Error] - glfw(%i): %s", error_code, description);
}

void GLAPIENTRY gl_debug_cb(
    GLenum source, 
    GLenum type, 
    GLuint id, 
    GLenum severity, 
    GLsizei length, 
    const GLchar *message, 
    const void *userParam
    ) {
    printf(
        "[%s] - opengl: type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR? "Error" : "Info"),
        type, 
        severity,
        message
    );
}

void init() {
    shader_program = shader_create("./src/shader/main.vert", "./src/shader/main.frag");

    /* setup vertex array */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    /* setup vertex buffer*/
    int vertices_count = sizeof(vertices) / sizeof(float) / 6;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /* positions */
    glVertexAttribPointer(0, vertices_count, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* colors */
    glVertexAttribPointer(1, vertices_count, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* setup element buffer*/
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
}

void  render() {
    shader_use(shader_program);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    /* normal (GL_FILL) / wireframe (GL_LINE) mode */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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
    glDebugMessageCallback(gl_debug_cb, 0);

    init();

    int n;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
    printf("[Info]: Maximum nr of vertex attributes supported: %i\n", n);

    while (!glfwWindowShouldClose(window)) {
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_destroy(shader_program);

    glfwTerminate();

    return 0;
}