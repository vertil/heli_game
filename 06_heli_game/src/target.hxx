#pragma once
#include "engine.hxx"

class target{
    uint8_t texture_number=8;

    glm::mat4 positionMat=glm::mat4(1.0f);

    glm::mat4 buff=glm::mat4(1.0f);

    float middle_x=0.0f;
    float middle_y=0.0f;

    float appear_Distance=0.4f;



public:
    target(float x_in,float y_in,float app_Dis_in, uint8_t texture_in){
        middle_x=x_in;
        middle_y=y_in;

        texture_number=texture_in;

        positionMat=glm::translate(positionMat,glm::vec3(x_in,y_in,0.0f));
    }
    float getX(){
        return middle_x;
    }
    float getY(){
        return middle_y;
    }
    uint8_t getTexture(){
        return texture_number;
    }
    void picket(float x_in, float y_in){

    }
    void setPos(glm::mat4 mat_in){
        positionMat=mat_in;
    }
    glm::mat4 getMat(){
        return positionMat;
    }
    float getAppearDistance(){
        return appear_Distance;
    }
};
