#include "Texture.h"
#include <SDL.h>
#include <cmath>

Texture::Texture()
{
    mTexture = NULL;
    textureWidth = 0,textureHeight = 0;
}

Texture::~Texture()
{
    free();
}
void Texture::free()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        textureWidth = 0;
        textureHeight = 0;
    }
}
