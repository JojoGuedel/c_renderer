#version 460 core

in vec4 vert_color;
out vec4 FragColor;

void main()
{
    FragColor = vert_color;
}