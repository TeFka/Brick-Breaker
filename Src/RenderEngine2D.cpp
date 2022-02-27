#include "../Inc/RenderEngine2D.h"

#include "../Inc/helpers/RootDir.h"

glm::vec2 testVertices[] =
{
    glm::vec2(0.0f, 0.0f),// left
    glm::vec2(0.0f, 900.0f),// right
    glm::vec2(1200.0f,  0.0f)// top
};


glm::vec4 vertices2D[6] =
{
    glm::vec4(-1.0,-1.0,0.0,0.0),
    glm::vec4(1.0,-1.0,1.0,0.0),
    glm::vec4(-1.0,1.0,0.0,1.0),
    glm::vec4(-1.0,1.0,0.0,1.0),
    glm::vec4(1.0,-1.0,1.0,0.0),
    glm::vec4(1.0,1.0,1.0,1.0)
};

int RenderEngine2D::firstUnusedParticle(int type, int index)
{
    if(type==1)
    {
        for(int m = lastUsedParticle; m<particlesNr; m++)
        {
            if(this->temporaryParticleEffects[index]->particles[m].life<=0.0)
            {
                lastUsedParticle=m;
                return m;
            }
        }
        for(int m =0; m<lastUsedParticle; m++)
        {
            if(this->temporaryParticleEffects[index]->particles[m].life<=0.0)
            {
                lastUsedParticle=m;
                return m;
            }
        }
    }
    else if(type==2)
    {
        for(int m = lastUsedParticle; m<particlesNr; m++)
        {
            if(this->constantParticleEffects[index]->particles[m].life<=0.0)
            {
                lastUsedParticle=m;
                return m;
            }
        }
        for(int m =0; m<lastUsedParticle; m++)
        {
            if(this->constantParticleEffects[index]->particles[m].life<=0.0)
            {
                lastUsedParticle=m;
                return m;
            }
        }
    }
    lastUsedParticle=0;
    return 0;

}

void  RenderEngine2D::respawnParticle(particle& partic,float partcLife, glm::vec2 pos, float velocity, glm::vec2 offSet, glm::vec4 col)
{
    float randomX = (1-(2*((float)(rand()%100)/100)));
    float randomY = (1-(2*((float)(rand()%100)/100)));
    partic.pos.x = pos.x+(randomX*offSet.x);
    partic.pos.y = pos.y+(randomY*offSet.y);
    partic.color = col;
    partic.life = partcLife;
    partic.velocity.x = randomX*velocity;
    partic.velocity.y = randomY*velocity;
}

std::map<char, Character>  RenderEngine2D::setTextType(const char* filePath)
{
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        std::cout<<"No freetype"<<std::endl;
    }
    FT_Face fc;
    if(FT_New_Face(ft, filePath,0,&fc))
    {
        std::cout<<"No freetype face"<<std::endl;
    }
    FT_Set_Pixel_Sizes(fc,0,48);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    for(int m = 0; m<128; m++)
    {
        if(FT_Load_Char(fc,m,FT_LOAD_RENDER))
        {
            std::cout<<"Error: No glyph"<<std::endl;
        }
        GLuint textch;
        glGenTextures(1,&textch);
        glBindTexture(GL_TEXTURE_2D,textch);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RED,
                     fc->glyph->bitmap.width,fc->glyph->bitmap.rows,
                     0,GL_RED,GL_UNSIGNED_BYTE,fc->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        Character ch =
        {
            textch,
            glm::vec2(fc->glyph->bitmap.width,fc->glyph->bitmap.rows),
            glm::vec2(fc->glyph->bitmap_left,fc->glyph->bitmap_top),
            fc->glyph->advance.x
        };
        this->characters.insert(std::pair<char,Character>(m,ch));
    }

    FT_Done_Face(fc);
    FT_Done_FreeType(ft);

    return characters;
}

void  RenderEngine2D::frameBufferRez(GLFWwindow *window, int fbW, int fbH)
{
    glViewport(0,0, fbW, fbH);
}

