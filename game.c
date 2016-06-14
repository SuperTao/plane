
#include "plane.h"
extern GAME_ENGINE game_engine;
extern int game_state;
	//事件结构体
extern PLANE begin_background; 
extern PLANE background; 
extern SDL_Event event; 
extern int score_record;
extern PLANE background; 
extern PLANE bullet[MAX_BULLET];
extern PLANE small_enemy[MAX_SMALL_ENEMY];
extern PLANE mid_enemy[MAX_MID_ENEMY];
extern PLANE big_enemy[MAX_BIG_ENEMY];
extern PLANE little_plane;
extern PLANE player; 
int auto_shot_flag = 0;
int count = 0;
int count_1 = 0;
int count_2 = 0;

void game_menu()
{
	while(game_state == GAME_MENU) 
	{
		show_little_plane();
		if(SDL_PollEvent( &event))
		{
		//=========joystick==============
			if(event.type == SDL_JOYBUTTONDOWN)
			{
				if(event.jbutton.which == 0)
				{
					if(event.jbutton.button == 9)
					{
						auto_shot_flag = 0;
						//分数清零
						score_record = 0;
						//开始标志
						//从新开启定时器
						game_state = GAME_RUNNING;
						break;
					}
					if(event.jbutton.button == 8)
					{
						game_state = GAME_EXIT;
					}

				}
			}

			printf("game menu\n");
			//按键事件检测，按下时，更新相应方向的速度
			if(event.type == SDL_KEYDOWN)
			{
				printf("keyevent\n");
		//===============ENTER进入游戏===============
				if(event.key.keysym.sym == SDLK_RETURN)
				{
					//关闭自动射击
					auto_shot_flag = 0;
					//分数清零
					score_record = 0;
					//开始标志
					game_state = GAME_RUNNING;
				}
		//=================退出游戏====================
				if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					game_state = GAME_EXIT;
				}
			}
		//=============鼠标关系游戏窗口=================
			if(event.type == SDL_QUIT )
				game_state = GAME_EXIT;
		}
		//防止程序一直在检测事件
		SDL_Delay(10);
	}
}

void game_running()
{
	while(game_state == GAME_RUNNING)
	{
//============开始游戏==============
		printf("game_running\n");
		//显示背景
		SDL_BlitSurface(game_engine.background, &background.src_rect[0], game_engine.window, &background.dst_rect); 

		//-------------按键--------------
		if(SDL_PollEvent( &event))
		{
			//---------joystick------------------
			joy_button_down();
			joy_button_up();
		 	joy_axis_motion();
	//按键事件检测，按下时，更新相应方向的速度
			if(event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_w: player.speed_y = -2;break;
					case SDLK_s: player.speed_y = 2;break;
					case SDLK_a: player.speed_x = -2;break;
					case SDLK_d: player.speed_x = 2;break;
					case SDLK_u:		//自动射击 
								 auto_shot_flag = 1;
								 break;
					case SDLK_j:		//手动射击 
								 auto_shot_flag = 0;
								 shot_bullet(&player, bullet);
								 break;
				}
				if(event.key.keysym.sym == SDLK_ESCAPE)
					game_state = GAME_START;
					//begin_flag = 0;
			}
			//按键松开时，对速度清零
			else if(event.type == SDL_KEYUP)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_w: player.speed_y = 0;break;
					case SDLK_s: player.speed_y = 0;break;
					case SDLK_a: player.speed_x = 0;break;
					case SDLK_d: player.speed_x = 0;break;
				}
			}
		}
		//延时一会
		SDL_Delay(10);
		//鼠标关闭右下窗口，退出游戏
		if(event.type == SDL_QUIT )
			break;
		//--------检查子弹是否射击到敌人----------------
		check_shot_enemy(small_enemy, MAX_SMALL_ENEMY, bullet, MAX_BULLET);
		check_shot_enemy(mid_enemy, MAX_MID_ENEMY, bullet, MAX_BULLET);
		check_shot_enemy(big_enemy, MAX_BIG_ENEMY, bullet, MAX_BULLET);
		//-------------飞机死亡判断--------------
		//check_collision(small_enemy, MAX_SMALL_ENEMY, &player);
		//check_collision(mid_enemy, MAX_MID_ENEMY, &player);
		//check_collision(big_enemy, MAX_BIG_ENEMY, &player);

		//飞机死亡回到上面的开始界面
		if(player.pic_index == player.pic_num)
		{
			game_state = GAME_START;
			//begin_flag = 0;
			//关闭定时器
			set_timer(0);
			//暂停一段时间再回到开始界面
			sleep(1);
		}
		
		int i, j;
		for(i = 0; i < MAX_BULLET; i++)
		{
			if(bullet[i].life == 1)
				show_one_element(&game_engine, bullet, i);
		}
		//------从新获取敌人的坐标-------------
		for(i = 0; i < MAX_SMALL_ENEMY; i++)
		{
			get_enemy_pos(&small_enemy[i], 1);
		}
		for(i = 0; i < MAX_MID_ENEMY; i++)
		{
			get_enemy_pos(&mid_enemy[i], 3);
		}
		for(i = 0; i < MAX_MID_ENEMY; i++)
		{
			get_enemy_pos(&big_enemy[i], 6);
		}
		//-----------显示飞机------------
		show_one_element(&game_engine, &player, 0);
		//-----------显示敌人------------
		show_all_element(&game_engine, big_enemy, MAX_BIG_ENEMY);
		show_all_element(&game_engine, mid_enemy, MAX_MID_ENEMY);
		show_all_element(&game_engine, small_enemy, MAX_SMALL_ENEMY);
		//------------------显示分数----------------
		show_score();
		//----------刷新窗体-------------
		SDL_Flip(game_engine.window);
	}
}
