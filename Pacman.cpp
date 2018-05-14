/*************************************************************************
    > File Name: Pacman.cpp
    > Author: genki_chen/黄臣
	> No: S170101106
    > Mail: genki_chen@aliyun.com 
    > Created Time: 2017年11月26日 星期日 17时19分46秒
 ************************************************************************/
//life为初始生命值，M为豆子个数，N为场地大小（这里为一个N×N的场地）
//游戏用hjkl来控制上下左右，左上和右下各有一个怪物，他们会根据自己和吃豆人的相对距离来判断行走方向
//每个豆子可以增加5点生命值，撞墙会失去10点生命值。
//enjoy the game

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<curses.h>
#define N 30
#define M 50
#define life 100

WINDOW *win2;
int local[N-2][N-2];

using namespace std;
class Point
{
	public:
		Point(int v)
		{
			value = v;
		}
		void value_Plus(int v=1)
		{
			value += v;
		}
		void value_diff(int v=1)
		{
			value -= v;
		}
		int get()
		{
			return value;
		}
		void set(int v)
		{
			value = v;
		}
	private:
		int value;
};
class Monster
{
	public:
		int mon_x , mon_y;
		Monster(int x, int y)
		{
			mon_x = x;
			mon_y = y;
		}
		void move(int move_flag)
		{
			switch(move_flag)
			{
				case 1:
					mon_x --;
					break;
				case 2:
					mon_x ++;
					break;
				case 3:
					mon_y --;
					break;
				case 4:
					mon_y ++;
					break;
			}	
			if(mon_x == 0)
				mon_x = N-2;
			if(mon_x == N-1)
				mon_x = 1;
			if(mon_y == 0)
				mon_y = N-2;
			if(mon_y == N-1)
				mon_y = 1;
		}
		void set(int x ,int y)
		{
			mon_x = x;
			mon_y = y;
		}
};

void setbeans(int n)
{
	int beans_x, beans_y;
	for(int i=0;i<N-2;i++)	
		for(int j=0;j<N-2;j++)
			local[i][j] = 0;
	srand((unsigned)time(NULL));  
	for(int i=0;i<n;i++)
	{
		beans_x = (rand()%(N-3))+1;
		beans_y = (rand()%(N-3))+1;		
		local[beans_x][beans_y] = 1;
	}
}