void  RenderEngine2D::setGLFW(const char* windwName, const int width, const int height, const int majorVersion, const int minorVersion)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    this->windax = glfwCreateWindow(width, height, windwName, NULL, NULL);

    glfwGetFramebufferSize(windax, &framebufferWidth, &framebufferHeight);
    glfwSetFramebufferSizeCallback(this->windax, frameBufferRez);

    glfwMakeContextCurrent(this->windax); //Important!!!!

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetInputMode(this->windax, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(this->windax, mouseInput);
    //glfwSetScrollCallback(this->windax, scrollInput);
}

void  RenderEngine2D::enableModes()
{
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void  RenderEngine2D::setDefaultShaders()
{
    //this->allShaders.push_back(new Shader("Resources/shaders/test_vert.glsl","Resources/shaders/test_frag.glsl"));
}

void  RenderEngine2D::setVAOs()
{
    glGenVertexArrays(1, &theVAO);
    glGenBuffers(1, &theVBO);

    glGenVertexArrays(1,&thePartcVAO);
    glGenBuffers(1,&thePartcVBO);

    glGenVertexArrays(1, &screenVAO);
    glBindVertexArray(screenVAO);
    glGenBuffers(1, &screenVBO);

    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2D), vertices2D, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1,&textVAO);
    glGenBuffers(1,&textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER,textVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*6*4,NULL,GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glGenVertexArrays(1, &this->testVAO);
    glGenBuffers(1, &this->testVBO);
    glBindVertexArray(this->testVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->testVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*this->testVertices.size(), this->testVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void  RenderEngine2D::setScreen()
{
    //rbo
    glGenRenderbuffers(1,&this->RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,this->windowWidth,this->windowHeight);
    glBindRenderbuffer(GL_RENDERBUFFER,0);

    //FBOs
    glGenFramebuffers(1,&this->basicFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->basicFBO);
    //texture
    glGenTextures(1, &this->screenTexture);
    glBindTexture(GL_TEXTURE_2D,this->screenTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,this->windowWidth,this->windowHeight,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this->screenTexture,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,this->RBO);
    unsigned int attachment = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &attachment);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout<<"ERROR:FRAMEBUFFER1:Framebuffer is not complete!"<<std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    unsigned int mRBO;
    glGenRenderbuffers(1,&this->mRBO);
    glBindRenderbuffer(GL_RENDERBUFFER,this->mRBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_DEPTH24_STENCIL8,this->windowWidth,this->windowHeight);
    glBindRenderbuffer(GL_RENDERBUFFER,this->mRBO);
    unsigned int mFBO;
    glGenFramebuffers(1,&this->mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER,this->mFBO);
    unsigned int mFBOtext;
    glGenTextures(1,&this->mFBOtext);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,this->mFBOtext);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4,GL_RGB,this->windowWidth,this->windowHeight,GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,this->mFBOtext,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,this->mRBO);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: multisample Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void  RenderEngine2D::updateBlockVAO()
{

    glBindVertexArray(this->theVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->theVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(basicVertex)*this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)offsetof(basicVertex, textr));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,4, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)offsetof(basicVertex, col));
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(basicVertex), (void*)offsetof(basicVertex, textNr));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
}
void  RenderEngine2D::updateParticleVAO()
{
    glBindVertexArray(this->thePartcVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->thePartcVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(basicVertex)*this->particleVertices.size(), this->particleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)offsetof(basicVertex, textr));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,4, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)offsetof(basicVertex, col));
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(basicVertex), (void*)offsetof(basicVertex, textNr));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
}

