#include<SDL2/SDL.h>
#include<iostream>
#include<stdio.h>
#include<SDL2/SDL_image.h>
using namespace std;
SDL_Window* window=NULL;//���ӭn�R��
SDL_Event  e;//���ӭn�R��
bool quit;//���ӭn�R��
SDL_Surface* hungyun=NULL;
SDL_Surface* spotcircle=NULL;
SDL_Surface* settingssurface=NULL;
SDL_Renderer* world_renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
SDL_Rect world_rect,world_hung_rect,settings_rect;
SDL_Texture* background_texture=NULL;
SDL_Texture* hung_texture=NULL;
SDL_Texture* spotcircle_texture=NULL;
SDL_Texture* settings_texture=NULL;
int enterworld=0;
bool sett=0;
struct position
{
    int x;//�y��
    int y;//�y��
    int level;//�������d
    bool _end;//�O���O�̫�@��spot(�i���U�@�ӥ@��)
    bool _start;//�O���O�Ĥ@��spot(�i���W�@�ӥ@��)
    int _escape;//�ثe�Ҧb�@��(��̧ک_�Ǫ��ܼƦW)
    bool esc;//���S����settings�����@��
    bool enter;//�i���i�H�i�J(�e�����d���S���}��)
};
enum enterspot{help,start,one,two,three,ending};
class world
{
    public:
    position spot[6],character,intersect[15];//�w�ק�,character����l��m�A�����
    SDL_Surface* world_background;
    bool _help,_ending;
    void scene(position* now)//�w�ק�
    {
        SDL_RenderClear(world_renderer);
        background_texture=SDL_CreateTextureFromSurface(world_renderer,world_background);//�I��
        SDL_RenderCopy(world_renderer,background_texture,NULL,&world_rect);//�I��
        drawroute();//line
        drawspot();//spot
        world_hung_rect.x=now->x;world_hung_rect.y=now->y;//��
        hung_texture=SDL_CreateTextureFromSurface(world_renderer,hungyun);//��
        SDL_RenderCopy(world_renderer,hung_texture,NULL,&world_hung_rect);//��
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
        return ;
    }
    void _move(position &now)//�w�ק�
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
                                {
                                    animation(spot[i],spot[i-1],now);
                                    now=spot[i-1];
                                }
                                break;
                            case SDLK_UP:
                                if(i<three || (i==three && _ending==1))
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
                    while((now.y+10)<intersect[i+1].y)
                    {
                        now.y+=10;
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
                    while((now.y-10)>intersect[i-1].y)
                    {
                        now.y-=10;
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
                            return 0;
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
                        if(op==-2)settingssurface=IMG_Load("images/set0--2");
                        if(op==-1)settingssurface=IMG_Load("images/set0--1");
                        if(op==0)settingssurface=IMG_Load("images/set0-0");
                    }
                    else if(enterworld==1)
                    {
                        if(op==-2)settingssurface=IMG_Load("images/set1--2");
                        if(op==-1)settingssurface=IMG_Load("images/set1--1");
                        if(op==0)settingssurface=IMG_Load("images/set1-0");
                        if(op==0)settingssurface=IMG_Load("images/set1-1");
                    }
                    else if(enterworld==2)
                    {
                        if(op==-2)settingssurface=IMG_Load("images/set2--2");
                        if(op==-1)settingssurface=IMG_Load("images/set2--1");
                        if(op==0)settingssurface=IMG_Load("images/set2-0");
                        if(op==0)settingssurface=IMG_Load("images/set2-1");
                        if(op==0)settingssurface=IMG_Load("images/set2-2");
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
world _world[3];

int main(int argc, char* argv[])
{
    window=SDL_CreateWindow("world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,1200, 800, SDL_WINDOW_SHOWN );
    //��l��world
    position now=_world[0].spot[start];
    while(!quit)
    {
        _world[now._escape].scene(&now);
        _world[now._escape]._move(now);
        if(now.level!=-1)
        {
            //�I�s���d(�p�GĹ�F�I�sadjustenter(now))
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
                //����welcomesurface
            }
            else if(now._escape==0 || now._escape==1 ||now._escape==2)
            {
                now=_world[now._escape].spot[start];
            }
        }
    }
    return 0;
}
void adjustenter(position now)
{
    if(now.level==0)return;
    for(int i=0;i<6;i++)
    {
        if(_world[now._escape].spot[i].x==now.x && _world[now._escape].spot[i].y==now.y)
        {
            for(int j=0;j<15;j++)
            {
                if(_world[now._escape].spot[i+1].x==_world[now._escape].intersect[j].x && _world[now._escape].spot[i+1].y==_world[now._escape].intersect[j].y)
                {
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
        break;
    }
}
