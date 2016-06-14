/*joystick上有一个ANALOG按键
ANALOG没有按下时，左边摇杆的功能和左边4个按键的功能相同，右边摇杆的功能和右边4个的按键功能相同。右边4按键的编号是1，2，3，4，侧边4个按键的编号是5，6，7，8,左边摇杆按下时编号是10，右边摇杆按下是编号是11。分别对应按键的结构体event.jbutton.button中的0到7，在这个结构体SELECT按键的编号是8，START按键的编号是9.
ANALOG按下时，左边摇杆还是原来的功能，x轴，y轴编号是0，1。而右边摇杆与左边摇杆功能一样，x轴，y轴的编号是2，3。右边按键和侧边按键的编号以及SELECT，START编号不变。
*/
#include <stdio.h>

#include "plane.h"
#include <SDL/SDL.h>
//是否开启ANALOG 模式,如果没启动，编译的就可以去掉那部分代码，虽然一起编译也不会右影响
#define ANALOG 1


extern int auto_shot_flag;
extern int game_state;
extern SDL_Joystick *stick;
extern SDL_Event event; 
extern PLANE player; 

void init_joystick()
{
	//检查是否有控制杆,检查控制杆数量
	//SDL_NumJoysticks 返回控制杆数量
	if(SDL_NumJoysticks() < 1)
	{
		//没有就退出
		printf("can't find joystick!\n");
		return;
	}
	//打开控制杆,第一个控制杆从0开始计数
	stick = SDL_JoystickOpen(0);
	if(stick == NULL)
	{
		printf("open joystick fail\n");
		return;
	}
}
//关闭控制杆
void close_joystick()
{
	SDL_JoystickClose(stick);
}

//控制杆的轴的范围是-32768到32767,这里认为-8000到8000的范围是中间位置
void joy_axis_motion()
{
	if(event.type == SDL_JOYAXISMOTION)
	{	//which 为0时表示joystick设备号是0，有时候会有多个设备
		if(event.jaxis.which == 0)
		{
			//左边摇杆x轴方向		
			if(event.jaxis.axis == 0)
			{
				if((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
				{	
					//这里认为处于轴的中间位置
					player.speed_x = 0;
				}
				else
				{	//左边的轴向左偏
					if(event.jaxis.value < 0)
						player.speed_x = -2;
					else
					//左边的轴向右偏
						player.speed_x = 2;
				}
			}
			//左边摇杆y轴方向坐标改变
			else if(event.jaxis.axis == 1)
			{
				if((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
				{	//中间位置
					player.speed_y = 0;
				}
				else 
				{	//左边的轴向上偏
					if(event.jaxis.value < 0)
						player.speed_y = -2;
					else
					//左边的轴向下偏
						player.speed_y = 2;
				}
			}
//ANALOG模式才有
#if ANALOG
			//右边摇杆x方向
			else if(event.jaxis.axis == 2)
			{
				if((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
				{	
					//这里认为处于轴的中间位置
					player.speed_x = 0;
				}
				else
				{	//左边的轴向左偏
					if(event.jaxis.value < 0)
						player.speed_x = -2;
					else
					//左边的轴向右偏
						player.speed_x = 2;
				}
			}
			//右边摇杆y轴方向坐标改变
			else if(event.jaxis.axis == 3)
			{
				if((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
					//中间位置
					player.speed_y = 0;
				else 
				{	//右边的轴向上偏
					if(event.jaxis.value < 0)
						player.speed_y = -2;
					else
					//右边的轴向下偏
						player.speed_y = 2;
				}
			}
#endif
		}
	}
}

//=========joystick down==============
void joy_button_down()
{
	//检测按键按下的动作
	if(event.type == SDL_JOYBUTTONDOWN)
	{	//which 为0是表示joystick设备号是0，有时会有多个设备	
		if(event.jbutton.which == 0)
		{	
			switch (event.jbutton.button)
			{
				case 0:     //按键1
					auto_shot_flag = 1;
					break;
				case 1:		//按键2
					break;
				case 2:		//按键3
					break;
				case 3:		//按键4
					break;
				case 4:		//按键5
					break;
				case 5:		//按键6
					break;
				case 6:		//按键7
					break;
				case 7:		//按键8
					break;
				case 8:		//SELECT
					//从新到开始界面
					game_state = GAME_START;
					break;
				case 9:		//START
					break;
				case 10:	//左边摇杆按下
					break;
				case 11:	//右边摇杆按下
					break;
			}
		}
	}
}

void joy_button_up()
{
	//检测松开按键动作
	if(event.type == SDL_JOYBUTTONUP)
	{	//which 为0时表示joystick设备号是0，有时候会有多个设备
		if(event.jbutton.which == 0)
		{	
			switch (event.jbutton.button)
			{
				case 0:     //按键1
					//停止自动射击
					auto_shot_flag = 0;
					break;
				case 1:		//按键2
					break;
				case 2:		//按键3
					break;
				case 3:		//按键4
					break;
				case 4:		//按键5
					break;
				case 5:		//按键6
					break;
				case 6:		//按键7
					break;
				case 7:		//按键8
					break;
				case 8:		//SELECT
					break;
				case 9:		//START
					break;
				case 10:	//左边摇杆松开
					break;
				case 11:	//右边摇杆松开 
					break;
			}
		}
	}
}
//ANALOG模式时
//左边4个按键的value值，上为1，左为2，下位4，右为8
void joy_hat_motion()
{
	if(event.type == SDL_JOYHATMOTION)
	{
		if(event.jhat.which == 0)
		{
			if(event.jhat.hat == 0)
			{

				switch (event.jhat.value)
				{
					case 0:		//center
						printf("value = %d\n", event.jhat.value);
						break;
					case 1:		//up
						printf("value = %d\n", event.jhat.value);
						break;
					case 3:		//rightup
						printf("value = %d\n", event.jhat.value);
						break;
					case 2:		//right
						printf("value = %d\n", event.jhat.value);
						break;
					case 6:		//rightdown
						printf("value = %d\n", event.jhat.value);
						break;
					case 4:		//down
						printf("value = %d\n", event.jhat.value);
						break;
					case 12:	//leftdown
						printf("value = %d\n", event.jhat.value);
						break;
					case 8:		//left
						printf("value = %d\n", event.jhat.value);
						break;
					case 9:		//leftup
						printf("value = %d\n", event.jhat.value);
						break;
				}
			}
		}
	}
}

//使用手头上的joystcik没有测到，具体是什么情况不是很清楚
void joy_ball_motion()
{
	if(event.type == SDL_JOYBALLMOTION)
	{
		printf("ballmotion");
		if(event.jball.which == 0)
		{

			printf("xrel = %d yrel = %d\n", event.jball.xrel, event.jball.yrel);
			if(event.jball.ball == 0)
			{
				printf("xrel = %d yrel = %d\n", event.jball.xrel, event.jball.yrel);
			}
			if(event.jball.ball == 1)
			{
				printf("xrel = %d yrel = %d\n", event.jball.xrel, event.jball.yrel);
			}
			if(event.jball.ball == 2)
			{
				printf("xrel = %d yrel = %d\n", event.jball.xrel, event.jball.yrel);
			}
			if(event.jball.ball == 3)
			{
				printf("xrel = %d yrel = %d\n", event.jball.xrel, event.jball.yrel);
			}
		}

	}
}
