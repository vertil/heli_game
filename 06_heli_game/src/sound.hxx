#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <string_view>

class sound{
    SDL_AudioSpec format;
    Uint32 lenght=0;
    Uint32 pos=0;
    Uint8*buffer;


public:

    sound(uint32_t wav_lenght,
          SDL_AudioSpec wav_audio_spec){
        lenght=wav_lenght;
        format=wav_audio_spec;
    }
    bool load(std::string_view path){
        if(SDL_LoadWAV(path.data(),&format,&buffer,&lenght)==NULL){
            return false;
        }
        return true;
    }

    void play(){

    }
    void pause(){

    }
    void stop(){

    }
    void loop(){

    }

};
