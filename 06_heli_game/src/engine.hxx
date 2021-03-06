#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <array>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <thread>
#include <SDL2/SDL_mixer.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "structs.hxx"
#include "animation.hxx"
#include "heli.hxx"
#include "enemy.hxx"
#include "rocket.hxx"
#include "target.hxx"
#include "heal.hxx"
#include "lodepng.h"


#include "glad/glad.h"


static void APIENTRY callback_opengl_debug(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, [[maybe_unused]] const void* userParam);

static void audio_callback(void* userdata,uint8_t* stream, int len);

class engine{
    //openGL programs
    GLuint        program_id_simple = 0;
    GLuint        program_id_body = 0;
    //textures
    GLuint tex_handl [2];
    //--players
    float side=0.0f;
    float speed=0.5f;
    glm::mat4 bodyMove=glm::mat4(1.0f);
    glm::mat4 buffMat=glm::mat4(1.0f);

    //--MAP
    triangle background1;
    triangle background2;

    triangle body1;
    triangle body2;

    //----
    bool is_forward=false;
    char rotation='n';
    //----SDL
    SDL_Window *window;
    SDL_Event event_log;
    SDL_Renderer *rend;
    //---hp
    triangle hp1;
    triangle hp2;
    //audio
    std::vector<Mix_Chunk *> chunks;
    Mix_Music *gMusic=NULL;
public:
    engine(){
        std::stringstream serr;
        using namespace std;


        const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
        if (init_result != 0)
        {
            const char* err_message = SDL_GetError();
             serr << "error: failed call SDL_Init: " << err_message << std::endl;
            //return serr.str();
        }

        /*if (0 != SDL_Init(SDL_INIT_EVERYTHING))
        {
            std::cerr << SDL_GetError() << std::endl;
            //return EXIT_FAILURE;
        }*/
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        window=SDL_CreateWindow("Loop",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  1000,
                                  1000,
                                  SDL_WINDOW_OPENGL
                                  );

           if(window==NULL){
           std::cout<<"Couldn't create window"<<SDL_GetError()<<std::endl;
               SDL_DestroyWindow(window);
           }

           rend=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

           int gl_major_ver       = 3;
           int gl_minor_ver       = 2;
           int gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

           const char* platform_from_sdl = SDL_GetPlatform();
           std::string_view platform{platform_from_sdl};
           using namespace std::string_view_literals;
           using namespace std;
           auto list = { "Windows"sv, "Mac OS X"sv };
           auto it   = find(begin(list), end(list), platform);
           if (it != end(list))
           {
               gl_major_ver       = 4;
               gl_minor_ver       = (platform == "Mac OS X") ? 1 : 3;
               gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;
           }

           SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);
           SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
           SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

