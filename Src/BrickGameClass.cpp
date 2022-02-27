
#include "../Inc/brickGameClass.h"

BrickGameClass::BrickGameClass(int gWidth, int gHeight, float gSpeed)
{

    this->width = gWidth;
    this->height = gHeight;
    this->gameSpeed = gSpeed;

}

BrickGameClass::~BrickGameClass()
{

}

void BrickGameClass::doInput(GLFWwindow* window, float sp, float& daX, float deltaTime)
{
    if(this->gameSTATE==GAME_ACTIVE)
    {
        if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
        {
            daX-=(sp*deltaTime);
        }
        else if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
        {
            daX+=(sp*deltaTime);

        }
        if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
        {
            this->stickToPad=false;
        }
    }
    else if(this->gameSTATE==GAME_MENU)
    {
        if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS && !this->keysProc[GLFW_KEY_W])
        {
            if(this->currLevel>1)
            {
                this->currLevel--;
            }
            else
            {
                this->currLevel=5;
            }
            this->keysProc[GLFW_KEY_W]=GL_TRUE;
        }
        else if(glfwGetKey(window,GLFW_KEY_W)==GLFW_RELEASE)
        {
            this->keysProc[GLFW_KEY_W]=GL_FALSE;
        }
        if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS && !this->keysProc[GLFW_KEY_S])
        {
            if(this->currLevel<5)
            {
                this->currLevel++;
            }
            else
            {
                this->currLevel=1;
            }
            this->keysProc[GLFW_KEY_S]=GL_TRUE;
        }
        else if(glfwGetKey(window,GLFW_KEY_S)==GLFW_RELEASE)
        {
            this->keysProc[GLFW_KEY_S]=GL_FALSE;
        }
        if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)
        {
            this->gameSTATE=GAME_ACTIVE;
        }
    }
    else if(this->gameSTATE==GAME_WIN)
    {
        if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)
        {
            this->gameSTATE=GAME_ACTIVE;
            this->currLevel = 1;
        }
    }
    else if(this->gameSTATE==GAME_LOST)
    {
        if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)
        {
            this->gameSTATE=GAME_ACTIVE;
        }
    }
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
    {
        glfwWindowShouldClose(window);
        glfwDestroyWindow(window);
    }
}

dirr BrickGameClass::checkDirr(glm::vec2& target)
{
    glm::vec2 sides[4] =
    {
        glm::vec2(0.0,1.0),
        glm::vec2(1.0,0.0),
        glm::vec2(0.0,-1.0),
        glm::vec2(-1.0,0.0),
    };
    float maxVal = 0.0;
    int bestVal = -1;
    for(int i = 0; i<4; i++)
    {
        float product = dot(normalize(target),sides[i]);
        if(product>maxVal)
        {
            maxVal = product;
            bestVal=i;
        }

        return (dirr)bestVal;
    }
}

glm::vec3 BrickGameClass::checkCollision(gameObj* obj1, gameObj* obj2)
{
    glm::vec2 difference = obj1->pos-obj2->pos;
    glm::vec2 clamped = clamp(difference,glm::vec2(-obj2->siz.x/2,-obj2->siz.y/2),glm::vec2(obj2->siz.x/2,obj2->siz.y/2));
    glm::vec2 closest = obj2->pos+clamped;
    difference = closest-obj1->pos;
    if(length(difference)<obj1->siz.x/2)
    {
        return glm::vec3(difference,1.0);
    }
    else
    {
        return glm::vec3(difference,0.0);
    }
}

void BrickGameClass::spawnPowerUp(glm::vec2 pos,glm::vec2 siz, glm::vec3 col)
{
    GLuint random = rand()%100;
    if(random<12)
    {
        int powerText = 0;
        int powerNum = 0;
        if(random<2)
        {
            powerText=6;
            powerNum = 1;
        }
        else if(random<4)
        {
            powerText=8;
            powerNum = 2;
        }
        else if(random<6)
        {
            powerText=9;
            powerNum = 3;
        }
        else if(random<8)
        {
            powerText=10;
            powerNum = 4;
        }
        else if(random<10)
        {
            powerText=5;
            powerNum = 5;
        }
        else if(random<12)
        {
            powerText=7;
            powerNum = 6;
        }
        this->powerUps.push_back(new gameObj{powerNum, pos, glm::vec2(siz.x,siz.y*0.5), 0.0, (col+0.3f), powerText, 1});
    }
}

