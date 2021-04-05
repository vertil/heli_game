#include "level.hxx"

int main(int, char **){

    engine eng;


    //initialization of opengl programs
    std::string error = eng.initProgramBackground();//init opengl programs
    if (!error.empty())
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    error = eng.initProgramBody();//init opengl programs
    if (!error.empty())
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    //loading textures
    if (!eng.load_texture("materials/map1.png",0)){
        std::cout<<"failed to load chernarus2.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/bip.png",1)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/body.png",2)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/blades.png",3)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/enemy.png",4)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/rocket.png",5)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/artillery.png",6)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/apc.png",7)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/tank.png",8)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/boat.png",9)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/heal.png",10)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/base.png",11)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/hp.png",12)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }
    if (!eng.load_texture("materials/open.png",13)){
        std::cout<<"failed to load bip.png"<<std::endl;
        return EXIT_FAILURE;
    }


    eng.initData();
    eng.initfTriagBackground();


    level level_1("materials/map2.png",&eng);

    /*if(level_1.level_loop()==false){
        std::cout<<"konec"<<std::endl;
    }*/

    level level_2(1,&eng);
    if(level_2.level_loop()==false){
        std::cout<<"konec"<<std::endl;
    }




    //old version
    /*bool q=true;
    while(q){
        q=eng.event();
        eng.logic();
        eng.render();
        eng.swapBuffers();
    }*/
  
}
