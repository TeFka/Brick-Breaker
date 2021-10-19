
#ifndef GAMELEVEL_H_INCLUDED
#define GAMELEVEL_H_INCLUDED

#include<RenderEngine2D.h>

struct gameObj
{
    int num;
    glm::vec2 pos;
    glm::vec2 siz;
    float rotat;
    glm::vec3 col;
    int textNum;
    int exsist;
};

class gameLevel{
 public:
    int brickCount = 0;
    int allDone = 0;
    std::vector<gameObj> bricks;
    int brickTextr;
    int solidTextr;

    int completed;

    gameLevel(const GLchar*,GLuint,GLuint, int, int);

    ~gameLevel();

    void draw(RenderEngine2D* engine);

 private:

    void init(std::vector<std::vector<GLuint>>,GLuint,GLuint);
};

#endif
