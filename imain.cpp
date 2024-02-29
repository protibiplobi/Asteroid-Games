#include "iGraphics.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#define screenheight 700
#define screenwidth 1000
#define asteroidnumber 10
#define enemyshipnumber 1
#define spaceshipheight 105
#define spaceshipwidth 117
#define asteroidheight 60
#define asteroidwidth 60
#define shootwidth 8
#define shootheight 50


char animation[9][16] = {"anim\\anim1.bmp", "anim\\anim2.bmp", "anim\\anim3.bmp", "anim\\anim4.bmp", "anim\\anim5.bmp", "anim\\anim6.bmp", "anim\\anim7.bmp", "anim\\anim8.bmp"};
char game[7][20] = {"space1.bmp", "asteroid.bmp", "ship.bmp", "enemyship.bmp", "gameover.bmp","fire.bmp","enemyshoot.bmp"};

int player_x = 0, player_y = 0; // Co_ordinates of spaceship

int shoot_radius = 5; // bullet radius

int game_state = 0; // game_state=0 means menu, 1 means game, 2 means game over, 3 means high score

bool explosion_music = false;

int score = 0;
int lives = 5;
int game_over = 0;

int name_index = 0;
char name[100];

int number;

char menu[5][20] = {"menubg.bmp", "title.bmp", "menu.bmp"};
void showmenu()
{
	if (game_state == 0)
	{
		iShowBMP(0, 0, menu[0]);
		iShowBMP2(15, 550, menu[1], 255);  // title
		iShowBMP2(120, 150, menu[2], 255); // menu
	}
}

void playgame()
{
	if (game_state == 1)
	{
		iShowBMP(0, 0, game[0]);				   // space background show
		iShowBMP2(player_x, player_y, game[2], 0); // spaceship show
	}
}


int bullet_no = -1;

struct weapon
{
	double bullet_x;
	double bullet_y;
	bool bullet_show;
};
weapon bullet2[20];
weapon bullet1[20];

void shoot()
{
	if (game_state == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			if (bullet2[i].bullet_show)
			{
				iSetColor(255, 0, 0);
				iShowBMP2(bullet2[i].bullet_x, bullet2[i].bullet_y,game[5], 0);
			}
			if (bullet2[i].bullet_y > screenheight)
			{
				bullet2[i].bullet_show = false;
			}
		}

		for (int i = 0; i < 20; i++)
		{
			if (bullet1[i].bullet_show)
			{
				iSetColor(255, 0, 0);
				iShowBMP2(bullet1[i].bullet_x, bullet1[i].bullet_y,game[5], 0);
			}
			if (bullet1[i].bullet_y > screenheight)
			{
				bullet1[i].bullet_show = false;
			}
		}
	}
}

struct enemy
{
	double asteroid_x;
	double asteroid_y;
	bool asteroid_show;

	double enemyship_x;
	double enemyship_y;
	double enemyship_dx = 3;
	double enemyship_dy = 3;
	bool enemyship_show;
	int enemyship_life;
	bool horizontal;
};
enemy asteroid[asteroidnumber];
enemy enemyship[enemyshipnumber];

struct enemy_attack
{
	double bullet_x;
	double bullet_y;
	bool bullet_show=false;
};
enemy_attack enemy_bullet[20];

void enemy_shoot()
{
	if (game_state == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			if (enemy_bullet[i].bullet_show)
			{
				iSetColor(255, 0, 0);
				iShowBMP2(enemy_bullet[i].bullet_x, enemy_bullet[i].bullet_y, game[6], 0);
			}
			if (enemy_bullet[i].bullet_y < 0)
			{
				enemy_bullet[i].bullet_show = false;
			}
		}
	}
}

