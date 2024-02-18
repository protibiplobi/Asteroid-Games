# include "iGraphics.h"
#include <windows.h>
#define screenheight 650
#define screenwidth 1000
#define NUMBEROFENEMY 50


FILE *fp;

/*
	function iDraw() is called again and again by the system.
	*/
char animation[9][16]={"anim1.bmp","anim2.bmp","anim3.bmp","anim4.bmp","anim5.bmp","anim6.bmp","anim7.bmp","anim8.bmp"};
char menu[5][20]= {"menubg.bmp", "title.bmp", "menu.bmp"};
char game[5][20] = {"space1.bmp", "asteroid.bmp", "ship.bmp","gameover.bmp"};
int space_x=0, space_y=0; 
int image=0;
int shoot_radius=5; //46,122 hocche top
bool collision= false;
double collision_x, collision_y;
int animation_index=0;
bool explosion_music= false;

int score=0;
int lives=5;
int game_over=0;
//image=0 maane menu, image=1 maane game, image=2 maane gameover
void showmenu()
{
	if(image==0)
	{
		iShowBMP(0,0,menu[0]);
		iShowBMP2(200,500,menu[1], 255);
		iShowBMP2(250,10,menu[2],255);
	}
}

void playgame()
{
	if(image == 1)
	{
		iShowBMP(0,0, game[0]);
		iShowBMP2(space_x,space_y,game[2],0);
	}
}

void scorecard()
{
	if(image==1)
	{
		iSetColor(255,255,255);
		iText(screenwidth-100, screenheight-30, "Score: ", GLUT_BITMAP_HELVETICA_18);
		iText(screenwidth-100, screenheight-50, "Lives: ", GLUT_BITMAP_HELVETICA_18);
		char scoreboard[10];
		sprintf(scoreboard, "%d", score);
		iText(screenwidth-40, screenheight-30, scoreboard, GLUT_BITMAP_HELVETICA_18);
		char showlives[10];
		sprintf(showlives, "%d", lives);
		iText(screenwidth-40, screenheight-50, showlives, GLUT_BITMAP_HELVETICA_18);

	}
}


int bullet_no=-1;

struct bullet
{
	double bullet_x;
	double bullet_y;
	bool bullet_show;
};
bullet guli[20];

void bomabaji()
{
	for(int i=0;i<20;i++)
	{
		if(guli[i].bullet_show)
		{
			iSetColor(255,0,0);
			iFilledCircle(guli[i].bullet_x, guli[i].bullet_y, shoot_radius, 100);
		}
		if(guli[i].bullet_y>screenheight)
		{
			guli[i].bullet_show = false;
		}
	}
}

struct shotru
{
	double enemy_x;
	double enemy_y;
	int enemy_index;
	bool enemy_show;
};
shotru dushmon[NUMBEROFENEMY];

void dushmon_spawn()
{
	for(int i=0; i<NUMBEROFENEMY;i++)
	{
		if(dushmon[i].enemy_show == true && image==1)
		{
			iShowBMP2(dushmon[i].enemy_x,dushmon[i].enemy_y, game[1],0);
		}
		if(dushmon[i].enemy_y<0)
		{
			dushmon[i].enemy_y= screenheight + rand() % 100;
		}
		if(dushmon[i].enemy_show==false) /*Respawning of enemies*/
		{
			dushmon[i].enemy_x = rand() % screenwidth;
			dushmon[i].enemy_y= screenheight + rand() % 100;
			dushmon[i].enemy_show = true;


		}
	}
}

