
#include "plane.h"

extern PLANE plane;
extern PLANE bullet[MAX_BULLET];
extern GAME_ENGINE game_engine;
extern int bullet_num;
extern int score_record;
extern PLANE background; 
extern PLANE begin_background; 
extern int auto_shot_flag;
extern PLANE player; 
extern PLANE small_enemy[MAX_SMALL_ENEMY];
extern PLANE mid_enemy[MAX_MID_ENEMY];
extern PLANE big_enemy[MAX_BIG_ENEMY];
extern char score_arr[10];
extern int score_length;
extern PLANE number[10]; 
extern int game_state;
extern int count_1;
int time_count = 0;
//extern int count_2;

void timer(int num)
{
	//不能在定时器里刷新屏幕，频率太高会显示不出来
	if(game_state == GAME_RUNNING)
	{

		time_count++;		//用于控制运动的速度
		//背景移动
		count_1++;
		if(count_1 == 4)
		{
			count_1 = 0;
			background.src_rect[0].y--;
			if(background.src_rect[0].y == 0)
				background.src_rect[0].y = 560;
		}

	//飞机移动，通过按键控制固定的速度
		player.dst_rect.y += player.speed_y;
		player.dst_rect.x += player.speed_x;
		//检查飞机是否碰到边缘
		check_edge(&player);
		//子弹运动
		move_bullet(bullet);
	
		//敌人自动向下运动
		int i, j;
		if(time_count%2 == 0)
		{
			for(i = 0; i < MAX_SMALL_ENEMY; i++)
			{
		 		if(small_enemy[i].life > 0 && small_enemy[i].pic_index == 0)
				{
					small_enemy[i].dst_rect.y += 2;
				}
				//敌人死亡之后，显示爆炸效果的图片
				else if(small_enemy[i].life == 0 && small_enemy[i].pic_index != 0)
		  		{	
						if(time_count%4 == 0)
						{
							//显示被射击之后的图像效果
							small_enemy[i].pic_index++;
							//图像效果显示完之后，消失
							if(small_enemy[i].pic_index == small_enemy[i].pic_num)
							{
								small_enemy[i].pic_index = 0;
								//分数增加
								score_record += 100;
							}
						}
				}
				//到底端消失
				if(small_enemy[i].dst_rect.y > SCREEN_HEIGHT)
				{
					small_enemy[i].life = 0;
				}
			}
		}
		//---------------中型飞机---------------
		if(time_count%4 == 0)
		{
			for(i = 0; i < MAX_MID_ENEMY; i++)
			{
		 		if(mid_enemy[i].life > 0 && mid_enemy[i].pic_index == 0)
				{
					mid_enemy[i].dst_rect.y += 2;
				}
				else if(mid_enemy[i].life >= 0 && mid_enemy[i].pic_index != 0)
		  		{	
						if(time_count%8 == 0)
						{
							//显示被射击之后的图像效果
							if(mid_enemy[i].pic_index == 1 && mid_enemy[i].life > 0)
							{
								mid_enemy[i].pic_index = 0;
								mid_enemy[i].dst_rect.y += 2;
							}
							else if(mid_enemy[i].pic_index >= 1 && mid_enemy[i].life == 0)
								mid_enemy[i].pic_index++;
							//图像效果显示完之后，消失
							if(mid_enemy[i].pic_index == mid_enemy[i].pic_num)
							{
								mid_enemy[i].pic_index = 0;						
								score_record += 300;
							}
						}
				}
				//到底端消失
				if(mid_enemy[i].dst_rect.y > SCREEN_HEIGHT)
				{
					mid_enemy[i].life = 0;
				}
			}
		}
		//--------------------大飞机-----------------------
		if(time_count%4 == 0)
		{
			for(i = 0; i < MAX_BIG_ENEMY; i++)
			{
		 		if(big_enemy[i].life > 0 && big_enemy[i].pic_index == 0)
				{
					big_enemy[i].dst_rect.y += 2;
				}
				else if(big_enemy[i].life >= 0 && big_enemy[i].pic_index != 0)
		  		{	
						if(time_count%8 == 0)
						{
							//显示被射击之后的图像效果
							if(big_enemy[i].pic_index == 1 && big_enemy[i].life > 0)
							{
	
								big_enemy[i].pic_index = 0;
								big_enemy[i].dst_rect.y += 2;
							}
							//死亡后显示被射击的效果
							else if(big_enemy[i].pic_index >= 1 && big_enemy[i].life == 0)
								big_enemy[i].pic_index++;
							//图像效果显示完之后，消失
							if(big_enemy[i].pic_index == big_enemy[i].pic_num)
							{
								big_enemy[i].pic_index = 0;
								score_record += 600;
							}
						}
				}
				//到底端消失
				if(big_enemy[i].dst_rect.y > SCREEN_HEIGHT)
				{
					big_enemy[i].life = 0;
				}
			}
		}
		//将分数转化为字符串,并进行显示	
		show_score();
	
		//玩家的飞机
		if(time_count%2 == 0)
		{
			//死亡后显示爆炸效果
			if(player.life == 0 && player.pic_index != 0)
	  		{	
				if(time_count%4 == 0)
				{
					//显示被射击之后的图像效果
					player.pic_index++;
					//图像效果显示完之后，消失
				}
			}
			//到底端消失
			if(player.dst_rect.y > (SCREEN_HEIGHT-player.src_rect[0].h))
			{
				player.life = 0;
				player.dst_rect.y = 0;
			}
		}
		//飞机飞行动态	
		if(time_count == 10)
		{
			player.src_rect[0].x = 433;
			player.src_rect[0].y = 331;
		}
		else if(time_count > 50 && time_count < 100)
		{
			player.src_rect[0].x = 433;
			player.src_rect[0].y = 0;
		}
		else if(time_count == 100)
			time_count = 0;
	
	
		//自动射击
		if(time_count %30 == 0)
		{
		//	time_count = 0;
			if(auto_shot_flag)
			{
				shot_bullet(&player, bullet);
			}
		}
	}
}

void set_timer(int ms)
{
    struct itimerval val;

    val.it_interval.tv_sec = ms/1000;
    val.it_interval.tv_usec = ms%1000*1000; 
    val.it_value.tv_sec = ms/1000;  
    val.it_value.tv_usec = ms%1000*1000;
			   
    setitimer(ITIMER_REAL, &val, NULL);
}

void move_bullet(PLANE *bullet)
{
	int i, j;
	for(i = 0; i < MAX_BULLET; i++)
	{
		if(bullet[i].life == 1)
		{
			//子弹向上移动5个像素点
			bullet[i].dst_rect.y -= 4;
			//子弹到到顶端消失
			if(bullet[i].dst_rect.y < 8)
			{
				//子弹生命变为0
				bullet[i].life = 0;
				//子弹数量减1
				bullet_num -= 1;
				continue;
			}
			//子弹生命为1才进行显示
			show_one_element(&game_engine, bullet, i);
		}
	}
}
