#include<SDL2/SDL.h>
#include<iostream>
#include<stdio.h>
#include<SDL2/SDL_image.h>
using namespace std;
SDL_Window* window=NULL;//未來要刪掉
SDL_Event  e;//未來要刪掉
bool quit;//未來要刪掉
SDL_Surface* hungyun=NULL;
SDL_Renderer* world_renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
SDL_Rect world_rect,world_hung_rect;
struct position
{
    int x;
    int y;
    int level;
    bool start_end;
    int _escape;//已修改
};
enum enterspot{help,start,one,two,three,ending};
class world
{
    bool enterworld;
    position spot[6],character;//已修改,character為初始位置，不能改
    SDL_Surface* world_background;
    bool _help,_ending;
    void scene()//已修改
    {
        SDL_RenderClear(world_renderer);
        SDL_Texture* background_texture=NULL;
        SDL_Texture* hung_texture=NULL;
        background_texture=SDL_CreateTextureFromSurface(world_renderer,world_background);
        SDL_RenderCopy(world_renderer,background_texture,NULL,&world_rect);
        world_hung_rect.x=character.x;world_hung_rect.y=character.y;
        hung_texture=SDL_CreateTextureFromSurface(world_renderer,hungyun);
        SDL_RenderCopy(world_renderer,hung_texture,NULL,&world_hung_rect);
        SDL_RenderPresent(world_renderer);
        SDL_DestroyTexture(background_texture);
        background_texture=NULL;
        SDL_DestroyTexture(hung_texture);
        hung_texture=NULL;
        //還需增加宏宏在頁面上
    }
    position _move(position &now)//已修改，缺動畫
    {
        bool space=1,escape=1;
        while(space && escape)
        {
            for(int i=0;i<6;i++)
            {
                if(now.x==spot[i].x && now.y==spot[i].y)
                {
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                        else if( e.type == SDL_KEYDOWN )
                        {
                            switch(e.key.keysym.sym)
                            {
                            case SDLK_DOWN:
                                if(i>start || (i==start && _help==1))
                                    now=spot[i-1];
                                break;
                            case SDLK_UP:
                                if(i<three || (i==three && _ending==1))
                                    now=spot[i+1];
                                break;
                            case SDLK_SPACE:
                                space=0;
                                break;
                            case SDLK_ESCAPE:
                                settings(&now);
                                if(now._escape)escape=0;
                                break;
                            }
                        }
                    }
                }
            }
        }
        return now;
    }
    void animation(position START,position END)//缺
    {

    }
    void settings(position *now_pointer)
    {
        /*
        原嘉的function
        */
    }
};
int main(int argc, char* argv[])
{
    return 0;
}
