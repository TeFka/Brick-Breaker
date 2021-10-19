#version 440

out vec4 daColor;

in vec2 textCo;
flat in int textNr;
in vec4 color;

uniform sampler2D textr[];

void main()
{
    vec4 textCol;
    switch(textNr)
    {
    case 0:
        textCol = texture(textr[0],textCo);
        break;
    case 1:
        textCol = texture(textr[1],textCo);
        break;
    case 2:
        textCol = texture(textr[2],textCo);
        break;
    case 3:
        textCol = texture(textr[3],textCo);
        break;
    case 4:
        textCol = texture(textr[4],textCo);
        break;
    case 5:
        textCol = texture(textr[5],textCo);
        break;
    case 6:
        textCol = texture(textr[6],textCo);
        break;
    case 7:
        textCol = texture(textr[7],textCo);
        break;
    case 8:
        textCol = texture(textr[8],textCo);
        break;
    case 9:
        textCol = texture(textr[9],textCo);
        break;
    case 10:
        textCol = texture(textr[10],textCo);
        break;
    case 11:
        textCol = texture(textr[11],textCo);
        break;
    case 12:
        textCol = texture(textr[12],textCo);
        break;
    case 13:
        textCol = texture(textr[13],textCo);
        break;
    case 14:
        textCol = texture(textr[14],textCo);
        break;
    }
    if(textNr==15)
    {
        daColor = color;
    }
    else
    {
        if(textCol.a<0.1)
        {
            discard;
        }
        else
        {
            daColor = (color*vec4(textCol.rgb,1.0));
        }

    }
}