void BrickGameClass::updatePowerUp(RenderEngine2D* engine,float deltaT, gameObj* paddle,glm::vec2& velocity)
{
    for(int i =0; i<this->powerUps.size(); i++)
    {
        if(this->powerUps[i]->exsist==1)
        {
            engine->setSprite(this->powerUps[i]->pos,this->powerUps[i]->siz,glm::vec4(this->powerUps[i]->col,1.0),this->powerUps[i]->textNum);
            this->powerUps[i]->pos.y+=gameSpeed*deltaT;
            if(this->powerUps[i]->pos.y-this->powerUps[i]->siz.y>height)
            {
                this->powerUps[i]->exsist=0;
            }
            if(checkCollision(paddle,this->powerUps[i]).z==1.0)
            {
                this->effects.push_back(new effect{powerUps[i]->num, 10, 1});

                if(this->effects[this->effects.size()-1]->effectNum==1)
                {
                    paddle->siz.x*=1.5;
                }
                else if(this->effects[this->effects.size()-1]->effectNum==2)
                {
                    this->passThroughBuff = 1;
                }
                else if(this->effects[this->effects.size()-1]->effectNum==3)
                {
                    this->stickyBuff = 1;
                }
                else if(this->effects[this->effects.size()-1]->effectNum==4)
                {
                    velocity.x=velocity.x*1.5;
                    velocity.y=velocity.y*1.5;
                }
                else if(this->effects[this->effects.size()-1]->effectNum==5)
                {
                    this->playMode=2;
                }
                else if(this->effects[this->effects.size()-1]->effectNum==6)
                {
                    this->playMode=3;
                }
                this->powerUps[i]->exsist=0;
            }
        }
    }
    for(int i =(this->powerUps.size()-1); i>=0; i--)
    {
        if(this->powerUps[i]->exsist==0)
        {
            this->powerUps.erase(this->powerUps.begin()+i);
        }
    }
}

void BrickGameClass::updateEffect(float deltaT, gameObj* paddle,glm::vec2& velocity)
{
    int lastMode[5] = {0,0,0,0,0};
    float lastModeLifeTime[5]= {-1.0,-1.0,-1.0,-1.0,-1.0};
    for(int i =0; i<this->effects.size(); i++)
    {
        if(this->effects[i]->exsist==1)
        {
            this->effects[i]->lifetime-=deltaT;
            if(this->effects[i]->effectNum<=4)
            {
                if(lastModeLifeTime[this->effects[i]->effectNum-1]<this->effects[i]->lifetime)
                {
                    lastMode[this->effects[i]->effectNum-1]=i;
                    lastModeLifeTime[this->effects[i]->effectNum-1]=this->effects[i]->lifetime;
                }
            }
            if(this->effects[i]->effectNum==5||this->effects[i]->effectNum==6)
            {
                if(this->effects[i]->lifetime>lastModeLifeTime[4])
                {
                    lastMode[4]=i;
                    lastModeLifeTime[4]=this->effects[i]->lifetime;
                }
            }
            if(this->effects[i]->lifetime<=0.0)
            {
                this->effects[i]->exsist=0;
                if(this->effects[i]->effectNum==1&&lastMode[0]==i)
                {
                    paddle->siz.x/=1.5;
                }
                else if(this->effects[i]->effectNum==2&&lastMode[1]==i)
                {
                    this->passThroughBuff=0;
                }
                else if(this->effects[i]->effectNum==3&&lastMode[2]==i)
                {
                    this->stickyBuff=0;
                }
                else if(this->effects[i]->effectNum==4&&lastMode[3]==i)
                {
                    velocity.x=velocity.x/1.5;
                    velocity.y=velocity.y/1.5;
                }
                else if(this->effects[i]->effectNum==5&&lastMode[4]==i)
                {
                    playMode=0;
                }
                else if(this->effects[i]->effectNum==6&&lastMode[4]==i)
                {
                    playMode=0;
                }
            }

        }
    }
    for(int i =(this->effects.size()-1); i>=0; i--)
    {
        if(this->effects[i]->exsist==0)
        {
            this->effects.erase(this->effects.begin()+i);
        }
    }
}

void BrickGameClass::updateLevel(RenderEngine2D* engine, gameLevel* level, gameObj* ball, glm::vec2& velocity, gameObj* padd, float deltaTime)
{
    int allDone = 1;
    for(int n = 0; n<level->bricks.size(); n++)
    {
        if(level->bricks[n].textNum==1)
        {
            allDone=0;
        }
        if(playMode==1)
        {
            this->bumpTime+=deltaTime;
        }
        if(this->bumpTime>30.0)
        {
            playMode=0;
            this->bumpTime=0.0;
        }
        if(level->bricks[n].exsist == 1)
        {
            glm::vec3 collision = checkCollision(ball,&level->bricks[n]);
            if(collision.z==1.0)
            {
                if(level->bricks[n].textNum==1)
                {
                    if(playMode==0)
                    {
                        playMode = 1;
                    }
                    level->bricks[n].exsist = 0;
                    spawnPowerUp(level->bricks[n].pos,level->bricks[n].siz, level->bricks[n].col);
                    engine->createParticleEffect(level->bricks[n].pos, 0.2f, 1.0f,500,glm::vec2(5.0),gameSpeed/2,
                                                 glm::vec2(level->bricks[n].siz.x/2,level->bricks[n].siz.y/2),glm::vec4(level->bricks[n].col,0.8),16,1,0);
                }
                if(this->passThroughBuff==0||level->bricks[n].textNum==1)
                {
                    glm::vec2 theVec = glm::vec2(collision.x,collision.y);
                    dirr dirrection = checkDirr(theVec);
                    if(dirrection==LEFT||dirrection==RIGHT)
                    {
                        velocity.x=-velocity.x;
                        float penetration = ball->siz.x/2-abs(collision.x);
                        if(dirrection==LEFT)
                        {
                            ball->pos.x+=penetration;
                        }
                        else
                        {
                            ball->pos.x-=penetration;
                        }
                    }
                    else
                    {
                        velocity.y=-velocity.y;
                        float penetration = ball->siz.x/2-abs(collision.y);
                        if(dirrection==UP)
                        {
                            ball->pos.y-=penetration;
                        }
                        else
                        {
                            ball->pos.y+=penetration;
                        }
                    }
                }
            }
        }
    }

    if(allDone==1)
    {
        if(this->currLevel==5)
        {
            this->gameSTATE=GAME_WIN;
        }
        else
        {
            this->currLevel++;
        }
        this->stickToPad=true;
        playMode = 0;
        velocity.x=0.0;
        velocity.y=0.0;
        ball->pos.x=width/2;
        //ball->pos.y=padd.pos.y+padd.siz.y+ball->siz.x;
        padd->pos.x = width/2;
        this->playerLifes=3;
    }
}