void  RenderEngine2D::updateUniforms()
{

    glBindBuffer(GL_UNIFORM_BUFFER,this->matrixUbo);
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(this->projectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void  RenderEngine2D::updateTime()
{
    this->currentTime = glfwGetTime();
    this->deltaTime = (currentTime - lastTime);
    this->lastTime = currentTime;

    if(this->deltaTime>1.0)
    {
        this->deltaTime=1.0;
    }
    /*if(this->deltaTime<0.01)
    {
        this->deltaTime=0.1;
    }*/
}

void  RenderEngine2D::updateViewport()
{
    glfwGetFramebufferSize(this->windax,&this->framebufferWidth,&this->framebufferHeight);
    if(this->windowWidth != this->framebufferWidth||this->windowHeight != this->framebufferHeight)
    {
        this->windowWidth = this->framebufferWidth;
        this->windowHeight = this->framebufferHeight;
        //this->projectionMatrix = glm::ortho(0.0f, float(this->windowWidth),float(this->windowHeight),0.0f,-1.0f,1.0f);
        glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,this->windowWidth,this->windowHeight);
        glBindRenderbuffer(GL_RENDERBUFFER,0);

        glBindTexture(GL_TEXTURE_2D,this->screenTexture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,this->windowWidth,this->windowHeight,0,GL_RGBA,GL_FLOAT,NULL);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindRenderbuffer(GL_RENDERBUFFER,this->mRBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_DEPTH24_STENCIL8,this->windowWidth,this->windowHeight);
        glBindRenderbuffer(GL_RENDERBUFFER,this->mRBO);

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,this->mFBOtext);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4,GL_RGB,this->windowWidth,this->windowHeight,GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
    }
}

RenderEngine2D::RenderEngine2D(const char* title,
                               const int windowW, const int windowH,
                               const int glMajor, const int glMinor)
{
    this->testVertices.push_back(glm::vec2(0.0f, 0.0f));
    this->testVertices.push_back(glm::vec2(0.0f, 900.0f));
    this->testVertices.push_back(glm::vec2(1200.0f, 0.0f));

    this->windowWidth = windowW;
    this->windowHeight = windowH;
    this->GLFWminorVersion = glMinor;
    this->GLFWminorVersion = glMajor;
    this->setGLFW(title, windowW, windowH, glMajor, glMinor);
    this->enableModes();
    this->setDefaultShaders();
    this->setVAOs();
    this->setScreen();
}
RenderEngine2D::~RenderEngine2D()
{

    glfwDestroyWindow(this->windax);
    glfwTerminate();
}

int  RenderEngine2D::setShader(const char* vertexFile, const char* fragmentFile, int useGeometryShader, const char* geometryFile)
{

    this->allShaders.push_back(new Shader(vertexFile,fragmentFile, useGeometryShader, geometryFile));
    return (this->allShaders.size()-1);
}

void  RenderEngine2D::setShaderUniforms(int charShaderNum,int screenShaderNum)
{
    for(i=0; i<this->allShaders.size(); i++)
    {
        this->allShaders[i]->setUb("matrixes",0);
    }
    glGenBuffers(1,&this->matrixUbo);
    glBindBuffer(GL_UNIFORM_BUFFER,this->matrixUbo);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),NULL,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferRange(GL_UNIFORM_BUFFER,0,this->matrixUbo,0,sizeof(glm::mat4));

    if(screenShaderNum!=0)
    {
        GLfloat offset = 1.0/300.0;
        GLfloat offsets[9][2]=
        {
            {-offset,offset},
            {0.0,offset},
            {offset,offset},
            {0.0,0.0},
            {offset,0.0},
            {offset,-offset},
            {0.0,-offset},
            {-offset,-offset},
            {-offset,0.0},
        };
        float edgeKernels[9]=
        {
            1,2,1,
            2,4,2,
            1,2,1
        };
        for(int i=0; i<9; i++)
        {
            std::stringstream ms;
            std::string daI;
            ms<<i;
            daI = ms.str();
            this->allShaders[screenShaderNum]->set1i(edgeKernels[i],("edgeKern["+daI+"]").c_str());
            this->allShaders[screenShaderNum]->set1f(edgeKernels[i]/16,("blurKern["+daI+"]").c_str());
        }
        this->allShaders[screenShaderNum]->use();
        glUniform2fv(glGetUniformLocation(this->allShaders[screenShaderNum]->id, "offsets"), 9, (GLfloat*)offsets);
        this->allShaders[screenShaderNum]->set1i(0,"textr");
    }
    this->projectionMatrix = glm::ortho(0.0f, float(this->windowWidth),float(this->windowHeight),0.0f,-1.0f,1.0f);

    this->allShaders[charShaderNum]->set1i(0,"textur");


}

