#include "engine.hxx"

int main(int, char **){

    engine eng;

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

    if (!eng.load_texture("materials/chernarus2.png",0)){
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

    eng.initData();
    eng.initfTriagBackground();
    bool q=true;
    while(q){
        q=eng.event();
        eng.logic();
        eng.render();
        eng.swapBuffers();
    }
  
}