void BrickGameClass::resetLevel(gameLevel* level, float paddleX)
{
    for(int i = 0; i<level->brickCount; i++)
    {
        level->bricks[i].exsist=1;
    }
    paddleX = width/2;
    this->playerLifes=3;
}

void BrickGameClass::moveBall(gameObj* ball, glm::vec2& velocity, float paddleTop, float paddleX, float paddleW, gameLevel* currActiveLevel, float deltaTime)
{
    int sticked=0;
    if(this->stickToPad==false)
    {
        ball->pos.x+=velocity.x*deltaTime;
        ball->pos.y+=velocity.y*deltaTime;
        if(ball->pos.x+ball->siz.x/2>width)
        {
            velocity.x=-velocity.x;
            ball->pos.x=width-ball->siz.x/2;
        }
        if(ball->pos.x-ball->siz.x/2<0.0)
        {
            velocity.x=-velocity.x;
            ball->pos.x=ball->siz.x/2;
        }
        if(ball->pos.y-ball->siz.x/2<0.0)
        {
            velocity.y=-velocity.y;
            ball->pos.y=ball->siz.x/2;
        }
        if(ball->pos.y+ball->siz.x/2>paddleTop&&ball->pos.x>paddleX-paddleW/2&& ball->pos.x<paddleX+paddleW/2)
        {
            if(this->stickyBuff==1)
            {
                sticked=1;
                this->ballXOnPaddle = ball->pos.x-paddleX;
            }
            else
            {
                ball->pos.y=paddleTop-ball->siz.x/2;
                float dist = ball->pos.x-paddleX;
                float percent = dist/(paddleW/2);
                glm::vec2 oldVelocity = velocity;

                if(percent<=0.2&&percent>=-0.2)
                {
                    velocity.x=-velocity.x;
                }
                else
                {
                    velocity.x += (gameSpeed*percent);
                    if(velocity.x>gameSpeed)
                    {
                        velocity.x=gameSpeed;
                    }
                }
                velocity.y=-velocity.y;
            }
        }
        if(ball->pos.y+ball->siz.x>height||sticked==1)
        {
            velocity.y=-velocity.y;
            ball->pos.y = paddleTop-ball->siz.x;
            if(sticked==0)
            {
                this->playerLifes--;
                this->ballXOnPaddle = 0.0;
            }
            if(this->playerLifes<=0)
            {
                resetLevel(currActiveLevel,paddleX);
                this->gameSTATE=GAME_LOST;
                playMode = 0;
            }
            this->stickToPad = true;
        }
    }
    else
    {
        velocity.x = 0.0;
        ball->pos.x = paddleX+this->ballXOnPaddle;
        ball->pos.y = paddleTop-ball->siz.x/2;
    }
}

int BrickGameClass::getWidth()
{

    return this->width;

}

int BrickGameClass::getHeight()
{

    return this->height;

}

float BrickGameClass::getGameSpeed()
{

    return this->gameSpeed;
}

gameState BrickGameClass::getGameSTATE()
{

    return this->gameSTATE;

}

bool BrickGameClass::getStickToPad()
{

    return this->stickToPad;

}

int BrickGameClass::getPlayerLifes()
{

    return this->playerLifes;

}

int BrickGameClass::getPlayMode()
{

    return this->playMode;

}

int BrickGameClass::getCurrLevel()
{

    return this->currLevel;

}

float BrickGameClass::getBallXOnPaddle()
{

    return this->ballXOnPaddle;

}

float BrickGameClass::getBumpTime()
{

    return this->bumpTime;

}

std::vector<effect*> BrickGameClass::getEffects()
{

    return this->effects;

}
std::vector<gameObj*> BrickGameClass::getPowerUps()
{

    return this->powerUps;

}
int BrickGameClass::getPowerUpsNr()
{

    return this->powerUpsNr;

}
int BrickGameClass::getEffectNr()
{

    return this->effects.size();

}

int BrickGameClass::getPassThroughBuff()
{

    return this->passThroughBuff;

}
int BrickGameClass::getStickyBuff()
{

    return this->stickyBuff;

}
