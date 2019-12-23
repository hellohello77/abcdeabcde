#include<SDL2/SDL.h>
#include<iostream>
#include<stdio.h>
#include<SDL2/SDL_image.h>
using namespace std;
SDL_Window* window=NULL;//未來要刪掉
SDL_Event  e;//未來要刪掉
bool quit=0;//未來要刪掉
SDL_Renderer* world_renderer=NULL;//未來要刪掉
SDL_Surface* storyplot[6]={};//begin
SDL_Texture* beginstory_texture=NULL;//begin
SDL_Surface* hungyun=NULL;//world
SDL_Surface* spotcircle=NULL;//world
SDL_Surface* settingssurface=NULL;//world
SDL_Rect world_hung_rect,settings_rect;//world
SDL_Texture* background_texture=NULL;//world
SDL_Texture* hung_texture=NULL;//world
SDL_Texture* spotcircle_texture=NULL;//world
SDL_Texture* settings_texture=NULL;//world
int enterworld=0;//world
bool sett=0;//world
SDL_Surface* _storyplot[6]={};//end
SDL_Texture* endstory_texture=NULL;//end
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
    position spot[6],character,intersect[15];//已修改,character為初始位置，不能改
    SDL_Surface* world_background;
    bool _help,_ending;
    void scene(position* now)//已修改
    {
        SDL_SetRenderDrawColor(world_renderer,0x00,0x00,0x00,0xFF);
        SDL_RenderClear(world_renderer);
        background_texture=SDL_CreateTextureFromSurface(world_renderer,world_background);//背景
        SDL_RenderCopy(world_renderer,background_texture,NULL,NULL);//背景
        drawroute();//line
        drawspot();//spot
        world_hung_rect.x=(now->x)-30;world_hung_rect.y=(now->y)-40;//宏
        hung_texture=SDL_CreateTextureFromSurface(world_renderer,hungyun);//宏
        SDL_RenderCopy(world_renderer,hung_texture,NULL,&world_hung_rect);//宏
        if(sett)
        {
            settings_texture=SDL_CreateTextureFromSurface(world_renderer,settingssurface);
            SDL_RenderCopy(world_renderer,settings_texture,NULL,&settings_rect);
        }
        SDL_RenderPresent(world_renderer);
        SDL_DestroyTexture(background_texture);
        background_texture=NULL;
        SDL_DestroyTexture(hung_texture);
        hung_texture=NULL;
        if(sett)
        {
            SDL_FreeSurface(settingssurface);
            SDL_DestroyTexture(settings_texture);
            settings_texture=NULL;
        }
        return ;
    }
    void _move(position &now)//已修改
    {
        bool space=1,escape=1;
        while(space && escape)
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
                for(int i=0;i<6;i++)
                {
                    if(now.x==spot[i].x && now.y==spot[i].y)
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
                                {
                                    animation(spot[i],spot[i-1],now);
                                    now=spot[i-1];
                                }
                                break;
                            case SDLK_UP:
                                if((i<three || (i==three && _ending==1)) && spot[i+1].enter)
                                {
                                    animation(spot[i],spot[i+1],now);
                                    now=spot[i+1];
                                }
                                break;
                            case SDLK_SPACE:
                                space=0;
                                break;
                            case SDLK_ESCAPE:
                                if(settings(&now))
                                {
                                    now.esc=1;
                                    return;
                                }
                                break;
                            }
                        }break;
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
                        if(intersect[i+1].enter==1)
                            SDL_SetRenderDrawColor( world_renderer, 0xEE, 0xD0, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0x66, 0x55, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                    else
                    {
                        SDL_Rect fillroute = { intersect[i+1].x-10, intersect[i+1].y-10, 20, intersect[i].y-intersect[i+1].y+20 };
                        if(intersect[i+1].enter==1)
                            SDL_SetRenderDrawColor( world_renderer, 0xEE, 0xD0, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0x66, 0x55, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                }
                else if(intersect[i].y==intersect[i+1].y)
                {
                    if(intersect[i].x<intersect[i+1].x)
                    {
                        SDL_Rect fillroute = { intersect[i].x-10, intersect[i].y-10, intersect[i+1].x-intersect[i].x+20, 20 };
                        if(intersect[i+1].enter)
                            SDL_SetRenderDrawColor( world_renderer, 0xEE, 0xD0, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0x66, 0x55, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                    else
                    {
                        SDL_Rect fillroute = { intersect[i+1].x-10, intersect[i+1].y-10, intersect[i].x-intersect[i+1].x+20 , 20};
                        if(intersect[i+1].enter==1)
                            SDL_SetRenderDrawColor( world_renderer, 0xEE, 0xD0, 0x00, 0xFF );
                        else
                            SDL_SetRenderDrawColor( world_renderer, 0x66, 0x55, 0x00, 0xFF );
                        SDL_RenderFillRect( world_renderer, &fillroute );
                    }
                }
            }
        }
    }
    void drawspot()
    {
        for(int i=0;i<6;i++)
        {
            if(spot[i].x!=0)
            {
                SDL_Rect spot_rect={spot[i].x-20,spot[i].y-20,40,40};
                spotcircle_texture=SDL_CreateTextureFromSurface(world_renderer,spotcircle);
                SDL_RenderCopy(world_renderer,spotcircle_texture,NULL,&spot_rect);
                SDL_DestroyTexture(spotcircle_texture);
                spotcircle_texture=NULL;
            }
        }
    }
    void animation(position START,position END,position &now)
    {
        int S=-1,E=-1;
        for(int i=0;i<15;i++)
        {
            if(START.x==intersect[i].x && START.y==intersect[i].y)
                S=i;
            if(END.x==intersect[i].x && END.y==intersect[i].y)
                E=i;
        }
        if(E>S)
        {
            for(int i=S;i<E;i++)
            {
                if(intersect[i].x==intersect[i+1].x)
                {
                    while((now.y-10)>intersect[i+1].y)
                    {
                        now.y-=10;
                        scene(&now);
                    }
                    now.y=intersect[i+1].y;
                    scene(&now);
                }
                else if(intersect[i].y==intersect[i+1].y)
                {
                    while((now.x+10)<intersect[i+1].x)
                    {
                        now.x+=10;
                        scene(&now);
                    }
                    now.x=intersect[i+1].x;
                    scene(&now);
                }
            }
        }
        else if(E<S)
        {
            for(int i=S;i>E;i--)
            {
                if(intersect[i].x==intersect[i-1].x)
                {
                    while((now.y+10)<intersect[i-1].y)
                    {
                        now.y+=10;
                        scene(&now);
                    }
                    now.y=intersect[i-1].y;
                    scene(&now);
                }
                else if(intersect[i].y==intersect[i-1].y)
                {
                    while((now.x-10)>intersect[i-1].x)
                    {
                        now.x-=10;
                        scene(&now);
                    }
                    now.x=intersect[i-1].x;
                    scene(&now);
                }
            }
        }
        return;
    }
    bool settings(position *now_p)
    {
        int op=-1;
        sett=1;
        if(enterworld==0)
        {
            settingssurface=IMG_Load("images/set0--1.png");
        }
        else if(enterworld==1)
        {
            settingssurface=IMG_Load("images/set1--1.png");
        }
        else if(enterworld==2)
        {
            settingssurface=IMG_Load("images/set2--1.png");
        }
        scene(now_p);
        sett=0;
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
                        {
                            op-=1;
                        }
                        break;
                    case SDLK_UP:
                        if(op!=2 && op<enterworld)
                            op+=1;
                        scene(now_p);
                        break;
                    case SDLK_SPACE:
                        if(op==-1)
                        {
                            scene(now_p);
                            return 0;
                        }
                        else
                        {
                            now_p->_escape=op;
                            _space=0;
                            return 1;
                        }
                        break;
                    }
                    sett=1;
                    if(enterworld==0)
                    {
                        if(op==-2)settingssurface=IMG_Load("images/set0--2.png");
                        if(op==-1)settingssurface=IMG_Load("images/set0--1.png");
                        if(op==0)settingssurface=IMG_Load("images/set0-0.png");
                    }
                    else if(enterworld==1)
                    {
                        if(op==-2)settingssurface=IMG_Load("images/set1--2.png");
                        if(op==-1)settingssurface=IMG_Load("images/set1--1.png");
                        if(op==0)settingssurface=IMG_Load("images/set1-0.png");
                        if(op==1)settingssurface=IMG_Load("images/set1-1.png");
                    }
                    else if(enterworld==2)
                    {
                        if(op==-2)settingssurface=IMG_Load("images/set2--2.png");
                        if(op==-1)settingssurface=IMG_Load("images/set2--1.png");
                        if(op==0)settingssurface=IMG_Load("images/set2-0.png");
                        if(op==1)settingssurface=IMG_Load("images/set2-1.png");
                        if(op==2)settingssurface=IMG_Load("images/set2-2.png");
                    }
                    scene(now_p);
                    sett=0;
                }
            }
        }
        return 1;
    }
};
void adjustenter(position now);
void initbegin();
void initend();
void begin_story();
void end_story();
void initworld();
world _world[3];//world

