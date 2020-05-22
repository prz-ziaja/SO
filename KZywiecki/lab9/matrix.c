#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <string.h> 
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

char* buffers[2];

int size_x;
int size_y;

int ind = 0;

void ChangeSize()
{
	int new_x, new_y;
	getmaxyx(stdscr, new_y, new_x);
	
	if(new_x != size_x || new_y != size_y)
	{
		buffers[0] = calloc(sizeof(char), size_x*size_y);
		buffers[1] = calloc(sizeof(char), size_x*size_y);
		size_y = new_y;
		size_x = new_x;
	}
	

}

void Display()
{
	char temp[2];
	temp[0] = 0;
	temp[1] = 0;
	for(int y = 0; y < size_y; y++)
	{
		for(int x = 0; x < size_x; x++)
		{
			char z = buffers[ind][x + y * size_x];
			if(z != '\0')
			{
				temp[0] = z;
				mvprintw(y, x, temp);
			}
		}
	}
}

void Update()
{
	ChangeSize();
	int from = ind;
	int to = from ? 0 : 1;
	memcpy(buffers[to] + size_x, buffers[from], size_x * (size_y - 1));

	for(char* begin = buffers[to]; begin < buffers[to] + size_x; begin++)
	{
		int temp = rand() % 'z';
		temp = temp + 1 - 'a';
		if(temp >= 0)
		{
			*begin = temp + 'a';
		}
		else
		{
			*begin = '\0';
		}
		
	}
	ind = ind ? 0 : 1;
}

int main()
{
	srand(time(NULL));
	initscr();
	curs_set(false);
	noecho();

	getmaxyx(stdscr, size_y, size_x);
	buffers[0] = calloc(sizeof(char), size_x*size_y);
	buffers[1] = calloc(sizeof(char), size_x*size_y);
	
	buffers[0][1] = 'a';
	buffers[0][2] = 'b';
	while(true)
	{
		clear();
		Update();
		Display();
		refresh();
		usleep(150000);
	}
	endwin();

	free(buffers[0]);
	free(buffers[1]);
	return 0;
}