void enemy_spawn()
{
	if (game_state == 1)
	{

		for (int i = 0; i < asteroidnumber; i++)
		{
			if (asteroid[i].asteroid_show == true)
			{
				iShowBMP2(asteroid[i].asteroid_x, asteroid[i].asteroid_y, game[1], 0);
			}
			if (asteroid[i].asteroid_y < 0)
			{
				asteroid[i].asteroid_show = false;
			}
			if (asteroid[i].asteroid_show == false) /*Respawning of enemies*/
			{
				asteroid[i].asteroid_x = rand() % (screenwidth-asteroidwidth);
				asteroid[i].asteroid_y = screenheight + rand() % 100;
				asteroid[i].asteroid_show = true;
			}
		}

		if (score > 50)
		{
			for (int i = 0; i < enemyshipnumber; i++)
			{
				if (enemyship[i].enemyship_show == true)
				{
					iShowBMP2(enemyship[i].enemyship_x, enemyship[i].enemyship_y, game[3], 0);
				}
				if (enemyship[i].enemyship_show == false) // Respawning of enemies//
				{
					enemyship[i].enemyship_x = rand() % (screenwidth - 100);
					enemyship[i].enemyship_y = screenheight + rand() % 100;
					enemyship[i].enemyship_show = true;
				}
				if (enemyship[i].enemyship_y < screenheight - 150 - (rand() % 300))
				{
					enemyship[i].horizontal = true;
				}
			}
		}
	}
}

struct destruction
{
	double collision_x;
	double collision_y;
	int animation_index;
	bool collision_show;
};
destruction collision[20];