           SDL_GLContext gl_context = SDL_GL_CreateContext(window);
           if (gl_context == nullptr)
           {
               gl_major_ver       = 3;
               gl_minor_ver       = 2;
               gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

               SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);
               SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
               SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);
               gl_context = SDL_GL_CreateContext(window);
           }
           assert(gl_context != nullptr);

           int result =
               SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
           assert(result == 0);
           result =
               SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
           assert(result == 0);

           if (gl_major_ver < 3)
           {
               std::clog << "current context opengl version: " << gl_major_ver
                         << '.' << gl_minor_ver << '\n'
                         << "need openg ES version at least: 3.2\n"
                         << std::flush;
               throw std::runtime_error("opengl version too low");
           }

           std::clog << "OpenGl "<< gl_major_ver << '.'<< gl_minor_ver<<'\n';

           if (gladLoadGLES2Loader(SDL_GL_GetProcAddress) == 0)
           {
               std::clog << "error: failed to initialize glad" << std::endl;
           }

           if (platform != "Mac OS X") // not supported on Mac
           {
               glEnable(GL_DEBUG_OUTPUT);
               glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
               glDebugMessageCallback(callback_opengl_debug, nullptr);
               glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                                 nullptr, GL_TRUE);
           }

           std::cout<<"sas"<<std::endl;

           glGenTextures(20, tex_handl); //texture descriptor
           OM_GL_CHECK()

    }
    void initData(){
        //blow=new animation(1,5,4);
        //helicopter=new heli(2,3);

        body1.v[0].x=-0.1;
        body1.v[0].y=-0.1;
        body1.v[0].tex_x=0.0;
        body1.v[0].tex_y=1.0;
                //2
                body1.v[1].x=-0.1;
                body1.v[1].y=0.1;
                body1.v[1].tex_x=0.0;
                body1.v[1].tex_y=0.0;
                //3
                body1.v[2].x=0.1;
                body1.v[2].y=-0.1;
                body1.v[2].tex_x=1.0;
                body1.v[2].tex_y=1.0;
                //
                body2.v[0].x=-0.1;
                    body2.v[0].y=0.1;
                    body2.v[0].tex_x=0.0;
                    body2.v[0].tex_y=0.0;
                    //2
               body2.v[1].x=0.1;
               body2.v[1].y=0.1;
               body2.v[1].tex_x=1.0;
               body2.v[1].tex_y=0.0;
                    //3
               body2.v[2].x=0.1;
               body2.v[2].y=-0.1;
               body2.v[2].tex_x=1.0;
               body2.v[2].tex_y=1.0;

        //blow->setPlace(body1,body2);
        //--------hp

               hp1.v[0].x=-0.03;
               hp1.v[0].y=-0.03;
               hp1.v[0].tex_x=0.0;
               hp1.v[0].tex_y=1.0;
                       //2
                       hp1.v[1].x=-0.03;
                       hp1.v[1].y=0.03;
                       hp1.v[1].tex_x=0.0;
                       hp1.v[1].tex_y=0.0;
                       //3
                       hp1.v[2].x=0.03;
                       hp1.v[2].y=-0.03;
                       hp1.v[2].tex_x=1.0;
                       hp1.v[2].tex_y=1.0;
                       //
                       hp2.v[0].x=-0.03;
                           hp2.v[0].y=0.03;
                           hp2.v[0].tex_x=0.0;
                           hp2.v[0].tex_y=0.0;
                           //2
                      hp2.v[1].x=0.03;
                      hp2.v[1].y=0.03;
                      hp2.v[1].tex_x=1.0;
                      hp2.v[1].tex_y=0.0;
                           //3
                      hp2.v[2].x=0.03;
                      hp2.v[2].y=-0.03;
                      hp2.v[2].tex_x=1.0;
                      hp2.v[2].tex_y=1.0;


    }
    void initAudio(){
        /*int flags=MIX_INIT_MP3;
                int initted=Mix_Init(flags);
                if(initted&flags != flags) {
                    printf("Mix_Init: Failed to init required ogg and mod support!\n");
                    printf("Mix_Init: %s\n", Mix_GetError());
                    // handle error
                }*/

        if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT ,2,4096)<0){
            std::cout<<"can't open audio"<<std::endl;
        }

        gMusic=Mix_LoadMUS("materials/sounds/level.mp3");
        if(gMusic==NULL){
            printf("Mix_LoadMUS(\"song.wav\"): %s\n", Mix_GetError());
            std::cout<<"can't load music"<<std::endl;
        }
        chunks.push_back(Mix_LoadWAV("materials/sounds/shoot.wav"));
        chunks.push_back(Mix_LoadWAV("materials/sounds/select.wav"));
        chunks.push_back(Mix_LoadWAV("materials/sounds/closed.wav"));
        chunks.push_back(Mix_LoadWAV("materials/sounds/open.wav"));
        chunks.push_back(Mix_LoadWAV("materials/sounds/hit.wav"));

        for(int i=0;i<chunks.size();i++){
            if(chunks[i]==NULL){
                std::cout<<"can't load chunk"<<std::to_string(i)<<std::endl;

            }
        }

        Mix_VolumeChunk(chunks[0],15);
        Mix_VolumeChunk(chunks[2],40);
        Mix_VolumeChunk(chunks[4],MIX_MAX_VOLUME);


    }
    void playMusic(){
        Mix_PlayMusic(gMusic,-1);
    }
    void stopMusic(){
        Mix_HaltMusic();
    }
    void playChunk(int chunk_number){

        Mix_PlayChannel(-1,chunks[chunk_number],0);
    }
    ~engine(){
        for(int i=0;i<chunks.size();i++){
            Mix_FreeChunk(chunks[i]);
            chunks[i]=NULL;
        }
        Mix_FreeMusic(gMusic);
        gMusic=NULL;


        Mix_Quit();
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    bool load_texture(std::string_view path,uint number){
        const char* filename=&path[0];
        std::vector<unsigned char> image;
        unsigned int          w = 0;
        unsigned int          h = 0;
        int error = lodepng::decode(image,w,h,filename);
        /*GLuint tex_handl = 0;
        glGenTextures(1, &tex_handl); //texture descriptor
        OM_GL_CHECK()
        glBindTexture(GL_TEXTURE_2D, tex_handl);
        OM_GL_CHECK()*/
        glActiveTexture(GL_TEXTURE0+number);
        glBindTexture(GL_TEXTURE_2D,tex_handl[number]);
        GLint mipmap_level = 0;
        GLint border       = 0;
        glTexImage2D(GL_TEXTURE_2D, // type of texture
                     mipmap_level,  //
                     GL_RGBA,       //color format in
                     static_cast<GLsizei>(w),//texture weight
                     static_cast<GLsizei>(h),//texture height
                     border,
                     GL_RGBA,       // color format out(color what we want to get in opengl)
                     GL_UNSIGNED_BYTE, // Specifies the data type of the texel data
                     &image[0]);
        OM_GL_CHECK()
        //making correct color of zoomed texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        OM_GL_CHECK()
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        OM_GL_CHECK()
        return true;

    }
    bool event(){
        while(SDL_PollEvent(&event_log)){
            if(event_log.type==SDL_QUIT){
                return false;
            }else if(event_log.type==SDL_KEYDOWN){
                if(event_log.key.keysym.scancode==SDL_SCANCODE_G){
                    //blow->start();
                }else if(event_log.key.keysym.scancode==SDL_SCANCODE_W){
                    is_forward=true;
                }else if(event_log.key.keysym.scancode==SDL_SCANCODE_A){
                    rotation='l';
                }else if(event_log.key.keysym.scancode==SDL_SCANCODE_D){
                    rotation='r';
                }
                return true;
            }else if(event_log.type==SDL_KEYUP){
                if(event_log.key.keysym.scancode==SDL_SCANCODE_W){
                    is_forward=false;
                }else if(event_log.key.keysym.scancode==SDL_SCANCODE_A){
                    rotation='n';
                }else if(event_log.key.keysym.scancode==SDL_SCANCODE_D){
                    rotation='n';
                }
                return true;
            }else if(event_log.type==SDL_MOUSEWHEEL){
                if(event_log.wheel.y>0){
                    //helicopter->upSpeed();

                    return true;
                }
                if(event_log.wheel.y<0){
                    //helicopter->downSpeed();

                    return true;
                }
                return true;
            }

        }
        return true;
    }
    void swapBuffers(){
        SDL_GL_SwapWindow(window);
    }
    void initfTriagBackground(){
        background1.v[0].x=-1;
        background1.v[0].y=-1;
        background1.v[0].tex_x=0.0;
        background1.v[0].tex_y=1.0;
                //2
                background1.v[1].x=-1;
                background1.v[1].y=1;
                background1.v[1].tex_x=0.0;
                background1.v[1].tex_y=0.0;
                //3
                background1.v[2].x=1;
                background1.v[2].y=-1;
                background1.v[2].tex_x=1.0;
                background1.v[2].tex_y=1.0;
                //
                background2.v[0].x=-1;
                    background2.v[0].y=1;
                    background2.v[0].tex_x=0.0;
                    background2.v[0].tex_y=0.0;
                    //2
               background2.v[1].x=1;
               background2.v[1].y=1;
               background2.v[1].tex_x=1.0;
               background2.v[1].tex_y=0.0;
                    //3
               background2.v[2].x=1;
               background2.v[2].y=-1;
               background2.v[2].tex_x=1.0;
               background2.v[2].tex_y=1.0;
    }
    std::string initProgramBackground(){
        std::stringstream serr;
        //vertex
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        OM_GL_CHECK()
        std::string_view vertex_shader_src = R"(
                                             #version 300 es

                                             layout(location=0)in vec4 vPosition;
                                             in vec2 a_tex_coord;

                                             out vec2 v_tex_coord;

                                             void main()
                                             {
                                                 v_tex_coord=a_tex_coord;
                                                 gl_Position=vPosition;//vertex pos
                                             }
                                        )";
        const char* source            = vertex_shader_src.data();
        glShaderSource(vert_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(vert_shader);
        OM_GL_CHECK()

        GLint compiled_status = 0;
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(vert_shader, info_len, nullptr, info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(vert_shader);
            OM_GL_CHECK()

            std::string shader_type_name = "vertex";
            serr << "Error compiling shader(vertex)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
            return serr.str();
        }
        //fragment
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        OM_GL_CHECK()
        std::string_view fragment_shader_src = R"(
                                               #version 300 es
                                               precision mediump float;

                                               in vec2 v_tex_coord;

                                               uniform sampler2D s_texture;

                                               out vec4 frag_color;
                                               void main()
                                               {
                                                 frag_color = texture(s_texture, v_tex_coord);
                                               }
                          )";
        /**/
        source                          = fragment_shader_src.data();
        glShaderSource(fragment_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(fragment_shader);
        OM_GL_CHECK()

        compiled_status = 0;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(
                fragment_shader, info_len, nullptr, info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(fragment_shader);
            OM_GL_CHECK()

            serr << "Error compiling shader(fragment)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
            return serr.str();
        }
        //union shaderis in one program

        program_id_simple = glCreateProgram();
        OM_GL_CHECK()
        if (0 == program_id_simple)
        {
            serr << "failed to create gl program";
            return serr.str();
        }

        glAttachShader(program_id_simple, vert_shader);
        OM_GL_CHECK()
        glAttachShader(program_id_simple, fragment_shader);
        OM_GL_CHECK()

        // bind attribute location
        glBindAttribLocation(program_id_simple, 0, "a_position");
        OM_GL_CHECK()
        // link program after binding attribute locations
        glLinkProgram(program_id_simple);
        OM_GL_CHECK()
        // Check the link status
        GLint linked_status = 0;
        glGetProgramiv(program_id_simple, GL_LINK_STATUS, &linked_status);
        OM_GL_CHECK()
        if (linked_status == 0)
            {
                GLint infoLen = 0;
                glGetProgramiv(program_id_simple, GL_INFO_LOG_LENGTH, &infoLen);
                OM_GL_CHECK()
                    std::vector<char> infoLog(static_cast<size_t>(infoLen));
                glGetProgramInfoLog(program_id_simple, infoLen, nullptr, infoLog.data());
                OM_GL_CHECK()
                serr << "Error linking program:\n" << infoLog.data();
                glDeleteProgram(program_id_simple);
                OM_GL_CHECK()
                return serr.str();
            }
        return "";

    }
    std::string initProgramBody(){
        std::stringstream serr;
        //vertex
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        OM_GL_CHECK()
        std::string_view vertex_shader_src = R"(
                                             #version 300 es
                                             uniform mat4 rot_matrix;
                                             layout(location=0)in vec4 vPosition;
                                             in vec2 a_tex_coord;

                                             out vec2 v_tex_coord;

                                             void main()
                                             {
                                                 v_tex_coord=a_tex_coord;
                                                 gl_Position=rot_matrix * vPosition;//vertex pos
                                             }
                                        )";
        const char* source            = vertex_shader_src.data();
        glShaderSource(vert_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(vert_shader);
        OM_GL_CHECK()

        GLint compiled_status = 0;
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(vert_shader, info_len, nullptr, info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(vert_shader);
            OM_GL_CHECK()

            std::string shader_type_name = "vertex";
            serr << "Error compiling shader(vertex)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
            return serr.str();
        }
        //fragment
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        OM_GL_CHECK()
        std::string_view fragment_shader_src = R"(
                                               #version 300 es
                                               precision mediump float;

                                               in vec2 v_tex_coord;

                                               uniform sampler2D s_texture;

                                               out vec4 frag_color;
                                               void main()
                                               {
                                                 frag_color = texture(s_texture, v_tex_coord);
                                               }
                          )";
        /**/
        source                          = fragment_shader_src.data();
        glShaderSource(fragment_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(fragment_shader);
        OM_GL_CHECK()

        compiled_status = 0;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(
                fragment_shader, info_len, nullptr, info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(fragment_shader);
            OM_GL_CHECK()

            serr << "Error compiling shader(fragment)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
            return serr.str();
        }
        //union shaderis in one program

        program_id_body = glCreateProgram();
        OM_GL_CHECK()
        if (0 == program_id_body)
        {
            serr << "failed to create gl program";
            return serr.str();
        }

        glAttachShader(program_id_body, vert_shader);
        OM_GL_CHECK()
        glAttachShader(program_id_body, fragment_shader);
        OM_GL_CHECK()

        // bind attribute location
        glBindAttribLocation(program_id_body, 0, "a_position");
        OM_GL_CHECK()
        // link program after binding attribute locations
        glLinkProgram(program_id_body);
        OM_GL_CHECK()
        // Check the link status
        GLint linked_status = 0;
        glGetProgramiv(program_id_body, GL_LINK_STATUS, &linked_status);
        OM_GL_CHECK()
        if (linked_status == 0)
            {
                GLint infoLen = 0;
                glGetProgramiv(program_id_body, GL_INFO_LOG_LENGTH, &infoLen);
                OM_GL_CHECK()
                    std::vector<char> infoLog(static_cast<size_t>(infoLen));
                glGetProgramInfoLog(program_id_body, infoLen, nullptr, infoLog.data());
                OM_GL_CHECK()
                serr << "Error linking program:\n" << infoLog.data();
                glDeleteProgram(program_id_body);
                OM_GL_CHECK()
                return serr.str();
            }

        return "";

    }
    std::string activateProgBackground(uint8_t text_num){
        glUseProgram(program_id_simple);
        OM_GL_CHECK()

        int location = glGetUniformLocation(program_id_simple, "s_texture");
        OM_GL_CHECK()
        assert(-1 != location);
        int texture_unit = text_num;//number of texture
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        OM_GL_CHECK()

        glUniform1i(location, 0 + texture_unit);
        OM_GL_CHECK()

        glEnable(GL_BLEND);
        OM_GL_CHECK()
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        OM_GL_CHECK()

        return "";
    }
    std::string activateProgBody(uint8_t text_num, glm::mat4 mat_in){
        // turn on rendering with just created shader program
        glUseProgram(program_id_body);
        OM_GL_CHECK()

        GLuint mem=glGetUniformLocation(program_id_body,"rot_matrix");
        OM_GL_CHECK()

        glUniformMatrix4fv(mem,1,GL_FALSE,glm::value_ptr(mat_in));
                OM_GL_CHECK()


        int location = glGetUniformLocation(program_id_body, "s_texture");
        OM_GL_CHECK()
        assert(-1 != location);
        int texture_unit = text_num;//number of texture
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        OM_GL_CHECK()

        glUniform1i(location, 0 + texture_unit);
        OM_GL_CHECK()

        glEnable(GL_BLEND);
        OM_GL_CHECK()
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        OM_GL_CHECK()

        return "";
    }
    void render_triangle(const triangle& t){
        //vertexs
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                              &t.v[0].x);
        OM_GL_CHECK()

        glEnableVertexAttribArray(0);
        OM_GL_CHECK()
        //textures
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                             &t.v[0].tex_x);
        OM_GL_CHECK()
        glEnableVertexAttribArray(1);
        OM_GL_CHECK()

        glDrawArrays(GL_TRIANGLES, 0, 3);
        OM_GL_CHECK()

    }
    void render_sprite(const sprite& t){
        render_triangle(t.triag_1);
        render_triangle(t.triag_2);
    }
    void render_background(){
       activateProgBackground(0);
       render_triangle(background1);
       render_triangle(background2);

    }
    void render_numbackground(u_int8_t textnum){
        activateProgBackground(textnum);
        render_triangle(background1);
        render_triangle(background2);

    }
    void render_hp(uint8_t hp,bool cargo_open){
        float g=-0.95f;
        for(int i=0;i<hp;i++){
            activateProgBody(12,glm::translate(glm::mat4(1.0f),glm::vec3(g,0.95f,0.0f)));
            render_triangle(hp1);
            render_triangle(hp2);

            g+=0.1;
        }
        if(cargo_open==true){
            activateProgBody(13,glm::translate(glm::mat4(1.0f),glm::vec3(g,0.95f,0.0f)));
            render_triangle(hp1);
            render_triangle(hp2);
        }

    }
    void render_body(){
        render_triangle(body1);
        render_triangle(body2);
    }
    void render(){
        //nxt 3 lines = render_background
        activateProgBackground(0);
        render_triangle(background1);
        render_triangle(background2);



        //activateProgBody(helicopter->getBodyTexNum(),helicopter->getMoveMat());
        render_triangle(body1);
        render_triangle(body2);

        /*if(blow->isactive()){
            activateProgBody(blow->get_tex_num(),helicopter->getMoveMat());
            render_sprite(blow->getframe());

        }*/

        //activateProgBody(helicopter->getBladesTexNum(),helicopter->getWingMat());
        render_triangle(body1);
        render_triangle(body2);



    }
    void logic(){
        /*if(is_forward){
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
        helicopter->rotateWing();*/
    }
    triangle gettriangle(int a){
        if(a==1){
            return body1;
        }else{
            return body2;
        }
    }
};
//tests for gl functions
static const char* source_to_strv(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

static const char* type_to_strv(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "POP_GROUP";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
    }
    return "unknown";
}
static const char* severity_to_strv(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
    }
    return "unknown";
}

static std::array<char, GL_MAX_DEBUG_MESSAGE_LENGTH> local_log_buff;

static void APIENTRY callback_opengl_debug(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, [[maybe_unused]] const void* userParam)
{
    // The memory formessageis owned and managed by the GL, and should onlybe
    // considered valid for the duration of the function call.The behavior of
    // calling any GL or window system function from within thecallback function
    // is undefined and may lead to program termination.Care must also be taken
    // in securing debug callbacks for use with asynchronousdebug output by
    // multi-threaded GL implementations.  Section 18.8 describes thisin further
    // detail.

    auto& buff{ local_log_buff };
    int   num_chars = std::snprintf(
        buff.data(), buff.size(), "%s %s %d %s %.*s\n", source_to_strv(source),
        type_to_strv(type), id, severity_to_strv(severity), length, message);

    if (num_chars > 0)
    {
        // TODO use https://en.cppreference.com/w/cpp/io/basic_osyncstream
        // to fix possible data races
        // now we use GL_DEBUG_OUTPUT_SYNCHRONOUS to garantie call in main
        // thread
        std::cerr.write(buff.data(), num_chars);
    }
}

