#version 440

layout(location=0) in vec4 pos;
out vec2 textr;

uniform mat4 projectionMatrix;

void main(){

  gl_Position = projectionMatrix*vec4(pos.xy,0.0,1.0);
  textr = vec2(pos.z,pos.w);
}
