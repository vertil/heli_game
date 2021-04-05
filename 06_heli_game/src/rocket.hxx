#pragma once
#include "engine.hxx"

class rocket{
    u_int8_t texture_number=5;
    float speed =0.2f;
    glm::mat4 positionMat=glm::mat4(1.0f);
    glm::mat4 buff;
public:
    rocket(glm::mat4 mat_in,float speed_in){

        positionMat=mat_in;
        speed=speed_in;

        buff=glm::mat4(1.0f);
        buff=glm::rotate(buff,
                             glm::radians(-90.0f),
                             glm::vec3(0.0f,0.0f,1.0f));

        positionMat*=buff;



        positionMat=glm::scale(positionMat,glm::vec3(0.75f,0.75f,0.0f));

    }
    void move(){
        buff=glm::mat4(1.0f);

        buff=glm::translate(buff, glm::vec3(0.0f,0.01f*speed,0.0f));
        positionMat*=buff;
    }
    glm::mat4 getMat(){
        return positionMat;
    }

};
