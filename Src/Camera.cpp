
#include "../Inc/Camera.h"

Camera::Camera(glm::vec3 position,
           glm::vec3 camFront,
           glm::vec3 up,
           float movementSpeedx, float sensetivityx, float zoomx,
           float yawx, float pitchx)
    {
        camPos = position;
        defPos = position;
        camUp = up;
        yaw = yawx;
        pitch = pitchx;
        movementSpeed=movementSpeedx;
        sensetivity=sensetivityx;
        zoom=zoomx;
    }

    glm::vec3 Camera::getPosition()
    {
        return this->camPos;
    }

    void Camera::setPosition(glm::vec3 pos)
    {
        this->camPos=pos;
    }

    glm::vec3 Camera::getFront()
    {
        return this->camFront;
    }

    void Camera::setFront(glm::vec3 fron)
    {
        this->camFront=fron;
    }

    void Camera::setUp(glm::vec3 up)
    {
        this->camUp=up;
    }

    void Camera::setRight(glm::vec3 right)
    {
        this->camRight=right;
    }

    float Camera::getZoom()
    {
        return this->zoom;
    }

    float Camera::getSensetivity()
    {
        return sensetivity;
    }
    glm::mat4 Camera::getViewMatrix()
    {
        glm::mat4 lok=glm::mat4(1.0);
        glm::mat4 mult=glm::mat4(1.0);

        glm::vec3 zaxis = -glm::normalize(camFront);
        glm::vec3 xaxis = glm::normalize(glm::cross(camUp,zaxis));
        glm::vec3 yaxis = glm::cross(zaxis,xaxis);

        lok[0][0]=xaxis.x;
        lok[1][0]=xaxis.y;
        lok[2][0]=xaxis.z;
        lok[0][1]=yaxis.x;
        lok[1][1]=yaxis.y;
        lok[2][1]=yaxis.z;
        lok[0][2]=zaxis.x;
        lok[1][2]=zaxis.y;
        lok[2][2]=zaxis.z;

        mult[3][0]=-camPos.x;
        mult[3][1]=-camPos.y;
        mult[3][2]=-camPos.z;

        lok = lok*mult;

        return lok;
        //glm::lookAt(camPos,camPos+camFront,camUp);
    }

    void Camera::scrollCall(float ySet)
    {
        if(zoom>=0.1 && zoom<=45.0)
        {
            zoom-=ySet;
        }
        if(zoom<0.1)
        {
            zoom=0.1;
        }
        if(zoom>45.0)
        {
            zoom=45.0;
        }
    }
