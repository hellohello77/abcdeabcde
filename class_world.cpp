#include<SDL2/SDL.h>
#include<iostream>
#include<stdio.h>
#include<SDL2/SDL_image.h>
using namespace std;
SDL_Window* window=NULL;//未來要刪掉

SDL_Event  e;//未來要刪掉
bool quit;//未來要刪掉
SDL_Surface* hungyun=NULL;
SDL_Surface* spotcircle=NULL;
SDL_Renderer* world_renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
SDL_Rect world_rect,world_hung_rect;
struct position
{
    int x;//座標
    int y;//座標
    int level;//對應關卡
    bool _end;//是不是最後一個spot(可跳下一個世界)
    bool _start;//是不是第一個spot(可跳上一個世界)
    int _escape;//目前所在世界(原諒我奇怪的變數名)
    bool esc;//有沒有用settings切換世界
    bool enter;//可不可以進入(前面關卡有沒有破完)
};
enum enterspot{help,start,one,two,three,ending};
class world
{
    public:
    bool enterworld;
    position spot[6],character,intersect[15];//已修改,character為初始位置，不能改
    SDL_Surface* world_background;
    bool _help,_ending;
    void scene(position* now)//已修改
    {
        SDL_RenderClear(world_renderer);
        SDL_Texture* background_texture=NULL;//背景
        background_texture=SDL_CreateTextureFromSurface(world_renderer,world_background);//背景
        SDL_RenderCopy(world_renderer,background_texture,NULL,&world_rect);//背景
        drawroute();//line
        drawspot();//spot
        SDL_Texture* hung_texture=NULL;//宏
        world_hung_rect.x=now->x;world_hung_rect.y=now->y;//宏
        hung_texture=SDL_CreateTextureFromSurface(world_renderer,hungyun);//宏
        SDL_RenderCopy(world_renderer,hung_texture,NULL,&world_hung_rect);//宏
        SDL_RenderPresent(world_renderer);
        SDL_DestroyTexture(background_texture);
        background_texture=NULL;
        SDL_DestroyTexture(hung_texture);
        hung_texture=NULL;
        return ;
    }
    void _move(position &now)//已修改，缺動畫
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
                                if(settings(&now))
                                {
                                    escape=0;
                                    now.esc=1;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        return;
    }
    void drawroute()
    {
        for(int i=0;i<14;i++)
        {
            if(intersect[i].x>0 && intersect[i+1].x>0)
            {
                if(intersect[i].x==intersect[i+1].x)
                {
                    if(intersect[i].y<intersect[i+1].y)
                    {
                        SDL_Rect fillroute = { intersect[i].x-10, intersect[i].y-10, 20, intersect[i+1].y-intersect[i].y+20 };
                        if(intersect[i+1].enter)
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                    else
                    {
                        SDL_Rect fillroute = { intersect[i+1].x-10, intersect[i+1].y-10, 20, intersect[i].y-intersect[i+1].y+20 };
                        if(intersect[i+1].enter)
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                }
                else if(intersect[i].y==intersect[i+1].y)
                {
                    if(intersect[i].x<intersect[i+1].x)
                    {
                        SDL_Rect fillroute = { intersect[i].x-10, intersect[i].y-10, intersect[i+1].x-intersect[i].x+20, 20 };
                        if(intersect[i+1].enter)
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                    else
                    {
                        SDL_Rect fillroute = { intersect[i+1].x-10, intersect[i+1].y-10, intersect[i].x-intersect[i+1].x+20 , 20};
                        if(intersect[i+1].enter)
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0xFF, 0x00, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                }
            }
        }
    }
    void drawspot()
    {
        //if
    }
    void animation(position START,position END)//缺
    {

    }
    bool settings(position *now_p)
    {
        int op=-1;
        bool _space=1;
        while(_space)
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
                        if(op!=-2)
                            op-=1;
                        scene(now_p);
                        break;
                    case SDLK_UP:
                        if(op!=2)
                            op+=1;
                        break;
                    case SDLK_SPACE:
                        if(op==-1)
                            return 0;
                        else
                            now_p->_escape=op;
                        _space=0;
                        break;
                    }
                }
            }
        }
        return 1;
    }
};
void adjustenter(position &now)
{

}
int main(int argc, char* argv[])
{
    window=SDL_CreateWindow("world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,1200, 800, SDL_WINDOW_SHOWN );
    world _world[3];
    //初始化world
    position now=_world[0].spot[start];
    while(!quit)
    {
        _world[now._escape].scene(&now);
        _world[now._escape]._move(now);
        if(now.level!=-1)
        {
            //呼叫關卡(如果贏了呼叫adjustenter())
        }
        else if(now._start==1 && now._escape!=0)
        {
            now._escape--;
            now=_world[now._escape].spot[ending];
        }
        else if(now._end==1)
        {
            now._escape++;
            now=_world[now._escape].spot[start];
        }
        else if(now.esc==1)
        {
            if(now._escape==-2)
            {
                //切到welcomesurface
            }
            else if(now._escape==0 || now._escape==1 ||now._escape==2)
            {
                now=_world[now._escape].spot[start];
            }
        }
    }
    return 0;
}
