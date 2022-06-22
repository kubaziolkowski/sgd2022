#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <sstream>

#ifndef SDL2_BASIC_SETUP_TEXTURE_H
#define SDL2_BASIC_SETUP_TEXTURE_H
using namespace std;

class Texture
{
public:
    Texture();

    ~Texture();

    bool loadFromFile(std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)

#endif

    //Deallocates texture
    void free();


    //Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = NULL, double angle = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);



private:
    SDL_Texture* mTexture;

    int textureWidth,textureHeight;
};


#endif //SDL2_BASIC_SETUP_TEXTURE_H
