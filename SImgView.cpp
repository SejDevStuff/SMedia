//              SMedia
//  (c) 2023 Sej (github.com/SejDevStuff)
// 
//  SImgView
//  Image viewer for .simg files, uses SDL2 to render the window.


#include "s_img.hpp"
#include <SDL2/SDL.h>

void renderImage(sm::SImgData imgdat)
{
    if (SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        cout << "err: cannot init video" << endl;
        return;
    }
    SDL_Window* win = SDL_CreateWindow("SImgView (ESC to stop)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)imgdat.width, (int)imgdat.height, SDL_WINDOW_SHOWN);

    if (win == NULL)
    {
        cout << "err: cannot init window" << endl;
        return;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);
    SDL_Event e;
    
    bool run = true;
    
    while (run)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        for (int i = 0; i < imgdat.pixelArray.size(); i++)
        {
            sm::SImgPixelComponent pix = imgdat.pixelArray[i];
            SDL_SetRenderDrawColor(renderer, pix.r, pix.g, pix.b, 255);
            SDL_RenderDrawPoint(renderer, pix.x, pix.y);
        }
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&e)) 
        {
            switch (e.type) {
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        run = false;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        run = false;
                    }
                    break;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 1;
    }
    string filepath = argv[1];
    sm::S_Img image;
    sm::SImgData imgdat = image.load_simg(filepath);
    cout << "Loading " << filepath << endl;
    if (image.error())
    {
        cout << image.getError() << endl;
        return 1;
    }
    renderImage(imgdat);
}