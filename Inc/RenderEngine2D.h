
#ifndef RENDERENGINE2D_H_INCLUDED
#define RENDERENGINE2D_H_INCLUDED

#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<ft2build.h>
#include<ft2build.h>
#include FT_FREETYPE_H

#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<map>
#include<tuple>

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>

#include<Shader.h>
#include<Camera.h>
#include<Texture.h>

#pragma once

struct basicVertex
{
    glm::vec2 pos;
    glm::vec2 textr;
    glm::vec4 col;
    int textNr;
};

struct screenVertex
{
    glm::vec3 pos;
    glm::vec2 textr;
};

struct Character
{
    GLuint text;
    glm::ivec2 siz;
    glm::ivec2 bear;
    GLuint advance;
};

struct particle
{
    glm::vec2 pos;
    glm::vec2 velocity;
    glm::vec4 color;
    float life;
    particle():pos(0.0),velocity(0.0),color(1.0),life(0.0) {}
};

struct particleEffect
{
    int id;
    float life;
    float partcLife;
    int particleGenerationRate;
    glm::vec2 pos;
    glm::vec2 offset;
    glm::vec2 siz;
    float velocity;
    glm::vec4 color;
    int textNum;
    std::vector<particle> particles;
};

class RenderEngine2D
{

private:
    //globals/constants
    int i;
    //window data
    GLFWwindow* windax;
    int windowWidth;
    int windowHeight;
    int lastWindowWidth;
    int lastWindowHeight;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    int GLFWminorVersion;
    int GLFWmajorVersion;

    //camera values
    Camera mainCamera;

    // time measuring
    float deltaTime;
    float lastTime;
    float currentTime;

    // mouse information
    double mouseX;
    double mouseY;
    int lastMouseX;
    int lastMouseY;
    int mouseOffSetX;
    int mouseOffSetY;
    int firstMouse;

    // Matrices
    glm::mat4 projectionMatrix = glm::mat4(1.0);

    // Ubos
    unsigned int matrixUbo;
    unsigned int lightUbo;

    //lightning
    float lCutOff = glm::cos(glm::radians(12.0));
    float lOuterCutOff = glm::cos(glm::radians(15.0));
    float lightConst = 1.0;
    float lightLinear = 0.09;
    float lightQuadr = 0.032;

    //screen texture
    unsigned int basicFBO;
    unsigned int RBO;
    unsigned int screenTexture;
    unsigned int mFBO;
    unsigned int mRBO;
    unsigned int mFBOtext;

    unsigned int theVAO;
    unsigned int theVBO;
    unsigned int thePartcVAO;
    unsigned int thePartcVBO;
    unsigned int trnVAO;
    unsigned int trnVBO;
    unsigned int screenVAO;
    unsigned int screenVBO;
    unsigned int textVAO;
    unsigned int textVBO;
    unsigned int testVAO;
    unsigned int testVBO;

    std::vector<Shader*> allShaders;

    std::vector<Texture*> blockTextures;
    unsigned int textureArray;
    std::vector<Texture*> particleTextures;
    int blockTexturesAmount = 0;
    int particleTexturesAmount = 0;

    std::vector<glm::vec3> lightPositions;

    std::vector<particleEffect*> temporaryParticleEffects;
    std::vector<particleEffect*> constantParticleEffects;
    int particlesNr = 0;
    int lastUsedParticle = 0;
    int createdParticles = 0;
    std::vector<int> particleEffectDestructor;

    //rendering
    std::vector<basicVertex> vertices;
    std::vector<basicVertex> particleVertices;
    std::vector<basicVertex> transparentVertices;

    //other
    std::map<GLchar, Character> characters;

    //test
    std::vector<glm::vec2> testVertices;

    //functions
    int firstUnusedParticle(int type, int index);

    void respawnParticle(particle&,float, glm::vec2, float, glm::vec2, glm::vec4);

    std::map<char, Character> setTextType(const char*);

    static void frameBufferRez(GLFWwindow *, int, int);

    void setGLFW(const char*, const int, const int, const int, const int);

    void enableModes();

    void setDefaultShaders();

    void setVAOs();

    void setScreen();

    void updateBlockVAO();

    void updateParticleVAO();

    void updateUniforms();

    void updateTime();

    void updateViewport();

public:
    RenderEngine2D(const char*,
                   const int, const int,
                   const int, const int);

    ~RenderEngine2D();

    int setShader(const char*, const char*, int = 0, const char* = "");

    void setShaderUniforms(int,int = 0);

    void setTexture(const char*, int, int);

    void setLights(glm::vec3);

    void setText(const char*);

    void particleSetup(int=500);

    float getDeltaTime();

    GLFWwindow* getWindow();

    void writeText(int, std::string, GLfloat, GLfloat,GLfloat,glm::vec3);

    void setSprite(glm::vec2, glm::vec2, glm::vec4, int);

    void setTransparentSprite(glm::vec2, glm::vec2, glm::vec4, int);

    void createParticleEffect(glm::vec2, float, float, int, glm::vec2, float, glm::vec2,glm::vec4, int, int,int=0);

    void updateParticleEffect(int, glm::vec2);

    void makeParticleEffect();

    void clearVerts();

    void update();

    void renderBegin();

    void drawTest();

    void drawBlocks(int);

    void drawParticles(int);

    void drawTransparent(int);

    void updateScreen(int, int);

    void renderEnd();

};

#endif