void  RenderEngine2D::setTexture(const char* file, int place, int type)
{
    if(type==1)
    {
        this->blockTextures.push_back(new Texture(file,GL_TEXTURE_2D,place));
        this->blockTexturesAmount++;
    }
    else if(type==2)
    {
        this->particleTextures.push_back(new Texture(file,GL_TEXTURE_2D,place));
        this->particleTexturesAmount++;
    }
}

void  RenderEngine2D::setLights(glm::vec3 pos)
{
    this->lightPositions.push_back(pos);
}

void  RenderEngine2D::setText(const char* file)
{
    this->characters = setTextType(file);
}

void  RenderEngine2D::particleSetup(int particleStorageSize)
{
    this->particlesNr = particleStorageSize;
}

float  RenderEngine2D::getDeltaTime()
{
    return this->deltaTime;
}
GLFWwindow*  RenderEngine2D::getWindow()
{
    return this->windax;
}

void  RenderEngine2D::writeText(int shaderIndex, std::string text, GLfloat wPart, GLfloat hPart,GLfloat scale,glm::vec3 color)
{
    GLfloat x = float(this->windowWidth)*wPart;
    GLfloat y = float(this->windowHeight)*hPart;
    glEnable(GL_BLEND);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glm::mat4 proj = glm::ortho(0.0f, float(this->windowWidth), 0.0f, float(this->windowHeight),-1.0f,1.0f);
    this->allShaders[shaderIndex]->setMat4(proj,"projectionMatrix");
    this->allShaders[shaderIndex]->setVec3f(color,"textCol");
    this->allShaders[shaderIndex]->use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->textVAO);
    std::string::const_iterator c;
    GLfloat sum = 0;
    for(c=text.begin(); c!=text.end(); c++)
    {
        Character ch = this->characters[*c];
        sum+=(ch.advance >> 6) * scale;
    }
    x=x-sum/2;
    for(c=text.begin(); c!=text.end(); c++)
    {
        Character ch = this->characters[*c];
        GLfloat xPos = x+ch.bear.x*scale;
        GLfloat yPos = y-((ch.siz.y-ch.bear.y)*scale);
        GLfloat chW = ch.siz.x*scale;
        GLfloat chH = ch.siz.y*scale;
        GLfloat chVertices[6][4]=
        {
            {xPos,yPos+chH,0.0,0.0},
            {xPos,yPos,0.0,1.0},
            {xPos+chW,yPos,1.0,1.0},
            {xPos,yPos+chH,0.0,0.0},
            {xPos+chW,yPos,1.0,1.0},
            {xPos+chW,yPos+chH,1.0,0.0}
        };
        glBindTexture(GL_TEXTURE_2D,ch.text);
        glBindBuffer(GL_ARRAY_BUFFER,this->textVBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(chVertices),chVertices);
        glDrawArrays(GL_TRIANGLES,0,6);
        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_BLEND);
}

