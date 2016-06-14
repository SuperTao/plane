
#include "plane.h"

extern GAME_ENGINE game_engine;
extern PLANE player; 
extern PLANE background; 
extern PLANE begin_background; 
extern PLANE number[10]; 
extern PLANE bullet[MAX_BULLET];
extern PLANE small_enemy[MAX_SMALL_ENEMY];
//每个中型飞机对应一个结构体变量
extern PLANE mid_enemy[MAX_MID_ENEMY];
//每个大型飞机对应一个结构体变量
extern PLANE big_enemy[MAX_BIG_ENEMY];

void init_game()
{
	init_SDL();
	init_rand();
	init_timer();
	init_joystick();
	init_player(&player);
	init_begin_background(&begin_background);
	init_background(&background);
	init_bullet(bullet);
	init_small_enemy(small_enemy);
	init_mid_enemy(mid_enemy);
	init_big_enemy(big_enemy);
	init_number(number);
}

void init_SDL()
{
	beginSDL(SDL_INIT_VIDEO);
	game_engine.window = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BPP, SDL_SWSURFACE);
	if(NULL == game_engine.window)
	{
		perror("SDL_SetVideoMode fail\n");
	}
	//设备窗体的标题及图标
	SDL_WM_SetCaption("PLANE WARS",NULL); 
	game_engine.picture = load_image(game_engine.picture, "./pic/picture.png");
	//开始界面的背景
	game_engine.begin = load_image(game_engine.begin, "./pic/begin_background.png");
	//正常游戏背景
	game_engine.background = load_image(game_engine.background, "./pic/background.png");
	//加载显示数字的图片
	game_engine.score = IMG_Load("./pic/score.png");
	if(NULL == game_engine.score)	
	{
		perror("load picture fail\n");
	}
	//除去截取到的图片周围的空白
	SDL_SetColorKey (game_engine.score, SDL_SRCCOLORKEY, SDL_MapRGB(game_engine.score->format, 0, 0, 0));
	//格式化从新输出
	game_engine.score = SDL_DisplayFormat(game_engine.score);
	//===========joystick==================
	SDL_Init(SDL_INIT_EVERYTHING);
	//设置窗口大小和分辨率
	SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BPP, SDL_SWSURFACE);
}

void init_rand()
{ //使用时间作为随机数种子，获取坐标的时候用到
	srand(time(NULL)); 
}

void init_timer()
{
	//定时器设置
	set_timer(5);
	signal(SIGALRM, timer);
}

void init_begin_background(PLANE *background)
{
	background->src_rect[0].x = 0;
	background->src_rect[0].y = 0;
	background->src_rect[0].w = SCREEN_WIDTH;
	background->src_rect[0].h = SCREEN_HEIGHT;
	background->dst_rect.x = 0;
	background->dst_rect.y = 0;
}

void init_background(PLANE *background)
{
	background->src_rect[0].x = 0;
	background->src_rect[0].y = 560;
	background->src_rect[0].w = SCREEN_WIDTH;
	background->src_rect[0].h = SCREEN_HEIGHT;

	background->dst_rect.x = 0;
	background->dst_rect.y = 0;
}

void init_player(PLANE *player)
{
	player->pic_num = 4;
	player->pic_index = 0;

	player->src_rect[0].x = 431;
	player->src_rect[0].y = 0;
	player->src_rect[0].w = 65;
	player->src_rect[0].h = 80;

	player->src_rect[1].x = 431;
	player->src_rect[1].y = 249;
	player->src_rect[1].w =	65;
	player->src_rect[1].h = 80;

	player->src_rect[2].x = 431;
	player->src_rect[2].y = 83;
	player->src_rect[2].w = 65;
	player->src_rect[2].h = 80;

	player->src_rect[3].x = 431;
	player->src_rect[3].y = 166;
	player->src_rect[3].w = 65;
	player->src_rect[3].h = 80;

	player->dst_rect.x = 130;
	player->dst_rect.y = 480;

	player->speed_x = 0;
	player->speed_y = 0;

	player->life = 1;
}

void init_bullet(PLANE *bullet)
{
	int i;
	for(i = 0; i < MAX_BULLET; i++)
	{
		bullet[i].pic_num = 0;
		bullet[i].pic_index = 0;
	
		bullet[i].src_rect[0].x = 498;
		bullet[i].src_rect[0].y = 0;
		bullet[i].src_rect[0].w = 8;
		bullet[i].src_rect[0].h = 15;

		bullet[i].dst_rect.x = 0;
		bullet[i].dst_rect.y = 0;

		bullet[i].life = 0;
	}
}