void collision_animation()
{
	for (int i = 0; i < 20; i++)
	{
		if (collision[i].collision_show)
		{
				iShowBMP2(collision[i].collision_x, collision[i].collision_y, animation[collision[i].animation_index], 0);
		}
			// PlaySoundA("music\\efprintf(point,"Yo, bang bang skitz, skitz, nigga");xplosion.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
}


void scorecard()
{
	if (game_state == 1)
	{
		iSetColor(255, 255, 255);
		iText(screenwidth - 100, screenheight - 30, "Score: ", GLUT_BITMAP_HELVETICA_18);
		iText(screenwidth - 100, screenheight - 50, "Lives: ", GLUT_BITMAP_HELVETICA_18);
		char scoreboard[10];
		sprintf(scoreboard, "%d", score);
		iText(screenwidth - 40, screenheight - 30, scoreboard, GLUT_BITMAP_HELVETICA_18);
		char showlives[10];
		sprintf(showlives, "%d", lives);
		iText(screenwidth - 40, screenheight - 50, showlives, GLUT_BITMAP_HELVETICA_18);
	}
}






void gameover()
{
	if (lives < 0)
	{
		game_state = 2;
		game_over = 1;
		iShowBMP(0, 0, game[4]);
		iSetColor(255, 255, 255);
		iText(410, screenheight - 180, "Your Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
		char scoreboard[10];
		sprintf(scoreboard, "%d", score);
		iSetColor(255, 255, 255);
		iText(530, screenheight - 180, scoreboard, GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(0, 0, 0);
		iText(370, screenheight - 265, name, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (game_state == 2)
	{
		player_x = 0;
		player_y = 0;
		for (int i = 0; i < asteroidnumber; i++)
		{
			asteroid[i].asteroid_x = rand() % (screenwidth - 100);
			asteroid[i].asteroid_y = screenheight + rand() % 100;
			asteroid[i].asteroid_show = true;
		}

		for (int i = 0; i < enemyshipnumber; i++)
		{
			enemyship[i].enemyship_x = rand() % (screenwidth - 100);
			enemyship[i].enemyship_y = screenheight + rand() % 100;
			enemyship[i].enemyship_show = true;
			enemyship[i].horizontal = false;
		}
		for (int i = 0; i < 20; i++)
		{
			bullet1[i].bullet_show = false;
			bullet2[i].bullet_show = false;
			enemy_bullet[i].bullet_show=false;
		}
		enemyship[0].enemyship_show=false;
	}
}

void highscore()
{
	FILE *numberfp = fopen("numberofplayer.txt", "r");
	fscanf(numberfp, "%d", &number);
	fclose(numberfp);
	FILE *fp = fopen("highscore.txt", "r");
	char show[1000];
	int perform;
	int i = 0;
	int dy = 400;
	for (int i = 0; i < number; i++)
	{
		fscanf(fp, "%s %d", show, &perform);
		char scorecard[10];
		itoa(perform, scorecard, 10);
		iSetColor(255, 0, 0);
		iText(400, 100 + dy, show, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(500, 100 + dy, scorecard, GLUT_BITMAP_TIMES_ROMAN_24);
		dy -= 50;
	}
	fclose(fp);
	iText(300, 100, "Press Enter to go to Main Menu", GLUT_BITMAP_TIMES_ROMAN_24);
}

/*
	function iDraw() is called again and again by the system.
	*/
void iDraw()
{
	iClear();
	showmenu();
	playgame();
	shoot();
	enemy_spawn();
	enemy_shoot();
	collision_animation();
	scorecard();
	gameover();
	if (game_state == 3)
	{
		highscore();
	}
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my)
{
	printf("x = %d, y= %d\n", mx, my);
	// place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{
	/* if (game_state == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx>=120 && mx<=582 && my>=150 && my<=475 ) {
		//place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
		game_state = 1;
	} */

	if (game_state == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 120 && mx <= (120 + 291))
	{
		if (my >= 150 && my <= (150 + 63))
		{
			exit(0);
		}
		if (my >= 231 && my <= (231 + 63))
		{
			// credit
		}
		if (my > 312 && my <= (312 + 63))
		{
			game_state = 3;
		}
		if (my >= 394 && my <= (394 + 63))
		{
			// instructions
		}
		if (my >= 476 && my <= (476 + 63))
		{
			game_state = 1;
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
		if (game_state == 1)
		{
			if (bullet_no >= 19)
			{
				bullet_no = -1;
			}
			bullet_no++;
			bullet2[bullet_no].bullet_x = player_x + 18;
			bullet2[bullet_no].bullet_y = player_y + 80;
			bullet1[bullet_no].bullet_x = player_x + 99;
			bullet1[bullet_no].bullet_y = player_y + 80;
			bullet2[bullet_no].bullet_show = true;
			bullet1[bullet_no].bullet_show = true;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// place your codes here
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key)
{
	if (key == 'q')
	{
		exit(0);
	}
	if (game_state == 2 && key != '\b')
	{
		name[name_index] = key;
		name_index++;
		name[name_index] = '\0';
	}
	else if (game_state == 2 && key == '\b' && name_index != 0)
	{
		name_index--;
		name[name_index] = '\0';
	}
	if (key == '\r')
	{
		if (game_state == 2)
		{
			FILE *fp = fopen("highscore.txt", "a");
			fprintf(fp, "%s %d\n", name, score);
			fclose(fp);

			FILE *ofp = fopen("numberofplayer.txt", "r");
			fscanf(ofp, "%d", &number);
			fclose(ofp);
			number++;
			ofp = fopen("numberofplayer.txt", "w");
			fprintf(ofp, "%d", number);
			fclose(ofp);
			game_state = 0;
			lives = 5;
			score = 0;
		}
		if (game_state == 3)
		{
			game_state = 0;
		}
	}

	// place your codes for other keys here
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
void iSpecialKeyboard(unsigned char key)
{
	if (key == GLUT_KEY_UP && player_y + 140 < screenheight)
	{
		player_y += 10;
	}
	else if (key == GLUT_KEY_DOWN && player_y > 0)
	{
		player_y -= 10;
	}
	else if (key == GLUT_KEY_RIGHT && (player_x + 100 < screenwidth))
	{
		player_x += 10;
	}
	else if (key == GLUT_KEY_LEFT && (player_x > 0))
	{
		player_x -= 10;
	}
}

void shooting()
{
	if (game_state == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			bullet2[i].bullet_y += 10;
		}
		for (int i = 0; i < 20; i++)
		{
			bullet1[i].bullet_y += 10;
		}
	}
}

int collision_no=0;

void collisioncheck()
{
	if (game_state == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < asteroidnumber; j++)
			{
				if ((bullet2[i].bullet_x + shootwidth > asteroid[j].asteroid_x) && asteroid[j].asteroid_show == true && bullet2[i].bullet_show == true && (bullet2[i].bullet_x - shootwidth < asteroid[j].asteroid_x + asteroidwidth) && ((bullet2[i].bullet_y + shootheight) > asteroid[j].asteroid_y) && ((asteroid[j].asteroid_y + asteroidheight) > (bullet2[i].bullet_y - shootheight)))
				{
					asteroid[j].asteroid_show = false;
					bullet2[i].bullet_show = false;
					collision[collision_no].collision_x = asteroid[j].asteroid_x;
					collision[collision_no].collision_y = asteroid[j].asteroid_y;
					collision[collision_no].collision_show = true;
					collision_no++;
					if(collision_no>19)
					{
						collision_no=0;
					}
					score++;
				}
			}

			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < asteroidnumber; j++)
				{
					if ((bullet1[i].bullet_x + shootwidth > asteroid[j].asteroid_x) && asteroid[j].asteroid_show == true && bullet1[i].bullet_show == true && (bullet1[i].bullet_x - shootwidth < asteroid[j].asteroid_x + asteroidwidth) && ((bullet1[i].bullet_y + shootheight) > asteroid[j].asteroid_y) && ((asteroid[j].asteroid_y + asteroidheight) > (bullet1[i].bullet_y - shootheight)))
					{
						asteroid[j].asteroid_show = false;
						bullet1[i].bullet_show = false;
						collision[collision_no].collision_x = asteroid[j].asteroid_x;
						collision[collision_no].collision_y = asteroid[j].asteroid_y;
						collision[collision_no].collision_show = true;
						collision_no++;
						if(collision_no>19)
						{
							collision_no=0;
						}
						score++;
					}
				}
			}

			for (int k = 0; k < asteroidnumber; k++)
			{
				if ((player_x < (asteroid[k].asteroid_x + asteroidwidth)) && (asteroid[k].asteroid_show == true) && ((player_x + spaceshipwidth) > asteroid[k].asteroid_x) && ((player_y + spaceshipheight) > asteroid[k].asteroid_y) && ((asteroid[k].asteroid_y + 70) > player_y))
				{
					asteroid[k].asteroid_show = false;
					collision[collision_no].collision_x = asteroid[k].asteroid_x;
					collision[collision_no].collision_y = asteroid[k].asteroid_y;
					collision[collision_no].collision_show = true;
					collision_no++;
					if(collision_no>19)
					{
						collision_no=0;
					}
					lives--;
				}
			}
			

		}


	}
}



int enemy_bullet_no=-1;

void change()
{
	if (game_state == 1)
	{
		shooting();
		for (int i = 0; i < asteroidnumber; i++)
		{
			asteroid[i].asteroid_y -= 4;
		}
		for (int i = 0; i < enemyshipnumber; i++)
		{
			if (enemyship[i].horizontal == false)
			{
				enemyship[i].enemyship_y -= enemyship[i].enemyship_dy;
			}

			enemyship[i].enemyship_x -= enemyship[i].enemyship_dx;

			if (enemyship[i].enemyship_x >= screenwidth || enemyship[i].enemyship_x <= 5)
			{
				enemyship[i].enemyship_dx = -enemyship[i].enemyship_dx;
			}
		}
	}
	for(int i=0;i<20;i++)
	{
			enemy_bullet[i].bullet_y -= 5;
	}
	
}

void enemy_coordinates()
{
	if (game_state == 1)
	{
		for (int i = 0; i < asteroidnumber; i++)
		{
			asteroid[i].asteroid_x = rand() % (screenwidth - 100);
			asteroid[i].asteroid_y = screenheight + rand() % 100;
			asteroid[i].asteroid_show = true;
		}
		if (score > 50)
		{
			for (int i = 0; i < enemyshipnumber; i++)
			{
				enemyship[i].enemyship_x = rand() % (screenwidth - 100);
				enemyship[i].enemyship_y = screenheight + rand() % 100;
				enemyship[i].enemyship_show = true;
				enemyship[i].horizontal = false;
			}
		}
	}
}

void enemy_shooting()
{
	if (game_state == 1 && enemyship[0].enemyship_show==true)
	{
		if (enemy_bullet_no >= 19)
		{
			enemy_bullet_no = -1;
		}
		enemy_bullet_no++;
		enemy_bullet[enemy_bullet_no].bullet_x = enemyship[0].enemyship_x + 6;
		enemy_bullet[enemy_bullet_no].bullet_y = enemyship[0].enemyship_y + 0;
		enemy_bullet[enemy_bullet_no].bullet_show = true;
	}
}


void col_animation()
{
	for(int i=0;i<20;i++)
		{
			if(collision[i].collision_show==true)
			{
				collision[i].animation_index++;

				if (collision[i].animation_index == 7)
				{
					collision[i].collision_show = false;
					collision[i].animation_index = 0;
				}
			}
		}
}

int main()
{
	enemy_coordinates();
	// PlaySoundA("music\\street.wav", NULL, SND_FILENAME | SND_ASYNC);
	iSetTimer(20, collisioncheck);
	iSetTimer(1, change);
	iSetTimer(30,col_animation);
	iSetTimer(1000,enemy_shooting);
	iInitialize(screenwidth, screenheight, "Asteroid Escape");
	return 0;
}