void  RenderEngine2D::setSprite(glm::vec2 pos, glm::vec2 siz, glm::vec4 color, int textN)
{
    this->vertices.push_back(basicVertex{glm::vec2(pos.x-siz.x/2,pos.y-siz.y/2),glm::vec2(0.0,1.0),color,textN-1});
    this->vertices.push_back(basicVertex{glm::vec2(pos.x-siz.x/2,pos.y+siz.y/2),glm::vec2(0.0,0.0),color,textN-1});
    this->vertices.push_back(basicVertex{glm::vec2(pos.x+siz.x/2,pos.y+siz.y/2),glm::vec2(1.0,0.0),color,textN-1});
    this->vertices.push_back(basicVertex{glm::vec2(pos.x-siz.x/2,pos.y-siz.y/2),glm::vec2(0.0,1.0),color,textN-1});
    this->vertices.push_back(basicVertex{glm::vec2(pos.x+siz.x/2,pos.y+siz.y/2),glm::vec2(1.0,0.0),color,textN-1});
    this->vertices.push_back(basicVertex{glm::vec2(pos.x+siz.x/2,pos.y-siz.y/2),glm::vec2(1.0,1.0),color,textN-1});
}
void  RenderEngine2D::setTransparentSprite(glm::vec2 pos, glm::vec2 siz, glm::vec4 color, int textN)
{
    this->transparentVertices.push_back(basicVertex{glm::vec2(pos.x-siz.x/2,pos.y-siz.y/2),glm::vec2(0.0,1.0),color,textN-1});
    this->transparentVertices.push_back(basicVertex{glm::vec2(pos.x-siz.x/2,pos.y+siz.y/2),glm::vec2(0.0,0.0),color,textN-1});
    this->transparentVertices.push_back(basicVertex{glm::vec2(pos.x+siz.x/2,pos.y+siz.y/2),glm::vec2(1.0,0.0),color,textN-1});
    this->transparentVertices.push_back(basicVertex{glm::vec2(pos.x-siz.x/2,pos.y-siz.y/2),glm::vec2(0.0,1.0),color,textN-1});
    this->transparentVertices.push_back(basicVertex{glm::vec2(pos.x+siz.x/2,pos.y+siz.y/2),glm::vec2(1.0,0.0),color,textN-1});
    this->transparentVertices.push_back(basicVertex{glm::vec2(pos.x+siz.x/2,pos.y-siz.y/2),glm::vec2(1.0,1.0),color,textN-1});
}

void  RenderEngine2D::createParticleEffect(glm::vec2 pos, float lifeTime, float partcLife, int particleGenerationRate, glm::vec2 siz, float speed, glm::vec2 offset,glm::vec4 color, int textNum, int state,int id)
{
    std::vector<particle> partcs;
    for(i=0; i<this->particlesNr; i++)
    {
        partcs.push_back(particle());
    }
    if(state==1)
    {
        this->temporaryParticleEffects.push_back(new particleEffect{id,lifeTime,partcLife,particleGenerationRate,pos,offset,siz,speed,color,textNum,partcs});
    }
    else if(state==2)
    {
        this->constantParticleEffects.push_back(new particleEffect{id,lifeTime,partcLife,particleGenerationRate,pos,offset,siz,speed,color,textNum,partcs});
    }
}

void  RenderEngine2D::updateParticleEffect(int id, glm::vec2 newPos)
{
    this->constantParticleEffects[id]->pos = newPos;
}

