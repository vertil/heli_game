#pragma once
#include "engine.hxx"

class heal{
    uint8_t texture_number=10;

    glm::mat4 positionMat=glm::mat4(1.0f);

    float middle_x=0.0f;
    float middle_y=0.0f;

public:
    heal(float x_in,float y_in){
        middle_x=x_in;
        middle_y=y_in;

        positionMat=glm::translate(positionMat,glm::vec3(x_in,y_in,0.0f));
        positionMat=glm::scale(positionMat,glm::vec3(0.45f,0.45f,0.0f));
    }
    u_int8_t getTexture_number(){
        return texture_number;
    }
    glm::mat4 getMat(){
        return positionMat;
    }
};
