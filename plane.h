
#ifndef _PLANE_H
#define _PLANE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

#include <SDL/SDL.h>		//-lSDL
#include <SDL/SDL_image.h>	//-lSDL_image
#include <SDL/SDL_ttf.h>	//-lSDL_ttf.h
#include <SDL/SDL_mixer.h>	//-lSDL_mixer.h

//窗口大小
#define     SCREEN_WIDTH	320 
#define     SCREEN_HEIGHT   560
//分辨率
#define     BPP             32


#define     MAX_BULLET      6
#define     MAX_SMALL_ENEMY	6 
#define     MAX_MID_ENEMY   2
#define     MAX_BIG_ENEMY   1

enum {
	GAME_START,
	GAME_MENU,
	GAME_RUNNING,
	GAME_OVER,
	GAME_EXIT,
};

typedef struct GameEngine
{
	SDL_Surface *window;
	SDL_Surface	*background;
	SDL_Surface	*picture;
	SDL_Surface	*begin;
	SDL_Surface	*score;
}GAME_ENGINE;

typedef struct plane
{
	SDL_Rect src_rect[10];  //保存中枪以及正常时候的图片信息
	SDL_Rect dst_rect;
	int pic_num;		//图片数量，从1开始
	int pic_index;		//图片索引，用于查找图片，从0开始
	int life;			//表示右几条命，0代表死亡
	int speed_x;		//飞机移动x方向的速度
	int speed_y;		//飞机移动y方向的速度
}PLANE;

void beginSDL(int flag);

void endSDL(int flag);

void game_menu();
void game_running();
void timer(int num);
//显示分数
void show_score();

//初始化每个部分的坐标，不同状态显示效果不一样，
//例如子弹被射击之后，显示多长爆炸效果
void init_game();
void init_SDL();
void init_rand();
void init_timer();
void init_begin_background(PLANE *background);
void init_background(PLANE *background);
void init_player(PLANE *player);
void init_bullet(PLANE *bullet);
void init_small_enemy(PLANE *small_enemy);
void init_mid_enemy(PLANE *mid_enemy);
void init_big_enemy(PLANE *big_enemy);
//初始化从图上获取的数字图片的坐标
void init_number(PLANE *number);
//发射子弹
void shot_bullet(PLANE *player, PLANE *bullet);
void show_one_element(GAME_ENGINE *game_engine, PLANE *element, int i);
void show_all_element(GAME_ENGINE *game_engine, PLANE *element, int max_element);

//检测飞机是否到边框
void check_edge(PLANE *player);

//从新给敌人新的坐标和命
//enemy:敌人
//life:生命
void get_enemy_pos(PLANE *enemy, int life);
int  check_shot_enemy(PLANE *enemy, int max_enemy, PLANE *bullet, int max_bullet );
int check_collision(PLANE *enemy,  int max_enemy, PLANE *player);
void move_bullet(PLANE *bullet);
void show_little_plane();

SDL_Surface *load_image(SDL_Surface *image, char *file);
//----------joystick-----------------
//初始化
void init_joystick();
//按键按下
void joy_button_down();
//按键松开
void joy_button_up();
//摇杆控制
void joy_axis_motion();

void joy_hat_motion();

void close_joystick();
//ball功能没有检测到
void joy_ball_motion();

#endif  //_PLANE_H
