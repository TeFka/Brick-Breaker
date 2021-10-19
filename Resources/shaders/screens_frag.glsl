#version 440

out vec4 daColor;

in vec2 textCo;

uniform sampler2D textr;
uniform vec2 offsets[9];
uniform int edgeKern[9];
uniform float blurKern[9];
uniform int mode;

void main(){
   vec4 color = vec4(0.0);
   vec3 samples[9];
   if(mode==1||mode==3){
     for(int i=0;i<9;i++){
        samples[i]=vec3(texture(textr,textCo.st+offsets[i]).rgb);
     }
   }

   if(mode==0){
      color = vec4(texture(textr,textCo).rgb,1.0);
   }
   else if(mode==1){
       for(int i = 0; i < 9; i++){
        color += vec4(samples[i] * blurKern[i], 0.0f);
       }
       color.a = 1.0f;

    }
    else if(mode==2){
       color = vec4(1.0-texture(textr,textCo).rgb,1.0);
    }
    else if(mode==3){
       for(int i = 0; i < 9; i++){
        color += vec4(samples[i] * edgeKern[i], 0.0f);
       }
       color.a = 1.0f;
    }
    daColor = color;
}
