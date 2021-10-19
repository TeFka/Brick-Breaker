#version 440
layout (location = 0) in vec2 aPos;

layout (std140) uniform matrixes
{
    mat4 projectionMatrix;
};

void main()
{
    gl_Position = projectionMatrix*vec4(aPos.x, aPos.y, 0.0, 1.0);
}
