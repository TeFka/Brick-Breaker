#version 440

layout(location=0) in vec2 pos;
layout(location=1) in vec2 textCoord;
layout(location=2) in vec4 col;
layout(location=3) in int textN;

out vec2 textCo;
out vec4 color;
flat out int textNr;

layout (std140) uniform matrixes
{
    mat4 projectionMatrix;
};

void main(){
    gl_Position = projectionMatrix*vec4(pos,0.0,1.0);
    color = col;
    textCo = textCoord;
    textNr = textN;
}
