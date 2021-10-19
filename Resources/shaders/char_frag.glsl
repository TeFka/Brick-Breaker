#version 440

out vec4 daColor;

in vec2 textr;
uniform sampler2D textur;
uniform vec3 textCol;

void main(){

  vec4 sampled = vec4(1.0,1.0,1.0,texture(textur,textr).r);
  daColor = vec4(textCol,1.0)*sampled;

}
