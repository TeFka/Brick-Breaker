
#ifndef BRICKGAMECLASS_H_INCLUDED
#define BRICKGAMECLASS_H_INCLUDED

#include<glad/glad.h>
#include<GLFW/glfw3.h>

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

#include<gameLevel.h>

struct effect
{
    int effectNum;
    float lifetime;
    int exsist;
};

enum dirr
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};
enum gameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_LOST,
    GAME_WIN
};

struct Bullet
{
    glm::vec3 pos;
    glm::vec3 siz;
    glm::vec3 direction;
    glm::mat4 rotation;
    float speed;
    glm::vec4 col;
    GLuint textrUn;
    GLuint textrId;
};

class BrickGameClass
{

private:
    int width = 1200;
    int height = 900;
    float gameSpeed = 300;

    gameState gameSTATE = GAME_MENU;
    bool stickToPad = true;
    int playerLifes = 3;
    int playMode = 0;

    int currLevel=1;

    GLboolean keysProc[1024];

    float ballXOnPaddle = 0.0;
    float bumpTime = 0.0;

    effect effects[100];
    std::vector<gameObj>powerUps;
    int powerUpsNr = 0;
    int effectNr=0;

    int passThroughBuff=0;
    int stickyBuff=0;

public:

    BrickGameClass(int=1200, int=900, float=200);

    ~BrickGameClass();

    void doInput(GLFWwindow*, float, float&, float);

    dirr checkDirr(glm::vec2);

    glm::vec3 checkCollision(gameObj, gameObj);

    void spawnPowerUp(glm::vec2,glm::vec2, glm::vec3);

    void updatePowerUp(RenderEngine2D*,float, gameObj&,glm::vec2&);

    void updateEffect(float, gameObj&,glm::vec2&);

    void updateLevel(RenderEngine2D*, gameLevel*, gameObj&, glm::vec2&, gameObj&, float);

    void resetLevel(gameLevel& level, float paddleX);

    void moveBall(gameObj&, glm::vec2&, float, float, float, gameLevel&, float);

    int getWidth();
    int getHeight();
    float getGameSpeed();

    gameState getGameSTATE();

    bool getStickToPad();

    int getPlayerLifes();

    int getPlayMode();

    int getCurrLevel();

    float getBallXOnPaddle();

    float getBumpTime();

    effect* getEffects();
    std::vector<gameObj> getPowerUps();
    int getPowerUpsNr();
    int getEffectNr();

    int getPassThroughBuff();
    int getStickyBuff();

};

#endif