void  RenderEngine2D::makeParticleEffect()
{
    float randomX;
    float randomY;
    //temporary particles
    for(int p = 0; p<this->temporaryParticleEffects.size(); p++)
    {
        this->temporaryParticleEffects[p]->life-=this->deltaTime;
        if(this->temporaryParticleEffects[p]->life>0)
        {
            for (GLuint i = 0; i < this->temporaryParticleEffects[p]->particleGenerationRate; ++i)
            {
                int unusedParticle = firstUnusedParticle(1,p);
                respawnParticle(this->temporaryParticleEffects[p]->particles[unusedParticle],this->temporaryParticleEffects[p]->partcLife,
                                this->temporaryParticleEffects[p]->pos,this->temporaryParticleEffects[p]->velocity,
                                this->temporaryParticleEffects[p]->offset, this->temporaryParticleEffects[p]->color);
            }
        }
        int zeroParticles = 1;
        for(i = (this->temporaryParticleEffects[p]->particles.size()-1); i>=0; i--)
        {
            randomX = (1-(2*((float)(rand()%100)/100)));
            randomY = (1-(2*((float)(rand()%100)/100)));
            particle &tempP = this->temporaryParticleEffects[p]->particles[i];
            tempP.life -= this->deltaTime;
            // reduce life
            if (tempP.life > 0.0f)
            {
                // particle is alive, thus update
                tempP.pos += tempP.velocity * this->deltaTime;
                tempP.color.a -= (this->deltaTime/this->temporaryParticleEffects[p]->partcLife);
            }
            if(tempP.life>0.0)
            {
                zeroParticles = 0;

                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x-this->temporaryParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y-this->temporaryParticleEffects[p]->siz.y*randomY/2),glm::vec2(0.0,1.0),
                                                 tempP.color,this->temporaryParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x-this->temporaryParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y+this->temporaryParticleEffects[p]->siz.y*randomY/2),glm::vec2(0.0,0.0),
                                                 tempP.color,this->temporaryParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x+this->temporaryParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y+this->temporaryParticleEffects[p]->siz.y*randomY/2),glm::vec2(1.0,0.0),
                                                 tempP.color,this->temporaryParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x-this->temporaryParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y-this->temporaryParticleEffects[p]->siz.y*randomY/2),glm::vec2(0.0,1.0),
                                                 tempP.color,this->temporaryParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x+this->temporaryParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y+this->temporaryParticleEffects[p]->siz.y*randomY/2),glm::vec2(1.0,0.0),
                                                 tempP.color,this->temporaryParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x+this->temporaryParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y-this->temporaryParticleEffects[p]->siz.y*randomY/2),glm::vec2(1.0,1.0),
                                                 tempP.color,this->temporaryParticleEffects[p]->textNum-1});
            }
        }
        if(zeroParticles)
        {
            this->temporaryParticleEffects.erase(this->temporaryParticleEffects.begin()+p);
        }
    }

    //constant particles
    for(int p = 0; p<this->constantParticleEffects.size(); p++)
    {
        for (GLuint i = 0; i < this->constantParticleEffects[p]->particleGenerationRate; ++i)
        {
            int unusedParticle = firstUnusedParticle(2,p);
            respawnParticle(this->constantParticleEffects[p]->particles[unusedParticle],this->constantParticleEffects[p]->partcLife,
                            this->constantParticleEffects[p]->pos,this->constantParticleEffects[p]->velocity,
                            this->constantParticleEffects[p]->offset, this->constantParticleEffects[p]->color);
        }
        for(i = 0; i<this->constantParticleEffects[p]->particles.size(); i++)
        {
            randomX = (1-(2*((float)(rand()%100)/100)));
            randomY = (1-(2*((float)(rand()%100)/100)));
            particle &tempP = this->constantParticleEffects[p]->particles[i];
            tempP.life -= deltaTime;
            // reduce life
            if (tempP.life > 0.0f)
            {
                // particle is alive, thus update
                tempP.pos += tempP.velocity * this->deltaTime;
                tempP.color.a -= (this->deltaTime/this->constantParticleEffects[p]->partcLife);
            }
            if(tempP.life>0.0)
            {
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x-this->constantParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y-this->constantParticleEffects[p]->siz.y*randomY/2),glm::vec2(0.0,1.0),
                                                 tempP.color,this->constantParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x-this->constantParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y+this->constantParticleEffects[p]->siz.y*randomY/2),glm::vec2(0.0,0.0),
                                                 tempP.color,this->constantParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x+this->constantParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y+this->constantParticleEffects[p]->siz.y*randomY/2),glm::vec2(1.0,0.0),
                                                 tempP.color,this->constantParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x-this->constantParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y-this->constantParticleEffects[p]->siz.y*randomY/2),glm::vec2(0.0,1.0),
                                                 tempP.color,this->constantParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x+this->constantParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y+this->constantParticleEffects[p]->siz.y*randomY/2),glm::vec2(1.0,0.0),
                                                 tempP.color,this->constantParticleEffects[p]->textNum-1});
                this->particleVertices.push_back(basicVertex{glm::vec2(tempP.pos.x+this->constantParticleEffects[p]->siz.x*randomX/2,
                                                 tempP.pos.y-this->constantParticleEffects[p]->siz.y*randomY/2),glm::vec2(1.0,1.0),
                                                 tempP.color,this->constantParticleEffects[p]->textNum-1});
            }
        }
    }
}

