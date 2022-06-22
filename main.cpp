#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include "Texture.h"
#include "Jetski.h"
#include "Variables.h"

using namespace std;

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

Texture jetskiTexture;
Texture backgroundTexture;

int pdT = 0, dT = 0, fdT = 0;

bool Texture::loadFromFile(std::string path)
{
    free();

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    textureWidth = loadedSurface->w;
    textureHeight = loadedSurface->h;

            SDL_FreeSurface(loadedSurface);

    mTexture = newTexture;
    return mTexture != NULL;
}

void Jetski::move(int border)
{
    if (velocity > 30) {
        velocity = 30;
    } else if (velocity < -30) {
        velocity = -30;
    }

    if (up) velocity = velocity + acceleration * fdT;

    if (down) velocity = velocity - acceleration * fdT;

    if (!up && !down) {
        if (velocity > 0) velocity = velocity - acceleration * fdT;
        else if (velocity < 0) velocity = velocity + acceleration * fdT;
    }

    if (rightt && velocity != 0) {
        angle = angle + turnSpeed * velocity / maximumLinearVelocity;
    }
    if (leftt && velocity != 0) {
        angle = angle - turnSpeed * velocity / maximumLinearVelocity;
    }

    switch (border)
    {
        case 0: {
            jetskiX = jetskiX + cos(angle) * velocity;
            jetskiY = jetskiY + sin(angle) * velocity;
            break; //neutral
        }
        case 1: {
            jetskiX = 1;
            jetskiY = jetskiY + sin(angle) * velocity;
            break; //left
        }
        case 2: {
            jetskiX = 3900;
            jetskiY = jetskiY + sin(angle) * velocity;
            break; //right
        }
        case 3: {
            jetskiX = jetskiX + cos(angle) * velocity;
            jetskiY = 1;
            break; //up
        }
        case 4: {
            jetskiX = jetskiX + cos(angle) * velocity;
            jetskiY = 1948;
            break; //down
        }

    }

    shape.x = jetskiX;
    shape.y = jetskiY;

    if (jetskiX > 640 ) {
        offsetX = jetskiX - 640;
    }
    if (jetskiY > 380 ) {
        offsetY = jetskiY - 380;
    }


}


void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = { x, y, textureWidth, textureHeight };

    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

bool isColliding(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA < topB)
    {
        return false;
    }

    if (topA > bottomB)
    {
        return false;
    }

    if (rightA < leftB)
    {
        return false;
    }

    if (leftA > rightB)
    {
        return false;
    }

    else return true;
}

Jetski::Jetski()
{
    jetskiX = 120,jetskiY = 120;

    shape.w = jetskiWidth,shape.h = jetskiHeight;

    velocity = 0,turnSpeed = 0.09;
}

void Jetski::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
            case SDLK_w:
                up = 1;
                break;
            case SDLK_s:
                down = 1;
                break;
            case SDLK_d:
                rightt = 1;
                break;
            case SDLK_a:
                leftt = 1;
                break;
        }
    }
    if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
            case SDLK_w:
                up = 0;
                break;
            case SDLK_s:
                down = 0;
                break;
            case SDLK_d:
                rightt = 0;
                break;
            case SDLK_a:
                leftt = 0;
                break;
        }
    }
}
bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("Jetski", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL)
            {
                success = false;
            }
            else
            {

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    success = false;
                }
            }
        }
    }

    return success;
}


int main(int argc, char* args[])
{
           init();

            jetskiTexture.loadFromFile("/Users/kuba/CLionProjects/grasgd/resources/jetski.png");

            backgroundTexture.loadFromFile("/Users/kuba/CLionProjects/grasgd/resources/map2.png");

            bool quit = false;

            SDL_Event e;

            Jetski jetski;


            SDL_Rect leftBorder,rightBorder,upperBorder,bottomBorder;

            leftBorder.x = 0,leftBorder.y = 0,leftBorder.w = 0,leftBorder.h = 2048;
            rightBorder.x = 4096,rightBorder.y = 0,rightBorder.w = 0,rightBorder.h = 2048;
            upperBorder.x = 0,upperBorder.y = 0,upperBorder.w = 4096,upperBorder.h = 0;
            bottomBorder.x = 0,bottomBorder.y = 2048,bottomBorder.w = 4096,bottomBorder.h = 0;

            SDL_Rect goldBox1;
            goldBox1.x = 646,goldBox1.y = 128,goldBox1.w = 67,goldBox1.h = 67;

            //coordinates of other gold boxes: (681,631),(703,1187),(289,1167),(3705,1425),(3721,67)
            //coordinates of traps: (725,957),(1805,1253),(3701,849)

            SDL_Rect meta;
            meta.x = 3360,meta.y = 100,meta.w = 140, meta.h = 85;


            SDL_Rect goldBoxes[6] = { goldBox1};

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    jetski.handleEvent(e);
                }

                pdT = dT;
                dT = SDL_GetTicks();

                fdT = dT - pdT;
                if (fdT < 0.16) {
                    SDL_Delay(0.16 - fdT);
                }

                if (isColliding(leftBorder, jetski.shape)) {
                    jetski.move(1);
                }
                else if (isColliding(rightBorder, jetski.shape)) {
                    jetski.move(2);
                }
                else if (isColliding(upperBorder, jetski.shape)) {
                    jetski.move(3);
                }
                else if (isColliding(bottomBorder, jetski.shape)) {
                    jetski.move(4);
                }
                else {
                    jetski.move(0);
                }

                int points = 0;
                if (isColliding(jetski.shape,goldBoxes[points])) {
                    points++;
                    cout<<points;
                }

                if (isColliding(jetski.shape,meta)) {
                    //close();
                    SDL_Quit();

                }


                SDL_RenderClear(renderer);

                backgroundTexture.render(-offsetX, -offsetY);
                jetskiTexture.render(jetski.jetskiX - offsetX,
                                     jetski.jetskiY - offsetY,
                                     0, angle * 180 / M_PI);

                SDL_RenderPresent(renderer);
            }
             SDL_Quit();

            return 0;
}