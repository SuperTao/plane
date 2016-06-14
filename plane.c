
#include "plane.h" 

//每个子弹对应一个结构体变量
PLANE bullet[MAX_BULLET] = {0};
//每个小型飞机对应一个结构体变量
PLANE small_enemy[MAX_SMALL_ENEMY] = {0};
//每个中型飞机对应一个结构体变量
PLANE mid_enemy[MAX_MID_ENEMY] = {0};
//每个大型飞机对应一个结构体变量
PLANE big_enemy[MAX_BIG_ENEMY] = {0};
//开始界面的小飞机
PLANE little_plane = {0};
//保存玩家信息
PLANE player; 
//保存背景信息
PLANE background; 
//保存开始背景的信息
PLANE begin_background; 
//存数字的图片的数组
PLANE number[10] = {0}; 
//保存分数
int score_record = 0;
//将分数转成字符串之后保存在这个数组里
//预留了8位
char score_arr[8] = {0};
//保存分数字符串的长度
int score_length = 0;
//统计在屏幕中子弹的个数
int bullet_num = 0;
//自动射击标志
//int auto_shot_flag = 0;
//一些变量保存在结构体中
GAME_ENGINE game_engine;
//事件结构体
SDL_Event event; 
int game_state = 0;
//加载图片
extern int count_2;
SDL_Joystick *stick = NULL;
//SDL_Event event ;

int main(int argc, char *argv[])
{
	while(game_state != GAME_EXIT)
	{
		switch (game_state)
		{
			case GAME_START:
				init_game();
				game_state = GAME_MENU;
				break;
			case GAME_MENU:
				game_menu();
				break;
			case GAME_RUNNING:
				game_running();
				break;
		}
		printf("hello\n");
	}
	close_joystick();
	endSDL(SDL_INIT_VIDEO);
}

void show_score()
{
	int i, j;
	//将整形转为字符串存到数组中,并得到字符串的长度
	score_length = sprintf(score_arr, "%d", score_record);
	//判断字符串中每个元素的哪个数字并进行输出
	for(i = 0; i < score_length; i++)
	{
		j = *(score_arr+i)-'0';
		//在屏幕显示的坐标移动
		number[j].dst_rect.x += 16*i;
		//显示
		SDL_BlitSurface(game_engine.score, &(number[j].src_rect[0]), game_engine.window, &(number[j].dst_rect)); 
		//坐标清零，方便下次移动
		number[j].dst_rect.x = 0;
	}
}

//开始SDL
void beginSDL(int flag)
{
	//检查SDL是否运行，返回非0为已经运行
	if(SDL_WasInit(flag) != 0)
	{
		fprintf(stderr, "SDL is running!\n");
		return;
	}
	//转载SDL库，返回－1为失败
	if(SDL_Init(flag) == -1)
	{
		fprintf(stderr, "unable to init SDL!\n");
		return;
	}
	return;
}
//关闭SDL
void endSDL(int flag)
{
	//检查SDL是否运行，返回非0为已经运行
	if(SDL_WasInit(flag) == 0)
	{
		fprintf(stderr, "SDL is not running!\n");
		return;
	}
	//退出
	SDL_Quit();
	return;
}

//加载图片
SDL_Surface *load_image(SDL_Surface *image, char *file)
{
	image = IMG_Load(file);
	if(NULL == image)	
	{
		perror("load picture fail\n");
		return NULL;
	}
	//除去截取到的图片周围的空白
	SDL_SetColorKey (image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 255, 255));
	//按窗口对象重新格式化输出
	image = SDL_DisplayFormat(image);
	return image;
}