void  RenderEngine2D::clearVerts()
{
    this->vertices.clear();
    this->transparentVertices.clear();
    this->particleVertices.clear();
}

void  RenderEngine2D::update()
{
    this->updateViewport();
    this->updateTime();
    this->updateUniforms();
    this->clearVerts();
}

void  RenderEngine2D::renderBegin()
{
    glClearColor(0.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,this->windowWidth,this->windowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER,this->mFBO);
}

void  RenderEngine2D::drawTest()
{
    for(int b = 0; b<3; b++)
    {
        std::cout<<testVertices[b].x<<"  "<<testVertices[b].y<<std::endl;
        std::cout<<(this->projectionMatrix*glm::vec4(testVertices[b],0.0,1.0)).x<<"  "<<(projectionMatrix*glm::vec4(testVertices[b],0.0,1.0)).y<<std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glClearColor(0.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(this->testVAO);
    this->allShaders[0]->use();
    glDrawArrays(GL_TRIANGLES,0,3);

    glfwSwapBuffers(this->windax);
    glFlush();
    glfwPollEvents();
    glBindVertexArray(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(0);
}

void  RenderEngine2D::drawBlocks(int shaderIndex)
{
    this->updateBlockVAO();
    //blocks
    for(i = 0; i<this->blockTextures.size(); i++)
    {
        std::stringstream ms;
        std::string mIndex;
        ms<<i;
        mIndex=ms.str();
        this->allShaders[shaderIndex]->set1i(this->blockTextures[i]->getUnit(),("textr["+mIndex+"]").c_str());
        this->blockTextures[i]->bindT();
    }
    glBindVertexArray(this->theVAO);
    this->allShaders[shaderIndex]->use();
    glDrawArrays(GL_TRIANGLES,0,this->vertices.size());
}

void  RenderEngine2D::drawParticles(int shaderIndex)
{

    this->updateParticleVAO();
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    for(i = 0; i<this->particleTextures.size(); i++)
    {
        std::stringstream ms;
        std::string mIndex;
        ms<<i;
        mIndex=ms.str();
        this->allShaders[shaderIndex]->set1i(this->particleTextures[i]->getUnit(),("textr["+mIndex+"]").c_str());
        this->particleTextures[i]->bindT();
    }
    glBindVertexArray(this->thePartcVAO);
    this->allShaders[shaderIndex]->use();
    glDrawArrays(GL_TRIANGLES,0,this->particleVertices.size());
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void  RenderEngine2D::drawTransparent(int shaderIndex)
{
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBindVertexArray(this->trnVAO);
    this->allShaders[shaderIndex]->use();
    glDrawArrays(GL_TRIANGLES,0,this->transparentVertices.size());
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
void  RenderEngine2D::updateScreen(int shaderIndex, int mode)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER,mFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,basicFBO);
    glBlitFramebuffer(0,0,this->windowWidth,this->windowHeight,0,0,this->windowWidth,this->windowHeight,GL_COLOR_BUFFER_BIT,GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,this->windowWidth,this->windowHeight);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(this->screenVAO);
    this->allShaders[shaderIndex]->set1i(mode,"mode");
    this->allShaders[shaderIndex]->set1f(this->deltaTime,"time");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,this->screenTexture);
    this->allShaders[shaderIndex]->use();
    glDrawArrays(GL_TRIANGLES,0,6);
}

void  RenderEngine2D::renderEnd()
{
    glfwSwapBuffers(this->windax);
    glFlush();
    glfwPollEvents();
    glBindVertexArray(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(0);
}
