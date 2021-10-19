#include "../Inc/gameLevel.h"

 gameLevel::gameLevel(const GLchar *file,GLuint wid,GLuint heig, int textNum1, int textNum2){
       this->brickTextr = textNum1;
       this->solidTextr = textNum2;
       GLuint tileCode;
       std::string line;
       std::ifstream fstream(file);
       std::vector<std::vector<GLuint>> tileData;
       if(fstream){
          while(getline(fstream,line)){
            std::stringstream sstream(line);
            std::vector<GLuint> row;
            while(sstream >> tileCode){
                row.push_back(tileCode);
            }
            tileData.push_back(row);
          }
          if(tileData.size()>0){
            this->init(tileData,wid,heig);
          }
       }
    }

    gameLevel::~gameLevel(){

    }

    void gameLevel::draw(RenderEngine2D* engine){
       for(int n = 0;n<brickCount;n++){
          if(bricks[n].exsist==1){
            engine->setSprite(bricks[n].pos,bricks[n].siz,glm::vec4(bricks[n].col,0.8),bricks[n].textNum);
          }
       }
    }

    void gameLevel::init(std::vector<std::vector<GLuint>> tileData,GLuint lvlWid,GLuint lvlHeig){
         GLuint wid = tileData[0].size();
         GLuint heig = tileData.size();
         GLfloat unitWid = lvlWid/float(wid);
         GLfloat unitHeig = lvlHeig*0.4/float(heig);
         for(int x=0;x<wid;x++){
            for(int y=0;y<heig;y++){
               if(tileData[y][x]==1){
                  glm::vec2 pos((unitWid/2)+unitWid*x,(unitHeig/2)+unitHeig*y);
                  glm::vec2 siz(unitWid,unitHeig);
                  gameObj obj = {tileData[y][x],pos,siz,0.0,glm::vec3(0.3,0.3,0.3),solidTextr,1};
                  this->bricks.push_back(obj);
                  brickCount++;
               }
               else if(tileData[y][x]>1){
                    glm::vec3 col = glm::vec3(1.0);
                 if(tileData[y][x]==2){
                    col=glm::vec3(0.0,1.0,0.0);
                 }
                 else if(tileData[y][x]==3){
                    col=glm::vec3(0.0,0.0,0.5);
                 }
                 else if(tileData[y][x]==4){
                    col=glm::vec3(9.0,0.4,0.0);
                 }
                 else if(tileData[y][x]==5){
                    col=glm::vec3(0.4,0.0,0.0);
                 }
                   glm::vec2 pos((unitWid/2)+unitWid*x,(unitHeig/2)+unitHeig*y);
                   glm::vec2 siz(unitWid,unitHeig);
                   gameObj obj = {tileData[y][x],pos,siz,0.0,col,brickTextr,1};
                   this->bricks.push_back(obj);
                   brickCount++;
               }
            }
         }
    }
