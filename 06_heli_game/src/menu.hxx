#pragma once
#include "level.hxx"
#include <filesystem>


class gameMenu{
    engine * game_eng;
    glm::mat4 selectMat=glm::mat4(1.0);

    glm::mat4 firstNum=glm::mat4(1.0);
    glm::mat4 secondNum=glm::mat4(1.0);

    SDL_Event event_log;

    char s='s';

    bool runlevel=false;

    uint8_t selectedLevel=1;
    uint8_t amount_of_levels=1;
public:
    gameMenu(engine * engine_in){
        game_eng=engine_in;
        selectMat=glm::translate(glm::mat4(1.0f),glm::vec3(-0.3f,-0.05f,0.0f));
        selectMat=glm::scale(selectMat,glm::vec3(0.5f,0.5f,0.0f));

        using std::filesystem::directory_iterator;
        amount_of_levels=std::distance(directory_iterator("levels/"),directory_iterator());
        //std::cout<<std::to_string(amount_of_levels)<<std::endl;

        //level number
        firstNum=glm::translate(glm::mat4(1.0f),glm::vec3(0.4f,0.23f,0.0f));
        secondNum=glm::translate(glm::mat4(1.0f),glm::vec3(0.6f,0.23f,0.0f));

    }
    bool menu_event(){
        while(SDL_PollEvent(&event_log)){
            if(event_log.type==SDL_KEYDOWN){
                switch (event_log.key.keysym.scancode){
                case SDL_SCANCODE_E:

                    return false;
                    break;
                case SDL_SCANCODE_UP:
                    if(s=='s'){
                        s='e';
                        selectMat=glm::translate(selectMat,glm::vec3(0.0f,-0.3f,0.0f));
                    }else{
                        s='s';
                        selectMat=glm::translate(selectMat,glm::vec3(0.0f,+0.3f,0.0f));

                    }
                    game_eng->playChunk(1);
                    return true;
                    break;
                case SDL_SCANCODE_DOWN:
                    if(s=='s'){
                        s='e';
                        selectMat=glm::translate(selectMat,glm::vec3(0.0f,-0.3f,0.0f));
                    }else{
                        s='s';
                        selectMat=glm::translate(selectMat,glm::vec3(0.0f,+0.3f,0.0f));

                    }
                    game_eng->playChunk(1);

                    return true;
                    break;
                }

            }
        }
        return true;
    }
    bool menuloop(){
        bool q=true;
        while(q){
            q=menu_event();
            game_eng->render_numbackground(14);

            game_eng->activateProgBody(16,selectMat);
            game_eng->render_body();

            game_eng->swapBuffers();

        }
        if(s=='s'){
            return true;
        }else{
            return false;
        }

    }
    bool levelselect_event(){
        while(SDL_PollEvent(&event_log)){
            if(event_log.type==SDL_KEYDOWN){
                switch (event_log.key.keysym.scancode) {
                case SDL_SCANCODE_UP:
                    if(selectedLevel==amount_of_levels){
                        selectedLevel=1;
                    }else{
                        selectedLevel++;
                    }
                    game_eng->playChunk(1);
                    //std::cout<<std::to_string(selectedLevel)<<std::endl;

                    return true;
                    break;
                case SDL_SCANCODE_DOWN:
                    if(selectedLevel==1){
                        selectedLevel=amount_of_levels;
                    }else{
                        selectedLevel--;
                    }
                    game_eng->playChunk(1);

                    //std::cout<<std::to_string(selectedLevel)<<std::endl;
                    return true;
                    break;
                case SDL_SCANCODE_E:
                    runlevel=true;
                    return false;
                    break;
                case SDL_SCANCODE_ESCAPE:
                    runlevel=false;
                    return false;
                    break;

                }
            }
        }
        return true;
    }
    bool selectLevelLoop(){
        bool q=true;
        while(q){
            q=levelselect_event();

            //render background
            game_eng->render_numbackground(15);
            //render level number
            if(selectedLevel<10){
                game_eng->load_texture("materials/numbers/0.png",17);
                game_eng->load_texture("materials/numbers/"+
                                       std::to_string(selectedLevel)
                                       +".png",18);

            }else{
                game_eng->load_texture("materials/numbers/"+
                                       std::to_string(selectedLevel/10)
                                       +".png",17);



                game_eng->load_texture("materials/numbers/"+
                                       std::to_string(selectedLevel%10)
                                       +".png",18);
            }
            //first
            game_eng->activateProgBody(17,firstNum);
            game_eng->render_body();
            //second
            game_eng->activateProgBody(18,secondNum);
            game_eng->render_body();


            game_eng->swapBuffers();
        }
        return runlevel;
    }
    bool getRunlevelBool(){
        return runlevel;
    }
    u_int8_t getSelectedLevel(){
        return selectedLevel;
    }
};