//发射子弹
void shot_bullet(PLANE *player, PLANE *bullet)
{
	int i, j;
	bullet_num++;
	if(bullet_num <= MAX_BULLET)
	{
		for(i = 0; i < MAX_BULLET; i++)
		{
			if(bullet[i].life == 0)
			{
				bullet[i].life = 1;
				bullet[i].dst_rect.x = player->dst_rect.x + 28;
				bullet[i].dst_rect.y = player->dst_rect.y - 20;
				break;
			}
		}
	}
	else if(bullet_num > MAX_BULLET)
		bullet_num = MAX_BULLET;
	else if(bullet_num < 1)
		bullet_num = 0;
}
//显示一个敌人
void show_one_element(GAME_ENGINE *game_engine, PLANE *element, int i)
{
		SDL_BlitSurface(game_engine->picture, &(element[i].src_rect[element[i].pic_index]), game_engine->window, &(element[i].dst_rect)); 
}
//显示多个敌人
void show_all_element(GAME_ENGINE *game_engine, PLANE *element, int max_element)
{
	int i;
	for(i = 0; i < max_element; i++)
	{
		show_one_element(game_engine, element, i);
	}
}
//检查边缘
void check_edge(PLANE *player)
{
	if(player->dst_rect.x < 0)
		player->dst_rect.x = 0;
	else if(player->dst_rect.x > (SCREEN_WIDTH - player->src_rect[0].w))
		player->dst_rect.x = (SCREEN_WIDTH - player->src_rect[0].w);
	else if(player->dst_rect.y < 0)
		player->dst_rect.y = 0;
	else if(player->dst_rect.y > (SCREEN_HEIGHT - player->src_rect[0].h))
		player->dst_rect.y = SCREEN_HEIGHT - player->src_rect[0].h;
}
//从新给敌人新的坐标和命
void get_enemy_pos(PLANE *enemy, int life)
{
	int i = 0;
	//死亡，显示完爆炸效果之后，即图片索引为0
	//从新给生命，坐标
	while(enemy->life == 0 && enemy->pic_index == 0)
	{
		enemy->life = life;
		enemy->dst_rect.x = rand()%(SCREEN_WIDTH - enemy->src_rect[0].w);
		//防止随机坐标太靠边上
		if(enemy->dst_rect.x < 10 || enemy->dst_rect.x > (SCREEN_WIDTH-enemy->src_rect[0].w-10))
			continue;
		enemy->dst_rect.y = 2;
	}
}
//检查子弹是否射击到敌人
int check_shot_enemy(PLANE *enemy,  int max_enemy, PLANE *bullet, int max_bullet)
{
	int i, j;
	for(i = 0; i < max_bullet; i++)
	{
		for(j = 0; j < max_enemy; j++)
		{
			if(bullet[i].life == 1 && enemy[j].life > 0 && enemy[i].pic_index == 0)
			{
				//x方向判断
				if((enemy[j].dst_rect.y+enemy[j].src_rect[0].h) > bullet[i].dst_rect.y) 
				{
					//y方向判断
					if( (bullet[i].dst_rect.x < (enemy[j].dst_rect.x+enemy[j].src_rect[0].w) && ((bullet[i].dst_rect.x+bullet[i].src_rect[0].w) > enemy[j].dst_rect.x)))
					{
						bullet_num -= 1;
						bullet[i].life -= 1;
						enemy[j].pic_index++;
						enemy[j].life -= 1;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int check_collision(PLANE *enemy,  int max_enemy, PLANE *player)
{
	int j;
	for(j = 0; j < max_enemy; j++)
	{
		if(player->life == 1 && enemy[j].life > 0 && enemy[j].pic_index == 0)
		{
			//y方向判断
			if(((enemy[j].dst_rect.y+enemy[j].src_rect[0].h*2/3) > player->dst_rect.y) && (enemy[j].dst_rect.y < (player->dst_rect.y+player->src_rect[0].h*1/2))) 
			{
				//x方向判断
				if( (player->dst_rect.x < (enemy[j].dst_rect.x+enemy[j].src_rect[0].w*2/3) && ((player->dst_rect.x+player->src_rect[0].w) > (enemy[j].dst_rect.x+enemy[j].src_rect[0].w/3))))
				{
					//生命减少
					player->life -= 1;
					//图片索引增加
					player->pic_index++;
					return 1;
				}
			}
		}
	}
	return 0;
}

void show_little_plane()
{
	//开始界面，小飞机的动态效果
	SDL_BlitSurface(game_engine.begin, &begin_background.src_rect[0], game_engine.window, &begin_background.dst_rect); 
	//刷新显示
	count_2++;
	if(count_2 == 60)
	{
		little_plane.src_rect[0].x = 86;
		little_plane.src_rect[0].y = 710;
		little_plane.src_rect[0].w = 40;
		little_plane.src_rect[0].h = 26;
		little_plane.dst_rect.x = 100;
		little_plane.dst_rect.y = 280;
	}
	else if(count_2 == 120)
	{
		little_plane.src_rect[0].x = 48;
		little_plane.src_rect[0].y = 710;
		little_plane.src_rect[0].w = 80;
		little_plane.src_rect[0].h = 26;
		little_plane.dst_rect.x = 100;
		little_plane.dst_rect.y = 280;
	}
	else if(count_2 == 180)
	{
		little_plane.src_rect[0].x = 0;
		little_plane.src_rect[0].y = 685;
		little_plane.src_rect[0].w = 122;
		little_plane.src_rect[0].h = 26;
		little_plane.dst_rect.x = 100;
		little_plane.dst_rect.y = 280;
	}
	//0-100不显示
	else if(count_2 == 240)
	{
		count_2 = 0;
	}
	if(count_2 > 60)
		SDL_BlitSurface(game_engine.picture, &little_plane.src_rect[0], game_engine.window, &little_plane.dst_rect); 
	SDL_Flip(game_engine.window);

}

