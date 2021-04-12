#pragma once
#include "engine.hxx"

class level{
    std::string_view map_name;
    engine * game_eng;
    uint8_t number_of_enemies;
    uint8_t number_of_targets;
    uint8_t number_of_heals;

    uint8_t frame=1;

    //sdl event
    SDL_Event event_log;
    //helicopter data and animation
    uint8_t hp=5;
    heli * helicopter;
    animation* blow;
    bool is_forward=false;
    char rotation='n';
    bool cargo_open=false;
    bool cargo_full=false;
    bool cantload=false;
    uint8_t cargo=99;

    //objects
    std::vector<enemy> enemies;
    std::vector<rocket> rockets;
    std::vector<target> targets;
    std::vector<heal> heals;
    //base
    float baseX=0.0f;
    float baseY=0.0f;
    float baseAppearDis=0.5f;
    uint8_t base_Texture=11;
    glm::mat4 baseMat=glm::mat4(1.0f);


public:
    level(std::string_view map_in,engine * engine_in){
        map_name=map_in;
        game_eng=engine_in;

        if (!game_eng->load_texture(map_name,0)){
            std::cout<<"cant load "<<map_name<<std::endl;
        }

        //init helicopter and blow animation
        blow=new animation(1,5,4);
        helicopter=new heli(2,3);
        blow->setPlace(game_eng->gettriangle(1),game_eng->gettriangle(2));
        //
        enemies.push_back(enemy(-0.5f,-0.5f,0.5f,0.5f));

        targets.push_back(target(0.5f,0.5f,0.5f,6));

        heals.push_back(heal(0.7f,-0.5f));

    }
    level(uint8_t levelNumber, engine * engine_in){

        game_eng=engine_in;
        blow=new animation(1,5,4);
        helicopter=new heli(2,3);
        blow->setPlace(game_eng->gettriangle(1),game_eng->gettriangle(2));

        std::ifstream file;
        file.open("levels/level"+std::to_string(levelNumber)+".txt");
        if(!file){
            cantload==true;
            std::cout<<"can't read level"+levelNumber<<std::endl;

        }
        //
        std::string b;
        float x;
        float y;
        float fire_distance;
        float appear_distance;
        int zzz=0;
        uint8_t texture;
        //map
        file >> b;

        engine_in->load_texture("materials/"+b,0);
        //helicopter
        file >> x >> y >> b;
        helicopter->set_start(x,y);
        //hp
        file>>zzz>>b;
        hp=zzz;

        //enemies
        file >> zzz>>b;
        number_of_enemies=zzz;
        enemies.clear();
        enemies.reserve(number_of_enemies);
        for(int i=0;i<number_of_enemies;i++){
            file >>x>>y>>fire_distance>>appear_distance;
            enemies.push_back(enemy(x,y,appear_distance,fire_distance));

        }
        //targets
        file>>zzz>>b;
        number_of_targets=zzz;
        targets.clear();
        targets.reserve(number_of_targets);
        for(int i=0;i<number_of_targets;i++){
            file>> zzz>>x>>y>>appear_distance;
            //std::cout<<x<<" "<<y<<std::endl;
            targets.push_back(target(x,y,appear_distance,zzz));
        }

        //heals
        file>>zzz>>b;
        number_of_heals=zzz;
        heals.clear();
        heals.reserve(number_of_heals);
        for(int i=0;i<number_of_heals;i++){
            file>>x>>y>>appear_distance;
            heals.push_back(heal(x,y));
            //std::cout<<x<<" "<<y<<" "<<appear_distance<<std::endl;

        }

        //base
        file>>baseX>>baseY>>baseAppearDis>>b;

        baseMat=glm::translate(baseMat,glm::vec3(baseX,baseY,0.0f));
        file.close();


    }
    bool level_event(){
         while(SDL_PollEvent(&event_log)){
             if(event_log.type==SDL_QUIT){
                 return false;
             }else if(event_log.type==SDL_KEYDOWN){
                 switch (event_log.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                     is_forward=true;
                     break;
                    case SDL_SCANCODE_A:
                     rotation='l';
                     break;
                    case SDL_SCANCODE_D:
                     rotation='r';
                     break;
                    case SDL_SCANCODE_E:
                     if(cargo_open==true){
                         cargo_open=false;
                         //std::cout<<"close"<<std::endl;
                         game_eng->playChunk(2);
                     }else{
                         cargo_open=true;
                         //std::cout<<"open"<<std::endl;
                         game_eng->playChunk(3);

                     }

                     break;
                 }
             }else if(event_log.type==SDL_KEYUP){
                 switch (event_log.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                     is_forward=false;
                     break;
                    case SDL_SCANCODE_A:
                     rotation='n';
                     break;
                    case SDL_SCANCODE_D:
                     rotation='n';
                     break;
                    case SDL_SCANCODE_G:

                     blow->start();
                     break;

                 }
             }else if(event_log.type==SDL_MOUSEWHEEL){
                 if(event_log.wheel.y>0){
                     helicopter->upSpeed();

                     return true;
                 }
                 if(event_log.wheel.y<0){
                     helicopter->downSpeed();

                     return true;
                 }
                 return true;
             }
         }
         return true;

    }
    void move_Player(){
        if(is_forward){
            helicopter->move();
        }
        switch (rotation) {
            case 'l':
                helicopter->leftRotate();
                break;
            case 'r':
                helicopter->rightRotate();
                break;
        }
        helicopter->rotateWing();
    }
    float getDistance(enemy& en_in){
        //get position from mat4 - x=mat4[3][0] y=mat4[3][1]
        float a=helicopter->getMoveMat()[3][0]-en_in.getX();
        float b=helicopter->getMoveMat()[3][1]-en_in.getY();
        return sqrt(pow(a,2)+pow(b,2));
    }
    float getDistanceRocket(rocket& rock_in){
        float a=helicopter->getMoveMat()[3][0]-rock_in.getMat()[3][0];
        float b=helicopter->getMoveMat()[3][1]-rock_in.getMat()[3][1];

        return sqrt(pow(a,2)+pow(b,2));
    }
    float getDistanceTarget(target& targ_in){
        float a=helicopter->getMoveMat()[3][0]-targ_in.getMat()[3][0];
        float b=helicopter->getMoveMat()[3][1]-targ_in.getMat()[3][1];
        return sqrt(pow(a,2)+pow(b,2));
    }
    float getDistanceTargetBase(target& targ_in){
        float a=baseMat[3][0]-targ_in.getMat()[3][0];
        float b=baseMat[3][1]-targ_in.getMat()[3][1];
        return sqrt(pow(a,2)+pow(b,2));
    }
    float getDistanceHeal(heal& heal_in){
        float a=helicopter->getMoveMat()[3][0]-heal_in.getMat()[3][0];
        float b=helicopter->getMoveMat()[3][1]-heal_in.getMat()[3][1];
        //std::cout<<std::to_string(sqrt(pow(a,2)+pow(b,2)))<<std::endl;
        return sqrt(pow(a,2)+pow(b,2));
    }
    float getDistanceBase(){
        float a=helicopter->getMoveMat()[3][0]-baseX;
        float b=helicopter->getMoveMat()[3][1]-baseY;
        //std::cout<<std::to_string(sqrt(pow(a,2)+pow(b,2)))<<std::endl;
        return sqrt(pow(a,2)+pow(b,2));
    }
    bool checkPenetration(rocket& rock){
        if(rock.getMat()==helicopter->getMoveMat()){
            return true;
        }else{
            return false;
        }
    }
    void  logic(){
        //player moves
        move_Player();

        //enemies aim
        for(int i=0;i<enemies.size();i++){
            enemies[i].aimRotate(helicopter->getMoveMat(),getDistance(enemies[i]));
        }
        //enemies shooting
        if(frame%10==0){
            for(int i=0;i<enemies.size();i++){
                if(enemies[i].getFireDistance()>getDistance(enemies[i])){
                    rockets.push_back(rocket(enemies[i].getMoveMat(),0.5f));
                    game_eng->playChunk(0);
                }
            }

        }

        //rockets move
        for(int i=0;i<rockets.size();i++){
            rockets[i].move();
        }
        //rockets deleting
        for(auto it=rockets.begin();it!=rockets.end();){
            if(it->getMat()[3][0]>1.2f ||
               it->getMat()[3][0]<-1.2f ||
               it->getMat()[3][1]>1.2f ||
               it->getMat()[3][0]<-1.2f){
                it=rockets.erase(it);
            }else{
                ++it;
            }
        }
        //check heal
        for(auto it=heals.begin();it!=heals.end();){
            if(getDistanceHeal(*it)<0.04f){
                hp++;
                it=heals.erase(it);
            }else{
                ++it;
            }
        }

        //check
        for(auto it=rockets.begin();it!=rockets.end();){
            if(getDistanceRocket(*it)<0.05f){
                game_eng->playChunk(4);

               blow->start();
               hp--;
               it=rockets.erase(it);

            }else{
                it++;
            }
         }

        //check cargo
        if(cargo_full==false){

            for(auto it=targets.begin();it!=targets.end();){
               if(cargo_open==true){
                   if(getDistanceTarget(*it)<0.03f){
                       cargo=std::distance(targets.begin(),it);
                       //std::cout<<"bdish"<<std::endl;
                       cargo_full=true;
                       cargo_open=false;
                       break;
                   }
                   else{
                       it++;
                   }
               }else{
                   it++;
               }
            }
        }
        if(cargo_full==true&&cargo_open==true&&cargo!=88){
            targets[cargo].setPos(helicopter->getMoveMat());
            cargo_full=false;
            cargo_open=false;
            if(getDistanceTargetBase(targets[cargo])<0.07f){
                targets.erase(targets.begin()+cargo);

            }
            cargo=88;
        }






    }
    bool level_loop(){
        if(cantload==true){
            return false;
        }

        bool q=true;

        while(q==true){
            //loop start

            /*if(targets.size()==0 || hp==0){
                q=false;
                break;
            }*/

            if(targets.size()==0){
                q=false;
                break;
            }
            if(hp==0){
                q=false;
                break;
            }



            q=level_event();
            logic();
            //render map
            game_eng->render_background();

            //render heal
            for(auto i:heals){
                game_eng->activateProgBody(i.getTexture_number(),i.getMat());
                game_eng->render_body();

            }

            //render enemies
            for(auto i:enemies){
                if(getDistance(i)<=i.getAppearDistance()){
                    game_eng->activateProgBody(4,i.getMoveMat());
                    game_eng->render_body();
                }

            }

            //render rockets
            for(auto i:rockets){
                game_eng->activateProgBody(5,i.getMat());
                game_eng->render_body();
            }

            //render base
            if(getDistanceBase()<=baseAppearDis){
                game_eng->activateProgBody(base_Texture,baseMat);
                game_eng->render_body();
            }






            //targets render
            for(auto it=targets.begin();it!=targets.end();it++){
                if(it==targets.begin()+cargo){
                    game_eng->activateProgBody(it->getTexture(),helicopter->getMoveMat());
                    game_eng->render_body();

                }else{
                    if(getDistanceTarget(*it)<=it->getAppearDistance()){
                        game_eng->activateProgBody(it->getTexture(),it->getMat());
                        game_eng->render_body();
                    }


                }

            }



            //helicopter render
            game_eng->activateProgBody(2,helicopter->getMoveMat());
            game_eng->render_body();
            game_eng->activateProgBody(3,helicopter->getWingMat());
            game_eng->render_body();

            //render blow
            if(blow->isactive()){

                game_eng->activateProgBody(1,helicopter->getMoveMat());

                game_eng->render_sprite(blow->getframe());
            }

            game_eng->render_hp(hp,cargo_open);




            //----don't touch
            game_eng->swapBuffers();
            if(frame==600){
                frame=1;
            }else{
                frame++;
            }

            //loop end
        }
        return false;
    }


};