void showpoint(int va)
{
	int hon,dec,uni;
	if(va > 99)
	{

		hon = va / 100;
		dec = (va / 10) % 10;
		uni = va % 10;
		mvwaddch(win2,1,15,dec + 48);
		mvwaddch(win2,1,14,hon + 48);
		mvwaddch(win2,1,16,uni + 48);
	}
	else 
	{
		dec = va / 10;
		uni = va % 10;
		mvwaddch(win2,1,15,dec + 48);
		mvwaddch(win2,1,14,' ');
		mvwaddch(win2,1,16,uni + 48);
		mvwaddstr(win2,1,1,"P1 point is: ");
	}
	wrefresh(win2);
}
int main()
{
	int flag;
	int x = N/2 , old_x;
	int y = N/2 , old_y;
	int m1x,m1y,m2x,m2y;
	int temp_value;
	int beans_x[M] , beans_y[M];
	int a = 0;
	int sum=0;
	Point P1(life);
	Monster m1(4,5);
	Monster m2(25,26);
	WINDOW *win1;
	

	initscr();
	noecho();
	
	curs_set(0);
	cbreak();
	clear();
	mvaddstr(N/2 - 2,N+19,"PacMan");
	mvaddstr(2+N/2,N+10,"h:right  j:down  l:left");
	mvaddstr(1+N/2,N+19,"k:up");
	win1 = newwin(N,N,1,1);
	win2 = newwin(3,20,N/2-10,N+10);
	box(win1 , ACS_VLINE, ACS_HLINE);
	box(win2 , ACS_VLINE, ACS_HLINE);
	mvwaddch(win1,x,y,'e');
	
	//showMonster
	mvwaddch(win1,m1.mon_x,m1.mon_y,'M');
	mvwaddch(win1,m2.mon_x,m2.mon_y,'M');

	setbeans(M);
	for(int i=0;i<N-2;i++)	
		for(int j=0;j<N-2;j++)
		{
			if(local[i][j] == 1)
			{
				mvwaddch(win1,i,j,'*');
				beans_x[a] = i;
				beans_y[a] = j;
				a++;
			}
		}
	temp_value = P1.get();
	refresh();
	wrefresh(win1);
	showpoint(temp_value);
	while(1)
	{
		sum = 0;
		temp_value = P1.get();
		if(temp_value < 1)
		{
			P1.set(0);
			showpoint(temp_value);
			int ch;
			wclear(win1);
			mvwaddstr(win1,N/2,N/2,"game over");
			box(win1 , ACS_VLINE, ACS_HLINE);
			mvwaddstr(win1,N/2+1,N/2,"try again?");
			wrefresh(win1);	
			ch = getch();
			switch(ch)
			{
				case 'y':
					wclear(win1);
					setbeans(M);
					m1.set(4,5);
					m2.set(25,26);
					a = 0;
					for(int i=0;i<N-2;i++)	
						for(int j=0;j<N-2;j++)
						{
							if(local[i][j] == 1)
							{
								mvwaddch(win1,i,j,'*');
								beans_x[a] = i;
								beans_y[a] = j;
								a++;
							}
						}
					x = N/2;
					y = N/2;
					mvwaddch(win1,x,y,'e');
					box(win1 , ACS_VLINE, ACS_HLINE);
					temp_value = 90;
					P1.set(90);
					mvwaddch(win1,m1.mon_x,m1.mon_y,'M');
					mvwaddch(win1,m2.mon_x,m2.mon_y,'M');
					wrefresh(win1);	
					showpoint(temp_value);
					break;
				case 'n':
					endwin(); 
					exit(1); 
				default:
					break;
			}
				continue;
			
		}
		old_x = x;
		old_y = y;
		m1x = m1.mon_x;
		m1y = m1.mon_y;
		m2x = m2.mon_x;
		m2y = m2.mon_y;
		
		flag = getch();
		switch(flag)
		{
			case 'k':
				x--;
				P1.value_diff();
				break;
			case 'j':
				x++;
				P1.value_diff();
				break;
			case 'h':
				y--;
				P1.value_diff();
				break;
			case 'l':
				y++;
				P1.value_diff();
				break;
			case 27: 
				endwin(); 
                exit(1); 
			default:
				break;
		}
		
		mvwaddch(win1,m1.mon_x,m1.mon_y,' ');
		mvwaddch(win1,m2.mon_x,m2.mon_y,' ');

		if(x<1 || x>N-2)
		{
			x = N/2;
			P1.value_diff(10);
		}
		if(y<1 || y>N-2)
		{
			y = N/2;
			P1.value_diff(10);
		}
		for(a=0;a<M;a++)
		{
			if(x == beans_x[a] && y == beans_y[a])
			{
				P1.value_Plus(5);
				beans_x[a] = beans_y[a] = 0;
				break;
			}
		}
		if((x-m1x)^2 <= (y-m1y)^2)
		{
			if(x-m1x < 0)
				m1.move(1);	
			else if(x-m1x > 0)
				m1.move(2);
			else
			{
				if(y-m1y < 0)
					m1.move(3);	
				else if(y-m1y > 0)
					m1.move(4);
			}
		}
		else if((x-m1x)^2 > (y-m1y)^2)
		{
			if(y-m1y < 0)
				m1.move(3);	
			else if(y-m1y > 0)
				m1.move(4);
			else
			{
				if(x-m1x < 0)
					m1.move(1);	
				else if(x-m1x > 0)
					m1.move(2);
			}

		}
		if((x-m2x)^2 <= (y-m2y)^2)
		{
			if(x-m2x < 0)
				m2.move(1);	
			else if(x-m2x > 0)
				m2.move(2);
			else
			{
				if(y-m2y < 0)
					m2.move(3);	
				else if(y-m2y > 0)
					m2.move(4);
			}
		}
		else if((x-m2x)^2 > (y-m2y)^2)
		{
			if(y-m2y < 0)
				m2.move(3);	
			else if(y-m2y > 0)
				m2.move(4);
			else
			{
				if(x-m2x < 0)
					m2.move(1);	
				else if(x-m2x > 0)
					m2.move(2);
			}

		}
		if(old_x == m1x && old_y == m1y)
		{
			P1.set(0);
			continue;
		}
		if(old_x == m2x && old_y == m2y)
		{
			P1.set(0);
			continue;
		}
		for(int z=0;z<M;z++)
		{
			sum += beans_x[z] + beans_y[z];
		}
		if(sum == 0)
		{
			int ch;
			wclear(win1);
			mvwaddstr(win1,N/2,N/2,"game win!");
			box(win1 , ACS_VLINE, ACS_HLINE);
			mvwaddstr(win1,N/2+1,N/2,"try again?");
			wrefresh(win1);	
			ch = getch();
			switch(ch)
			{
				case 'y':
					P1.set(0);
					sum = 0;
					break;
				case 'n':
					endwin();
					exit(1);
			}


		}
		//刷新屏幕部分
		for(a=0;a<M;a++)
		{
			if(beans_x[a] != 0)
				mvwaddch(win1,beans_x[a],beans_y[a],'*');
		}
		
		mvwaddch(win1,old_x,old_y,' ');
		mvwaddch(win1,x,y,'e');
		mvwaddch(win1,m1.mon_x,m1.mon_y,'M');
		mvwaddch(win1,m2.mon_x,m2.mon_y,'M');
		wrefresh(win1);	
		showpoint(temp_value);

	}
	endwin();
	return 0;
}
