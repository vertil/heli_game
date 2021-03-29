#pragma once
#include <cstdint>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class heli{
    uint8_t number_body_tex=0;
    uint8_t number_blades_tex=0;

    glm::mat4 moveMat=glm::mat4(1.0f);
    glm::mat4 wingsMat=glm::mat4(1.0f);
    glm::mat4 buff=glm::mat4(1.0f);

    float angle=0.0f;


    float speed=0.5f;


public:
    heli(uint8_t number_body_tex_in,uint8_t number_blades_tex_in){
        number_body_tex=number_body_tex_in;
        number_blades_tex=number_blades_tex_in;
    }
    //moving
    void move(){

        buff=glm::mat4(1.0f);

        buff=glm::translate(buff, glm::vec3(0.0f,0.01f*speed,0.0f));
        moveMat*=buff;

    }
    void rotateWing(){
        if(angle==90.0f){
            angle=0;
        }else{
            angle+=20.0f*speed;
        }

        wingsMat=moveMat;
        wingsMat=glm::rotate(wingsMat,
                                      glm::radians(angle),
                                      glm::vec3(0.0f,0.0f,1.0f));
    }
    void leftRotate(){
        buff=glm::mat4(1.0f);
        buff=glm::rotate(buff,
                             glm::radians(2.0f),
                             glm::vec3(0.0f,0.0f,1.0f));
        moveMat*=buff;
    }
    void rightRotate(){
        buff=glm::mat4(1.0f);
        buff=glm::rotate(buff,
                             glm::radians(-2.0f),
                             glm::vec3(0.0f,0.0f,1.0f));



        moveMat*=buff;
    }
    //speed
    void upSpeed(){
        speed+=0.05f;


    }
    void downSpeed(){
        speed-=0.05f;

    }
    glm::mat4 getMoveMat(){
        return moveMat;
    }
    glm::mat4 getWingMat(){
        return wingsMat;
    }
    uint8_t getBodyTexNum(){
        return number_body_tex;
    }
    uint8_t getBladesTexNum(){
        return number_blades_tex;
    }
};








