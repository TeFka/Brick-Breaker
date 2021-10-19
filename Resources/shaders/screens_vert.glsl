#version 440

layout(location=0) in vec4 vert;

out vec2 textCo;

uniform int mode;
uniform float time;

void main(){
    gl_Position=vec4(vert.xy,0.0,1.0);
    vec2 textar = vec2(vert.z,vert.w);
    vec2 coord;
    if(mode==0){
       coord=textar;
    }
    else if(mode==1){
       float strength = 0.1;
       gl_Position.x +=sin(time*30)*strength;
       gl_Position.y +=sin(time*30)*strength;
       coord=textar;
    }
    else if(mode==2){
       coord = vec2(1.0-textar.x,1.0-textar.y);
    }
    else if(mode==3){
       float strength = 0.3;
       vec2 pos = vec2(textar.x + sin(time) * strength, textar.y + cos(time) * strength);
       coord = pos;
    }
    textCo = coord;
}