int main(int argc, char* argv[])
{
    window=SDL_CreateWindow("world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,1200, 800, SDL_WINDOW_SHOWN );
    world_renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    initworld();
    initbegin();
    initend();
    begin_story();
    position now=_world[0].spot[start];
    while(1)
    {
        _world[now._escape].scene(&now);
        _world[now._escape]._move(now);
        if(now.esc==1)
        {
            if(now._escape==-2)
            {
                cout<<"welcomesurface"<<endl;//切到welcomesurface
            }
            else if(now._escape==0 || now._escape==1 ||now._escape==2)
            {
                now=_world[now._escape].spot[start];
            }
        }
        else if(now.level>-1)
        {
            cout<<now.level<<endl;
            int i=1;
            if(i==1)
                adjustenter(now);//呼叫關卡(如果贏了呼叫adjustenter(now))
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
    }
    end_story();
    return 0;
}
void initworld()
{
    _world[0].world_background=IMG_Load("images/world1.png");
    _world[1].world_background=IMG_Load("images/world2.png");
    _world[2].world_background=IMG_Load("images/world3.png");
    _world[0]._help=1;_world[0]._ending=1;
    _world[1]._help=0;_world[1]._ending=1;
    _world[2]._help=0;_world[2]._ending=0;
    hungyun=IMG_Load("images/hungyun.png");
    spotcircle=IMG_Load("images/spotcircle.png");
    for(int i=0;i<3;i++)
    {
        if(_world[i]._help==1)
        {
            _world[i].spot[0].x=100;
            _world[i].spot[0].y=750;
        }
        else
        {
            _world[i].spot[0].x=0;
            _world[i].spot[0].y=0;
        }
        _world[i].spot[1].x=300;
        _world[i].spot[1].y=700;
        _world[i].spot[2].x=300;
        _world[i].spot[2].y=600;
        _world[i].spot[3].x=500;
        _world[i].spot[3].y=500;
        _world[i].spot[4].x=700;
        _world[i].spot[4].y=300;
        if(_world[i]._ending==1)
        {
            _world[i].spot[5].x=600;
            _world[i].spot[5].y=200;
        }
        else
        {
            _world[i].spot[5].x=0;
            _world[i].spot[5].y=0;
        }
    }
    int t=0;
    for(int j=0;j<3;j++)
    {
        for(int i=0;i<6;i++)
        {
            if(i==one || (i==help && _world[j]._help) || i==two || i==three)
            {
                _world[j].spot[i].level=t;
                t++;
            }
            else
                _world[j].spot[i].level=-1;
            if(i==ending && _world[j]._ending)
            _world[j].spot[i]._end=1;
            else
                _world[j].spot[i]._end=0;
            if(i==start)
            _world[j].spot[i]._start=1;
            else
                _world[j].spot[i]._start=0;
            _world[j].spot[i]._escape=j;
            _world[j].spot[i].esc=0;
            if(i<two)
            _world[j].spot[i].enter=1;
            else
                _world[j].spot[i].enter=0;
        }
        _world[j].character=_world[j].spot[start];
    }
    for(int i=0;i<15;i++)
    {
        _world[0].intersect[i].level=-1;
        _world[0].intersect[i]._end=0;
        _world[0].intersect[i]._start=0;
        _world[0].intersect[i]._escape=0;
        _world[0].intersect[i].esc=0;
        if(i<5)
            _world[0].intersect[i].enter=1;
        else
            _world[0].intersect[i].enter=0;
    }
    _world[0].intersect[0].x=100;
    _world[0].intersect[0].y=750;
    _world[0].intersect[1].x=100;
    _world[0].intersect[1].y=725;
    _world[0].intersect[2].x=300;
    _world[0].intersect[2].y=725;
    _world[0].intersect[3].x=300;
    _world[0].intersect[3].y=700;
    _world[0].intersect[4].x=300;
    _world[0].intersect[4].y=600;
    _world[0].intersect[5].x=300;
    _world[0].intersect[5].y=550;
    _world[0].intersect[6].x=500;
    _world[0].intersect[6].y=550;
    _world[0].intersect[7].x=500;
    _world[0].intersect[7].y=500;
    _world[0].intersect[8].x=500;
    _world[0].intersect[8].y=400;
    _world[0].intersect[9].x=700;
    _world[0].intersect[9].y=400;
    _world[0].intersect[10].x=700;
    _world[0].intersect[10].y=300;
    _world[0].intersect[11].x=700;
    _world[0].intersect[11].y=250;
    _world[0].intersect[12].x=600;
    _world[0].intersect[12].y=250;
    _world[0].intersect[13].x=600;
    _world[0].intersect[13].y=200;
    _world[0].intersect[14].x=0;
    _world[0].intersect[14].y=0;
    for(int i=0;i<15;i++)
    {
        _world[1].intersect[i].level=-1;
        _world[1].intersect[i]._end=0;
        _world[1].intersect[i]._start=0;
        _world[1].intersect[i]._escape=1;
        _world[1].intersect[i].esc=0;
        if(i<2)
            _world[1].intersect[i].enter=1;
        else
            _world[1].intersect[i].enter=0;
    }
    _world[1].intersect[0].x=300;
    _world[1].intersect[0].y=700;
    _world[1].intersect[1].x=300;
    _world[1].intersect[1].y=600;
    _world[1].intersect[2].x=300;
    _world[1].intersect[2].y=550;
    _world[1].intersect[3].x=500;
    _world[1].intersect[3].y=550;
    _world[1].intersect[4].x=500;
    _world[1].intersect[4].y=500;
    _world[1].intersect[5].x=500;
    _world[1].intersect[5].y=400;
    _world[1].intersect[6].x=700;
    _world[1].intersect[6].y=400;
    _world[1].intersect[7].x=700;
    _world[1].intersect[7].y=300;
    _world[1].intersect[8].x=700;
    _world[1].intersect[8].y=250;
    _world[1].intersect[9].x=600;
    _world[1].intersect[9].y=250;
    _world[1].intersect[10].x=600;
    _world[1].intersect[10].y=200;
    _world[1].intersect[11].x=0;
    _world[1].intersect[11].y=0;
    _world[1].intersect[12].x=0;
    _world[1].intersect[12].y=0;
    _world[1].intersect[13].x=0;
    _world[1].intersect[13].y=0;
    _world[1].intersect[14].x=0;
    _world[1].intersect[14].y=0;
    for(int i=0;i<15;i++)
    {
        _world[2].intersect[i].level=-1;
        _world[2].intersect[i]._end=0;
        _world[2].intersect[i]._start=0;
        _world[2].intersect[i]._escape=1;
        _world[2].intersect[i].esc=0;
        if(i<2)
            _world[2].intersect[i].enter=1;
        else
            _world[2].intersect[i].enter=0;
    }
    _world[2].intersect[0].x=300;
    _world[2].intersect[0].y=700;
    _world[2].intersect[1].x=300;
    _world[2].intersect[1].y=600;
    _world[2].intersect[2].x=300;
    _world[2].intersect[2].y=550;
    _world[2].intersect[3].x=500;
    _world[2].intersect[3].y=550;
    _world[2].intersect[4].x=500;
    _world[2].intersect[4].y=500;
    _world[2].intersect[5].x=500;
    _world[2].intersect[5].y=400;
    _world[2].intersect[6].x=700;
    _world[2].intersect[6].y=400;
    _world[2].intersect[7].x=700;
    _world[2].intersect[7].y=300;
    _world[2].intersect[8].x=0;
    _world[2].intersect[8].y=0;
    _world[2].intersect[9].x=0;
    _world[2].intersect[9].y=0;
    _world[2].intersect[10].x=0;
    _world[2].intersect[10].y=0;
    _world[2].intersect[11].x=0;
    _world[2].intersect[11].y=0;
    _world[2].intersect[12].x=0;
    _world[2].intersect[12].y=0;
    _world[2].intersect[13].x=0;
    _world[2].intersect[13].y=0;
    _world[2].intersect[14].x=0;
    _world[2].intersect[14].y=0;
    world_hung_rect.x=0;world_hung_rect.y=0;
    world_hung_rect.h=80;world_hung_rect.w=60;
    settings_rect.x=1000;settings_rect.y=550;
    settings_rect.h=200;settings_rect.w=150;
}
void adjustenter(position now)
{
    if(now.level==0){return;}
    for(int i=0;i<6;i++)
    {
        if(_world[now._escape].spot[i].x==now.x && _world[now._escape].spot[i].y==now.y)
        {
            for(int j=0;j<15;j++)
            {
                if(_world[now._escape].spot[i+1].x==_world[now._escape].intersect[j].x && _world[now._escape].spot[i+1].y==_world[now._escape].intersect[j].y)
                {
                    _world[now._escape].spot[i+1].enter=1;
                    while(!_world[now._escape].intersect[j].enter)
                    {
                        _world[now._escape].intersect[j].enter=1;
                        j--;
                    }
                    break;
                }
            }
        }
        if(i==three && now._escape==enterworld && now._escape<2)
        {
            enterworld+=1;
        }
    }
    return;
}
void initbegin()
{
    storyplot[0]=IMG_Load("images/begin0.png");
    storyplot[1]=IMG_Load("images/begin1.png");
    storyplot[2]=IMG_Load("images/begin2.png");
    storyplot[3]=IMG_Load("images/begin3.png");
    storyplot[4]=IMG_Load("images/begin4.png");
    storyplot[5]=IMG_Load("images/begin5.png");
}
void initend()
{
    _storyplot[0]=IMG_Load("images/end0.png");
    _storyplot[1]=IMG_Load("images/end1.png");
    _storyplot[2]=IMG_Load("images/end2.png");
    _storyplot[3]=IMG_Load("images/end3.png");
    _storyplot[4]=IMG_Load("images/end4.png");
    _storyplot[5]=IMG_Load("images/end5.png");
}
void begin_story()
{
    for(int i=0;i<6;i++)
    {
        beginstory_texture=SDL_CreateTextureFromSurface(world_renderer,storyplot[i]);//背景
        SDL_RenderCopy(world_renderer,beginstory_texture,NULL,NULL);
        SDL_RenderPresent(world_renderer);
        SDL_DestroyTexture(beginstory_texture);
        beginstory_texture=NULL;
        int b=1;
        while(b)
        {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else if( e.type == SDL_KEYDOWN )
            {
                if(e.key.keysym.sym==SDLK_SPACE)
                {
                    b=0;
                    break;
                }
                else if(e.key.keysym.sym==SDLK_ESCAPE)
                {
                    return;
                }
            }
        }
        }
    }
}
void end_story()
{
    for(int i=0;i<6;i++)
    {
        beginstory_texture=SDL_CreateTextureFromSurface(world_renderer,_storyplot[i]);//背景
        SDL_RenderCopy(world_renderer,endstory_texture,NULL,NULL);
        SDL_RenderPresent(world_renderer);
        SDL_DestroyTexture(endstory_texture);
        endstory_texture=NULL;
        int b=1;
        while(b)
        {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else if( e.type == SDL_KEYDOWN )
            {
                if(e.key.keysym.sym==SDLK_SPACE)
                {
                    break;
                }
                else if(e.key.keysym.sym==SDLK_ESCAPE)
                {
                    return;
                }
            }
        }
        }
    }
}