void init_small_enemy(PLANE *small_enemy)
{
	int i;
	for(i = 0; i < MAX_SMALL_ENEMY; i++)
	{
		small_enemy[i].pic_num = 4;
		small_enemy[i].pic_index = 0;

		small_enemy[i].src_rect[0].x = 84;
		small_enemy[i].src_rect[0].y = 658;
		small_enemy[i].src_rect[0].w = 32;
		small_enemy[i].src_rect[0].h = 23;

		small_enemy[i].src_rect[1].x = 48;
		small_enemy[i].src_rect[1].y = 657;
		small_enemy[i].src_rect[1].w = 32;
		small_enemy[i].src_rect[1].h = 23;

		small_enemy[i].src_rect[2].x = 420;
		small_enemy[i].src_rect[2].y = 732;
		small_enemy[i].src_rect[2].w = 37;
		small_enemy[i].src_rect[2].h = 28;

		small_enemy[i].src_rect[3].x = 473;
		small_enemy[i].src_rect[3].y = 718;
		small_enemy[i].src_rect[3].w = 40;
		small_enemy[i].src_rect[3].h = 40;

		small_enemy[i].dst_rect.x = 0;
		small_enemy[i].dst_rect.y = 0;
		small_enemy[i].life = 0;
	}
}

void init_mid_enemy(PLANE *mid_enemy)
{
	int i;
	for(i = 0; i < MAX_MID_ENEMY; i++)
	{
		mid_enemy[i].pic_num = 5;
		mid_enemy[i].pic_index = 0;

		mid_enemy[i].src_rect[0].x = 0;
		mid_enemy[i].src_rect[0].y = 569;
		mid_enemy[i].src_rect[0].w = 46;
		mid_enemy[i].src_rect[0].h = 59;

		mid_enemy[i].src_rect[1].x = 432;
		mid_enemy[i].src_rect[1].y = 414;
		mid_enemy[i].src_rect[1].w = 46;
		mid_enemy[i].src_rect[1].h = 62;

		mid_enemy[i].src_rect[2].x = 432;
		mid_enemy[i].src_rect[2].y = 538;
		mid_enemy[i].src_rect[2].w = 46;
		mid_enemy[i].src_rect[2].h = 59;

		mid_enemy[i].src_rect[3].x = 432;
		mid_enemy[i].src_rect[3].y = 600;
		mid_enemy[i].src_rect[3].w = 46;
		mid_enemy[i].src_rect[3].h = 59;

		mid_enemy[i].src_rect[4].x = 432;
		mid_enemy[i].src_rect[4].y = 478;
		mid_enemy[i].src_rect[4].w = 46;
		mid_enemy[i].src_rect[4].h = 58;

		mid_enemy[i].dst_rect.x = 0;
		mid_enemy[i].dst_rect.y = 0;

		mid_enemy[i].life = 0;
	}
}

