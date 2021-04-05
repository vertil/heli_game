#pragma once
#include "engine.hxx"


class enemy{
    uint8_t texture_number=4;

    glm::mat4 positionMat=glm::mat4(1.0f);
    glm::mat4 rotationMat=glm::mat4(1.0f);

    glm::mat4 buff=glm::mat4(1.0f);


    float angle=0.0f;

    float middle_x=0.0f;
    float middle_y=0.0f;

    float fire_Distance=0.4f;
    float appear_Distance=0.4f;

public:
    enemy(float x_in, float y_in,float appear_in, float fire_in){
        fire_Distance=fire_in;
        appear_Distance=appear_in;
        middle_x=x_in;
        middle_y=y_in;

        positionMat=glm::translate(positionMat,glm::vec3(x_in,y_in,0.0f));
        positionMat=glm::scale(positionMat,glm::vec3(0.75f,0.75f,0.0f));
    }
    float getFireDistance(){
        return fire_Distance;
    }
    float getAppearDistance(){
        return appear_Distance;
    }
    glm::mat4 getMoveMat(){
        return positionMat*rotationMat;
    }
    float getX(){
        return middle_x;
    }
    float getY(){
        return middle_y;
    }
    float getAngle(){
        return angle;
    }
    void aimRotate(glm::mat4 target,float distanse){



        if(target[3][0]>positionMat[3][0] && target[3][1]>positionMat[3][1]){
            float a=abs(target[3][1]-positionMat[3][1]);

            angle=asin(a/distanse);

        }else if (target[3][0]>positionMat[3][0] && target[3][1]<positionMat[3][1]){
            float a=abs(target[3][1]-positionMat[3][1]);

            angle=(asin(a/distanse))*(-1);

        }else if(target[3][0]<positionMat[3][0] && target[3][1]<positionMat[3][1]){
            float a=abs(target[3][1]-positionMat[3][1]);

            angle=(M_PI+asin(a/distanse));

        }else if(target[3][0]<positionMat[3][0] && target[3][1]>positionMat[3][1]){
            float a=abs(target[3][1]-positionMat[3][1]);

            angle=(M_PI+asin(a/distanse))*(-1);

        }


        rotationMat=glm::mat4(1.0f);
        rotationMat=glm::rotate(glm::mat4(1.0f),
                                angle,
                                glm::vec3(0.0f,0.0f,1.0f)
                                );

    }
};
