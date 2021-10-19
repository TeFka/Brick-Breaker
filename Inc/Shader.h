
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<cmath>

#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<map>
#include<tuple>

#pragma once

class Shader
{
    //members
    std::string loadShaderSource(char*) const;

    GLuint loadShader(GLenum, char*) const;

    void linkProgram(GLuint, GLuint, GLuint);

public:
    GLuint id;
    //contstructor
    Shader(const char*, const char*, int=0, const char* = "");

    ~Shader();

    void use() const;

    void unUse() const;

    void set1i(const GLint&, const GLchar*) const;

    void set1f(const GLfloat&, const GLchar*) const;

    void setVec2f(const glm::fvec2&, const GLchar*) const;

    void setVec3f(const glm::fvec3&, const GLchar*) const;

    void setVec4f(const glm::fvec4&, const GLchar*) const;

    void setMat3(const glm::mat3&, const GLchar*, GLboolean=GL_FALSE) const;

    void setMat4(const glm::mat4&, const GLchar* name, GLboolean=GL_FALSE) const;

    void setUb(const GLchar*,GLuint);
};

#endif
