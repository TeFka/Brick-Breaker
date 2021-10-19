#include "../Inc/Texture.h"

#include "../Inc/helpers/RootDir.h"

Texture::Texture(const char* fileName,GLenum type,GLint textureUnt,std::string typ)
    {
        this->type = type;
        this->typ = typ;
        this->path = fileName;
        this->textureUnit = textureUnt;
        unsigned char* img = SOIL_load_image(fileName,&this->width,&this->height,NULL,SOIL_LOAD_RGBA);

        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        glTexParameteri(type,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(type,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(type,GL_TEXTURE_WRAP_T,GL_REPEAT);
        if(img)
        {
            glTexImage2D(type,0,GL_RGBA,this->width,this->height,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
            glGenerateMipmap(type);

        }
        else
        {
            std::cout<<"ERROR:TEXTURE:Couldnt load texture"<<fileName<<std::endl;
        }
        glActiveTexture(0);
        glBindTexture(type, 0);
        SOIL_free_image_data(img);
    }
    Texture::~Texture()
    {
        glDeleteTextures(1,&this->id);
    }
    inline GLuint Texture::getId() const
    {
        return this->id;
    }

    std::string Texture::getTyp()
    {
        return this->typ;
    }

    const char* Texture::getPath()
    {
        return this->path;
    }

    void Texture::bindT()
    {
        glActiveTexture(GL_TEXTURE0 + this->textureUnit);
        glBindTexture(this->type,this->id);
    }

    void Texture::unBindT()
    {
        glActiveTexture(0);
        glBindTexture(this->type, 0);
    }

    GLuint Texture::getUnit() const
    {
        return this->textureUnit;
    }

    void Texture::loadNewTex(const char* fileName,GLenum type)
    {
        if(this->id)
        {
            glDeleteTextures(1,&this->id);
        }
        unsigned char* img = SOIL_load_image(fileName,&this->width,&this->height,NULL,SOIL_LOAD_RGBA);

        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        glTexParameteri(type,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(type,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(type,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        if(img)
        {
            glTexImage2D(type,0,GL_RGBA,this->width,this->height,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
            glGenerateMipmap(type);

        }
        else
        {
            std::cout<<"ERROR:TEXTURE:Couldnt load texture from file"<<fileName<<std::endl;
        }
        glActiveTexture(0);
        glBindTexture(type, 0);
        SOIL_free_image_data(img);
    }
