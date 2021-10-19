
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<map>
#include<tuple>

#include<SOIL2/SOIL2.h>

class Texture
{
private:
    GLuint id;
    int height;
    int width;
    unsigned int type;
    std::string typ;
    const char* path;
    GLint textureUnit;
public:
    Texture(const char*,GLenum,GLint,std::string = "texture_diffuse");

    ~Texture();

    inline GLuint getId() const;

    std::string getTyp();

    const char* getPath();

    void bindT();

    void unBindT();

    GLuint getUnit() const;

    void loadNewTex(const char*,GLenum);

};

#endif
