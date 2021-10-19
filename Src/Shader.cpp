#include "../Inc/Shader.h"

#include "../Inc/helpers/RootDir.h"

std::string Shader::loadShaderSource(char* fileName) const
    {
        std::string temp="";
        std::string src="";
        std::ifstream in_file;

        //Vertex
        in_file.open(fileName);

        if(in_file.is_open())
        {
            while(std::getline(in_file,temp))
            {
                src+=temp+" \n";
            }
        }
        else
        {
            std::cout<<"ERROR:SHADER:Didnt find shader file, "<<fileName<<std::endl;
        }
        in_file.close();

        return src;
    }

    GLuint Shader::loadShader(GLenum num, char* fileName) const
    {

        char infoLog[512];
        GLint success;
        GLuint shader = glCreateShader(num);
        std::string str_src = this->loadShaderSource(fileName);
        const char* src = str_src.c_str();
        glShaderSource(shader,1,&src, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout<<"ERROR:SHADER:Couldnt compile shader, "<<fileName<<std::endl;
            std::cout<<infoLog;
        }
        return shader;
    }

    void Shader::linkProgram(GLuint vertexS, GLuint fragmentS, GLuint geometryS)
    {
        char infoLog[512];
        GLint success;

        this->id = glCreateProgram();

        glAttachShader(this->id, vertexS);
        if(geometryS)
        {
            glAttachShader(this->id, geometryS);
        }
        glAttachShader(this->id, fragmentS);

        glLinkProgram(this->id);

        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            std::cout<<"ERROR:SHADER:Couldnt compile program"<<std::endl;
            std::cout<<infoLog<<"\n";
        }
        glUseProgram(0);
    }

    //contstructor
    Shader::Shader(const char* vertexF, const char* fragmentF, int useGeometryShader, const char* geometryF)
    {
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;
        GLuint geometryShader = 0;

        vertexShader = loadShader(GL_VERTEX_SHADER,(char*)vertexF);
        if(useGeometryShader != 0)
        {
            geometryShader = loadShader(GL_GEOMETRY_SHADER,(char*)geometryF);
        }
        fragmentShader = loadShader(GL_FRAGMENT_SHADER,(char*)fragmentF);

        this->linkProgram(vertexShader,fragmentShader,geometryShader);

        //END
        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(this->id);
    }
    void Shader::use() const
    {
        glUseProgram(this->id);
    }
    void Shader::unUse() const
    {
        glUseProgram(0);
    }

    void Shader::set1i(const GLint &value, const GLchar* name) const
    {
        this->use();
        glUniform1i(glGetUniformLocation(this->id,name), value);
        this->unUse();
    }
    void Shader::set1f(const GLfloat &value, const GLchar* name) const
    {
        this->use();
        glUniform1f(glGetUniformLocation(this->id,name),value);
        this->unUse();
    }
    void Shader::setVec2f(const glm::fvec2 &value, const GLchar* name) const
    {
        this->use();
        glUniform2fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));
        this->unUse();
    }
    void Shader::setVec3f(const glm::fvec3 &value, const GLchar* name) const
    {
        this->use();
        glUniform3fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));
        this->unUse();
    }
    void Shader::setVec4f(const glm::fvec4 &value, const GLchar* name) const
    {
        this->use();
        glUniform4fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));
        this->unUse();
    }
    void Shader::setMat3(const glm::mat3 &value, const GLchar* name, GLboolean trans) const
    {
        this->use();
        glUniformMatrix3fv(glGetUniformLocation(this->id,name),1,trans,glm::value_ptr(value));
        this->unUse();
    }
    void Shader::setMat4(const glm::mat4 &value, const GLchar* name, GLboolean trans) const
    {
        this->use();
        glUniformMatrix4fv(glGetUniformLocation(this->id,name),1,trans,glm::value_ptr(value));
        this->unUse();
    }
    void Shader::setUb(const GLchar* name,GLuint place)
    {
        unsigned index = glGetUniformBlockIndex(this->id,name);
        glUniformBlockBinding(this->id,index,place);
    }
