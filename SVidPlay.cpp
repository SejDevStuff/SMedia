//              SMedia
//  (c) 2023 Sej (github.com/SejDevStuff)
// 
//  SVidPlay
//  Very simple video player for .svid files. Uses SDL2 for window rendering
#include "s_vid.hpp"
#include <SDL2/SDL.h>

void playVideo(sm::SVidData video, sm::S_Vid &videoProcess)
{
    if (SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        cout << "err: cannot init video" << endl;
        return;
    }
    SDL_Window* win = SDL_CreateWindow("SVidPlay (ESC to stop)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)video.width, (int)video.height, SDL_WINDOW_SHOWN);

    if (win == NULL)
    {
        cout << "err: cannot init window" << endl;
        return;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);
    SDL_Event e;
    
    bool run = true;
    
    unsigned int currTick = SDL_GetTicks();
    unsigned int lastFrameTick = SDL_GetTicks();
    double delta = 0;
    int framesShown = 0;
    sm::SImgData lastFrame;

    while (run)
    {
        while (SDL_PollEvent(&e)) 
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

        currTick = SDL_GetTicks();
        delta = currTick - lastFrameTick;

        if (delta > 1000/(double)video.fps)
        {
            lastFrameTick = currTick;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            if (framesShown < video.framesTotal)
            {
                framesShown++;
                lastFrame = videoProcess.get_frame(video, framesShown, lastFrame);
                for (int i = 0; i < lastFrame.pixelArray.size(); i++)
                {
                    sm::SImgPixelComponent pix = lastFrame.pixelArray[i];
                    SDL_SetRenderDrawColor(renderer, pix.r, pix.g, pix.b, 255);
                    SDL_RenderDrawPoint(renderer, pix.x, pix.y);
                }
            }

            SDL_RenderPresent(renderer);
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
    cout << "Loading " << filepath << endl;
    sm::S_Vid video;
    sm::SVidData vidDat = video.load_svid(filepath);
    if (video.error())
    {
        cout << video.getError() << endl;
        return 1;
    }
    playVideo(vidDat, video);
}