void init_big_enemy(PLANE *big_enemy)
{
	int i;
	for(i = 0; i < MAX_BIG_ENEMY; i++)
	{
		big_enemy[i].pic_num = 7;
		big_enemy[i].pic_index = 0;
		big_enemy[i].src_rect[0].x = 222;
		big_enemy[i].src_rect[0].y = 858;
		big_enemy[i].src_rect[0].w = 108;
		big_enemy[i].src_rect[0].h = 165;

//		big_enemy[i].src_rect[1].x = 112;
//		big_enemy[i].src_rect[1].y = 736;
//		big_enemy[i].src_rect[1].w = 108;
//		big_enemy[i].src_rect[1].h = 165;

		big_enemy[i].src_rect[1].x = 310;
		big_enemy[i].src_rect[1].y = 686;
		big_enemy[i].src_rect[1].w = 109;
		big_enemy[i].src_rect[1].h = 170;

		big_enemy[i].src_rect[2].x = 4;
		big_enemy[i].src_rect[2].y = 754;
		big_enemy[i].src_rect[2].w = 106;
		big_enemy[i].src_rect[2].h = 170;

		big_enemy[i].src_rect[3].x = 325;
		big_enemy[i].src_rect[3].y = 170;
		big_enemy[i].src_rect[3].w = 107;
		big_enemy[i].src_rect[3].h = 170;

		big_enemy[i].src_rect[4].x = 325;
		big_enemy[i].src_rect[4].y = 515;
		big_enemy[i].src_rect[4].w = 107;
		big_enemy[i].src_rect[4].h = 170;

		big_enemy[i].src_rect[5].x = 325;
		big_enemy[i].src_rect[5].y = 340;
		big_enemy[i].src_rect[5].w = 107;
		big_enemy[i].src_rect[5].h = 170;
		
		big_enemy[i].src_rect[6].x = 325;
		big_enemy[i].src_rect[6].y = 0;
		big_enemy[i].src_rect[6].w = 107;
		big_enemy[i].src_rect[6].h = 170;

		big_enemy[i].dst_rect.x = 0;
		big_enemy[i].dst_rect.y = 0;

		big_enemy[i].life = 0;
	}
}
void init_number(PLANE *number)
{
	number[0].pic_num = 1;
	number[0].pic_index = 0;
	number[0].src_rect[0].x = 32;
	number[0].src_rect[0].y = 0;
	number[0].src_rect[0].w = 16;
	number[0].src_rect[0].h = 32;
	number[0].dst_rect.x = 0;
	number[0].dst_rect.y = 0;
	number[0].life = 1;

	number[1].pic_num = 1;
	number[1].pic_index = 0;
	number[1].src_rect[0].x = 48;
	number[1].src_rect[0].y = 0;
	number[1].src_rect[0].w = 16;
	number[1].src_rect[0].h = 32;
	number[1].dst_rect.x = 0;
	number[1].dst_rect.y = 0;
	number[1].life = 1;

	number[2].pic_num = 1;
	number[2].pic_index = 0;
	number[2].src_rect[0].x = 64;
	number[2].src_rect[0].y = 0;
	number[2].src_rect[0].w = 16;
	number[2].src_rect[0].h = 32;
	number[2].dst_rect.x = 0;
	number[2].dst_rect.y = 0;
	number[2].life = 1;

	number[3].pic_num = 1;
	number[3].pic_index = 0;
	number[3].src_rect[0].x = 80;
	number[3].src_rect[0].y = 0;
	number[3].src_rect[0].w = 16;
	number[3].src_rect[0].h = 32;
	number[3].dst_rect.x = 0;
	number[3].dst_rect.y = 0;
	number[3].life = 1;


	number[4].pic_num = 1;
	number[4].pic_index = 0;
	number[4].src_rect[0].x = 96;
	number[4].src_rect[0].y = 0;
	number[4].src_rect[0].w = 16;
	number[4].src_rect[0].h = 32;
	number[4].dst_rect.x = 0;
	number[4].dst_rect.y = 0;
	number[4].life = 1;

	number[5].pic_num = 1;
	number[5].pic_index = 0;
	number[5].src_rect[0].x = 112;
	number[5].src_rect[0].y = 0;
	number[5].src_rect[0].w = 16;
	number[5].src_rect[0].h = 32;
	number[5].dst_rect.x = 0;
	number[5].dst_rect.y = 0;
	number[5].life = 1;

	number[6].pic_num = 1;
	number[6].pic_index = 0;
	number[6].src_rect[0].x = 128;
	number[6].src_rect[0].y = 0;
	number[6].src_rect[0].w = 16;
	number[6].src_rect[0].h = 32;
	number[6].dst_rect.x = 0;
	number[6].dst_rect.y = 0;
	number[6].life = 1;

	number[7].pic_num = 1;
	number[7].pic_index = 0;
	number[7].src_rect[0].x = 144;
	number[7].src_rect[0].y = 0;
	number[7].src_rect[0].w = 16;
	number[7].src_rect[0].h = 32;
	number[7].dst_rect.x = 0;
	number[7].dst_rect.y = 0;
	number[7].life = 1;

	number[8].pic_num = 1;
	number[8].pic_index = 0;
	number[8].src_rect[0].x = 160;
	number[8].src_rect[0].y = 0;
	number[8].src_rect[0].w = 16;
	number[8].src_rect[0].h = 32;
	number[8].dst_rect.x = 0;
	number[8].dst_rect.y = 0;
	number[8].life = 1;

	number[9].pic_num = 1;
	number[9].pic_index = 0;
	number[9].src_rect[0].x = 176;
	number[9].src_rect[0].y = 0;
	number[9].src_rect[0].w = 16;
	number[9].src_rect[0].h = 32;
	number[9].dst_rect.x = 0;
	number[9].dst_rect.y = 0;
	number[9].life = 1;
}
