#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

typedef const char *string;

float vertices[] = {
    -0.3f, -0.3f, 0.0f, 
    -0.3f, 0.3f, 0.0f, 
    0.3f, 0.3f, 0.0f,
};

unsigned int indices[] = {
    0, 1, 2,
};

unsigned int vertex_shader;
unsigned int fragment_shader;

unsigned int shader_program;

unsigned int vbo;
unsigned int vao;
unsigned int ebo;

char* load_shader_src(const char* path) {
    FILE* file;

    if(fopen_s(&file, path, "rb")) {
        printf("[Error]: Could not open '%s'.\n", path);
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

void check_error(GLuint shader, GLenum name) {
    int success;
    char infoLog[512];

    glGetShaderiv(shader, name, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("[Error] - opengl shader: %s\n", infoLog);
    }
}

void init() {
    /* setup vertex shader */
    const char* vertex_shader_src = load_shader_src("./src/shader/main.vert");
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    free((char*)vertex_shader_src);
    check_error(vertex_shader, GL_COMPILE_STATUS);

    /* setup fragment shader */
    const char* fragment_shader_src = load_shader_src("./src/shader/main.frag");
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    free((char*) fragment_shader_src);
    check_error(fragment_shader, GL_COMPILE_STATUS);
    
    /* setup shader program */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /* setup vertex array */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    /* setup vertex buffer*/
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, sizeof(vertices) / sizeof(float) / 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    /* setup element buffer*/
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
}

void  render() {
    glUseProgram(shader_program);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

    while (!glfwWindowShouldClose(window)) {
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}