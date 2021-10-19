#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>

#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<vector>

class Camera
{
private:
    glm::vec3 camPos;
    glm::vec3 defPos;
    glm::vec3 camFront;
    glm::vec3 camUp;
    glm::vec3 camRight;

    float yaw;
    float pitch;
    float zoom;

    glm::vec3 movementDirection;
    float movementSpeed;
    float sensetivity;

    float velocity;
public:
    Camera(glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 = glm::vec3(0.0,0.0,1.0),
           glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f),
           float = 2.5, float = 0.1, float = 45.0,
           float = 0.0, float = 90.0);

    glm::vec3 getPosition();

    void setPosition(glm::vec3);

    glm::vec3 getFront();

    void setFront(glm::vec3);

    void setUp(glm::vec3);

    void setRight(glm::vec3);
    float getZoom();
    float getSensetivity();
    glm::mat4 getViewMatrix();
    void scrollCall(float);
};

#endif
