#include <curses.h>
#include <stdlib.h>
#include <time.h>

int STARTX = 0;
int STARTY = 0;
int ENDX = 0;
int ENDY = 0;

#define TIME_OUT  300

typedef struct _state {
	char oldstate;
	char newstate;
}state;

void display(WINDOW *win, state **area, int startx, int starty, int endx, int endy);
void initialize(state **area, int startx, int starty, int endx, int endy);
void generate(state **area, int startx, int starty, int endx, int endy);
void calc(state **area, int x, int y);
void update_state(state **area, int startx, int starty, int endx, int endy);

int main()
{	state **workarea;
	int i, j;
	
	initscr();
	cbreak();
	timeout(TIME_OUT);
	keypad(stdscr, TRUE);

	srand(time(NULL));

	ENDX = LINES - 1;
	ENDY = COLS - 1;

	workarea = (state **)calloc(LINES, sizeof(state *));
	for(i = 0;i < LINES; ++i)
		workarea[i] = (state *)calloc(COLS, sizeof(state));

	initialize(workarea, STARTX, STARTY, ENDX, ENDY);
	generate(workarea, STARTX, STARTY, ENDX, ENDY);
	
	while(getch() != KEY_F(1))
	{	for(i = STARTX+1; i <= ENDX; ++i)
			for(j = STARTY; j <= ENDY; ++j)
				calc(workarea, i, j);
		update_state(workarea, STARTX, STARTY, ENDX, ENDY);
		display(stdscr,  workarea, STARTX, STARTY, ENDX, ENDY);
		generate(workarea, STARTX, STARTY, ENDX, ENDY);	
	}
	
	endwin();
	return 0;
}	

void display(WINDOW *win, state **area, int startx, int starty, int endx, int endy)
{	int i, j;
	wclear(win);
	for(i = startx; i <= endx; ++i)
		for(j = starty;j <= endy; ++j)
			if(area[i][j].newstate != ' ')
				mvwaddch(win, i, j, area[i][j].newstate);
	wrefresh(win);
}

void initialize(state **area, int startx, int starty, int endx, int endy)
{
	int i,j;

	for(i = startx; i <= endx; ++i)
		for(j = starty; j <= endy; ++j)
		{
			area[i][j].newstate = ' ';
			area[i][j].oldstate = ' ';
		}
			
}

void generate(state **area, int startx, int starty, int endx, int endy)
{
	int j, s;

	for(j = starty; j <= endy; ++j)
	{
		if(!(rand() % 100))
		{
			s = rand() % 2;

			if(s)
				area[startx][j].newstate = 65 + rand() % 26; 
			else
				area[startx][j].newstate = 97 + rand() % 26;
		}
			
	}
}

void calc(state **area, int i, int j)
{
	if(area[i-1][j].oldstate != ' ' && area[i][j].oldstate == ' ')
	{
		area[i-1][j].newstate = ' ';
		area[i][j].newstate = area[i-1][j].oldstate;
	}
		
}

void update_state(state **area, int startx, int starty, int endx, int endy)
{	int i, j;
	
	for(i = startx; i <= endx; ++i)
		for(j = starty; j <= endy; ++j)
			area[i][j].oldstate = area[i][j].newstate;
}	