void collision_animation()
{

	if(collision)
	{
	while(animation_index<7)
	{
		iShowBMP2(collision_x,collision_y, animation[animation_index],0);
		animation_index++;
	}
	PlaySoundA("music\\explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	if(animation_index==7)
	{
		collision=false;
		animation_index=0;
	}
	}
}

void gameover()
{
	if(lives<0)
	{
		image = 2;
		game_over=1;
		iShowBMP(0,0,game[3]);
		iText(450, screenheight-200, "Your Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
		char scoreboard[10];
		sprintf(scoreboard, "%d", score);
		iText(575, screenheight-200, scoreboard, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}



void iDraw() {
	//place your drawing codes here
	iClear();
	showmenu();
	playgame();
	bomabaji();
	dushmon_spawn();
	collision_animation();
	scorecard();
	gameover();
}



/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
    if (image == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx>=353 && mx<=582 && my>=400 && my<=475 ) {
		//place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
		image = 1;
	}

	if (image == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx>=318 && mx<=635) 
	{
		if(my>=20 && my<=87)
		{
            exit(0);
		} 
		if(my>=120 && my<=187)
		{
			//credit
		}
		if(my>223 && my<=291)
		{
			//high score
		}
		if(my>=319 && my<=388)
		{
			//instructions
		}
        if(my>= 422 && my<=491)
		{
			image=1;
		}
	}
    



	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
		if(image==1)
		{
			if(bullet_no>=19)
			{
				bullet_no=-1;
			}
			bullet_no++;
			guli[bullet_no].bullet_x = space_x + 110;
			guli[bullet_no].bullet_y = space_y + 125;
			guli[bullet_no].bullet_show = true; 
		}
		
		if(image==2)
		{
			image=0;
			lives=5;
			score=0;
		}

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if (key == 'q') {
		exit(0);
	}
	
	
	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

if (key == GLUT_KEY_END) {
		exit(0);
	}

else if (key == GLUT_KEY_UP && space_y+140<screenheight)
	{
		space_y += 10;
	}
	else if (key == GLUT_KEY_DOWN && space_y>0)
	{
		space_y -= 10;
	}
	else if (key == GLUT_KEY_RIGHT && (space_x+100<screenwidth))
	{
		space_x += 10;
	}
	else if (key == GLUT_KEY_LEFT && (space_x>0))
	{
		space_x -= 10;
	}
}


void shooting()
{
	if(image==1)
	{
	for(int i=0; i<20; i++)
	{
        guli[i].bullet_y += 10;
	}
	}
}

void collisioncheck()
{
	if(image==1)
	{
		for(int i=0; i<20;i++)
		{
			for(int j=0; j<NUMBEROFENEMY; j++)
			{
			if((guli[i].bullet_x+shoot_radius>dushmon[j].enemy_x) && dushmon[j].enemy_show == true && guli[i].bullet_show==true && (guli[i].bullet_x-shoot_radius < dushmon[j].enemy_x+73) && ((guli[i].bullet_y+shoot_radius) > dushmon[j].enemy_y) && ((dushmon[j].enemy_y+ 70)>(guli[i].bullet_y-shoot_radius)))
			{ 
           		dushmon[j].enemy_show = false;
				guli[i].bullet_show=false;
				collision_x = dushmon[j].enemy_x;
				collision_y = dushmon[j].enemy_y;
				collision = true;
				score++;
			}
			}
		}

		for(int k=0; k<NUMBEROFENEMY; k++)
			{
			if((space_x<(dushmon[k].enemy_x+74)) && dushmon[k].enemy_show == true && ((space_x+144) > dushmon[k].enemy_x) && ((space_y+109) > dushmon[k].enemy_y) && ((dushmon[k].enemy_y+ 70)>space_y))
			{ 
           		dushmon[k].enemy_show = false;
				collision_x = dushmon[k].enemy_x;
				collision_y = dushmon[k].enemy_y;
				collision = true;
				lives--;
			}
			}
	}
}


void change()
{
	shooting();
	for(int i=0; i<NUMBEROFENEMY;i++)
	{
		dushmon[i].enemy_y -= 5;
	}
}





void enemy_coordinates()
{
	for(int i=0; i<NUMBEROFENEMY;i++)
	{
		dushmon[i].enemy_x = rand() % (screenwidth-100);
		dushmon[i].enemy_y= screenheight + rand() % 100;
		dushmon[i].enemy_show = true;
	}
}



int main() 
{
	enemy_coordinates();
	//place your own initialization codes here.
	//PlaySoundA("music\\street.wav", NULL, SND_FILENAME | SND_ASYNC);
	//iSetTimer(25,collisioncheck);
	iSetTimer(20,collisioncheck);
	iSetTimer(30, change);
	iInitialize(screenwidth, screenheight, "Asteroid Escape");
	return 0;
}
