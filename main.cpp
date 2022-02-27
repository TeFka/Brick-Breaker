
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<SOIL2/SOIL2.h>
#include<ft2build.h>
#include FT_FREETYPE_H

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

//#include<assimp/mesh.h>
//#include<assimp/Importer.hpp>
//#include<assimp/scene.h>
//#include<assimp/postprocess.h>

#include<Shader.h>
#include<Camera.h>
#include<Texture.h>

#include<BrickGameClass.h>

#include "Inc/helpers/RootDir.h"

using namespace std;
using namespace glm;

int main()
{
    RenderEngine2D gameEngine("DaGame",
                            1200, 900,
                            4, 4);

    BrickGameClass brickGame(1200,900,300);

    gameEngine.setText("C:/Windows/Fonts/arial.ttf");
    gameEngine.particleSetup(100000);
	string rootDir = ROOT_DIR;
	string shaderDir = "Resources/shaders/";
	string shaderNames[8] = {"basic_vert.glsl", "basic_frag.glsl", "char_vert.glsl", "char_frag.glsl", "particle_vert.glsl", "particle_frag.glsl", "screens_vert.glsl", "screens_frag.glsl"};

	string textureDir = "Resources/textures/";
	string textureNames[11] = {"block.png", "block_solid.png", "background.jpg", "gboll.png", "powerup_chaos.png",
							  "powerup_increase.png", "powerup_confuse.png", "powerup_passthrough.png", "powerup_sticky.png", "powerup_speed.png", "particle.png"};

	string levelDir = "Levels/";
	string levelNames[5] = {"level1.txt", "level2.txt", "level3.txt", "level4.txt", "level5.txt"};
    //Shaders

    int basicShaderNum = gameEngine.setShader((rootDir + shaderDir + shaderNames[0]).c_str(), (rootDir + shaderDir + shaderNames[1]).c_str());
    int charShaderNum = gameEngine.setShader((rootDir + shaderDir + shaderNames[2]).c_str(),(rootDir + shaderDir + shaderNames[3]).c_str());
    int particleShaderNum = gameEngine.setShader((rootDir + shaderDir + shaderNames[4]).c_str(),(rootDir + shaderDir + shaderNames[5]).c_str());
    int screenShaderNum = gameEngine.setShader((rootDir + shaderDir + shaderNames[6]).c_str(),(rootDir + shaderDir + shaderNames[7]).c_str());

    gameEngine.setShaderUniforms(charShaderNum,screenShaderNum);

    //Textures
    gameEngine.setTexture((rootDir + textureDir + textureNames[0]).c_str(),1,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[1]).c_str(),2,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[2]).c_str(),3,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[3]).c_str(),4,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[4]).c_str(),6,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[5]).c_str(),7,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[6]).c_str(),8,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[7]).c_str(),9,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[8]).c_str(),10,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[9]).c_str(),11,1);
    gameEngine.setTexture((rootDir + textureDir + textureNames[10]).c_str(),5,2);

    //Game levelsadfa
    vector<gameLevel*> levels;
    gameLevel* lvl1 = new gameLevel((rootDir + levelDir + levelNames[0]).c_str(),brickGame.getWidth(),brickGame.getHeight(),1,2);
    gameLevel* lvl2 = new gameLevel((rootDir + levelDir + levelNames[1]).c_str(),brickGame.getWidth(),brickGame.getHeight(),1,2);
    gameLevel* lvl3 = new gameLevel((rootDir + levelDir + levelNames[2]).c_str(),brickGame.getWidth(),brickGame.getHeight(),1,2);
    gameLevel* lvl4 = new gameLevel((rootDir + levelDir + levelNames[3]).c_str(),brickGame.getWidth(),brickGame.getHeight(),1,2);
    gameLevel* lvl5 = new gameLevel((rootDir + levelDir + levelNames[4]).c_str(),brickGame.getWidth(),brickGame.getHeight(),1,2);
    levels.push_back(lvl1);
    levels.push_back(lvl2);
    levels.push_back(lvl3);
    levels.push_back(lvl4);
    levels.push_back(lvl5);

    // paddle
    float paddleW = 200.0;
    float paddleH = 20.0;
    float paddleSp = 4*brickGame.getGameSpeed();
    //ball
    float ballR = 15.0;
    vec2 ballSpeed = vec2(brickGame.getGameSpeed(),2*brickGame.getGameSpeed());

    //objects
    gameObj paddle = {100,vec2(brickGame.getWidth()/2, float(brickGame.getHeight())-paddleH-30),vec2(paddleW,paddleH),0.0,vec3(0.0,1.0,1.0),1,1};
    gameObj ball = {101,vec2(brickGame.getWidth()/2, float(brickGame.getHeight())-1.5*paddleH-30-ballR),vec2(2*ballR,2*ballR),0.0,vec3(1.0,1.0,1.0),4,1};

    //constant particles
    gameEngine.createParticleEffect(ball.pos, 1.0f, 2.0f, 2, glm::vec2(ballR,ballR), brickGame.getGameSpeed(), glm::vec2(ballR), glm::vec4(1.0,0.0,0.0,0.8),1,2,0);

    while(!glfwWindowShouldClose(gameEngine.getWindow()))
    {
        gameEngine.update();
        gameEngine.renderBegin();
        if(paddle.pos.x-paddleW/2>=0.0&&paddle.pos.x+paddleW/2<=brickGame.getWidth())
        {
            brickGame.doInput(gameEngine.getWindow(),paddleSp,paddle.pos.x,gameEngine.getDeltaTime());
        }
        if(paddle.pos.x-paddleW/2<0.0)
        {
            paddle.pos.x=paddleW/2;
        }
        if(paddle.pos.x+paddleW/2>brickGame.getWidth())
        {
            paddle.pos.x=brickGame.getWidth()-paddleW/2;
        }
        if(brickGame.getGameSTATE()!=GAME_MENU)
        {
            if(brickGame.getGameSTATE()==GAME_ACTIVE)
            {
                brickGame.moveBall(&ball,ballSpeed,paddle.pos.y-paddleH/2,paddle.pos.x,paddleW, levels[brickGame.getCurrLevel()-1],gameEngine.getDeltaTime());
                brickGame.updateEffect(gameEngine.getDeltaTime(),&paddle,ballSpeed);

                gameEngine.setSprite(vec2(brickGame.getWidth()/2,brickGame.getHeight()/2), vec2(brickGame.getWidth(),brickGame.getHeight()), vec4(1.0,0.0,1.0,0.7), 3);
                brickGame.updateLevel(&gameEngine, levels[brickGame.getCurrLevel()-1],&ball, ballSpeed, &paddle, gameEngine.getDeltaTime());
                levels[brickGame.getCurrLevel()-1]->draw(&gameEngine);
                gameEngine.setSprite(paddle.pos, paddle.siz, vec4(paddle.col,0.9), 1);
                gameEngine.setSprite(ball.pos, ball.siz, vec4(1.0,0.0,1.0,0.7), 4);

                gameEngine.updateParticleEffect(0, ball.pos);
                gameEngine.makeParticleEffect();
                brickGame.updatePowerUp(&gameEngine,gameEngine.getDeltaTime(),&paddle,ballSpeed);
                gameEngine.drawBlocks(basicShaderNum);
                gameEngine.drawParticles(particleShaderNum);
                stringstream ms;
                string lif;
                ms<<brickGame.getPlayerLifes();
                lif = ms.str();
                gameEngine.writeText(charShaderNum,"Lives: "+lif,0.9f,0.05f,1.0f,vec3(1.0));
            }
            if(brickGame.getGameSTATE()==GAME_WIN)
            {
                gameEngine.writeText(charShaderNum,"You won",0.5f,0.5f,2.0,vec3(1.0));
                gameEngine.writeText(charShaderNum,"Press space to go again or esc to quit",0.5f,0.4f,1.0f,vec3(0.0,1.0,0.0));
                if(brickGame.getCurrLevel()==5)
                {
                    for(int i =0; i<lvl1->brickCount; i++)
                    {
                        lvl1->bricks[i].exsist=1;
                    }
                }
            }
            else if(brickGame.getGameSTATE()==GAME_LOST)
            {
                gameEngine.writeText(charShaderNum,"You lost",0.5f,0.5f,2.0,vec3(1.0));
                gameEngine.writeText(charShaderNum,"Press space to go again or esc to quit",0.5f,0.4f,1.0f,vec3(1.0,0.0,0.0));
            }
        }
        else if(brickGame.getGameSTATE()==GAME_MENU)
        {
            gameEngine.setSprite(vec2(brickGame.getWidth()/2.0f,brickGame.getHeight()/2.0f), vec2((float)brickGame.getWidth(),(float)brickGame.getHeight()), vec4(1.0,1.0,0.0,1.0), 3);
            gameEngine.drawBlocks(basicShaderNum);
            gameEngine.writeText(charShaderNum,"Mrs. Bounce",0.5f,0.8f,2.0f,vec3(1.0,0.0,0.0));
            for(int i =0; i<5; i++)
            {
                stringstream ms;
                string dif;
                ms<<(i+1);
                dif = ms.str();
                if(i==(brickGame.getCurrLevel()-1))
                {
                    gameEngine.writeText(charShaderNum,"Level "+dif,0.5f,((float)(6-i)/10.0f),1.0f,vec3(0.0,1.0,1.0));
                }
                else
                {
                    gameEngine.writeText(charShaderNum,"Level "+dif,0.5f,((float)(6-i)/10.0f),1.0f,vec3(1.0));
                }
            }
        }
        gameEngine.updateScreen(screenShaderNum,brickGame.getPlayMode());

        gameEngine.renderEnd();
    }
    glfwDestroyWindow(gameEngine.getWindow());
    glfwTerminate();

    return 0;
}
