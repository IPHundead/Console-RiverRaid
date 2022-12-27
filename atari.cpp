//ATARI version 0.4, Stable, DEV XN_UND

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include <cstdlib>
#include <time.h>

using namespace std;

#define big_three "HEEEY"

struct basicclass{
	bool alive=false;
	int x=0;
	int y=0;
};

struct bulletclass:basicclass{	
};

struct barrierclass:basicclass{
};

struct enemyclass:basicclass{	
	bool direction=0;
	int move=0;
};

void locate ( int x, int y ){
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition ( GetStdHandle ( STD_OUTPUT_HANDLE ), position );
}

int randomnumber()
{
	int randn;
	randn=rand()%50+1;
	if (randn<6)
	{
		return 6;
	}
	if (randn > 44)
	{
		return 44;
	}
	return randn;
}

void changecolor(string color)
{
	if (color == "blue")
	{
		system("color 9f");
	}
	else if (color == "red")
	{
		system("color 4f");
	}
	else if (color == "black")
	{
		system("color 0f");
	}
	else if (color == "yellow")
	{
		system("color 6f");
	}
	else if (color == "light yellow")
	{
		system("color e0");
	}
	else if (color == "gray")
	{
		system("color 8f");	
	}
	else
	{
		system("color 0f");
	}
}

void plane ( int planex, int planey)
{
	locate ( planex, planey-1);	
	cout << "A";
	locate ( planex, planey);
	cout << "H";
	locate ( planex-1, planey);
	cout << "=";
	locate ( planex-2, planey);
	cout << "<";
	locate ( planex+1, planey);
	cout << "=";
	locate ( planex+2, planey);
	cout << ">";
	locate ( planex, planey+1);
	cout << "Y";
	locate ( planex+1, planey+2);
	cout << ">";
	locate ( planex-1, planey+2);
	cout << "<";
	locate ( planex, planey+2);
	cout << "A";
	locate (0, 0);
}

void drawbarrier (int barrierx,int barriery)
{
	locate (barrierx, barriery);
	cout << "X";
	locate (barrierx+1, barriery);
	cout << "=";
	locate (barrierx-1, barriery);
	cout << "=";
	locate (barrierx+2, barriery);
	cout << ">";
	locate (barrierx-2, barriery);
	cout << "<";	
}

void drawenemy (int enemyx,int enemyy)
{
	locate (enemyx-2, enemyy);
	cout << "-=H=-";	
}

void subtitle(string sub, int startofscreen)
{
	string error="Not an odd number of characters";
	string clear="|                                                 |";
	
	locate (startofscreen+26-clear.length()/2, 28);
	cout << clear;
	
	if (sub.length()%2!=0)
	{
		locate (startofscreen+26-sub.length()/2, 28);
		cout << sub;
	}
	else 
	{
		locate (startofscreen+26-error.length()/2, 28);
		cout << error ;
	}
	locate (0, 0);
}

void save(int score, int highscore)
{
	if(score >= highscore){
		fstream scorefile("userinfo.txt");
  		scorefile << score;
	    scorefile.close();
	}
}

void quit(int score, int highscore, int startofscreen)
{
	changecolor("black");
	save (score, highscore);
	subtitle ("GAME QUIT", startofscreen);
	Sleep(1500);
}

//-------------------------------------------------------------
//#############################################################
//-------------------------------------------------------------

int main()
{	
    srand(time(0));
	fstream scorefile("userinfo.txt");
	int startofscreen=15,
	sideofscreen=55;

	bool dead=false;
	long long int score=0, highscore;
	int
	guncooldown=0, 
	enemycooldown=0, 
	hitreg=0, 
	hitscan=0, 
	pausetimer=0, 
	bulletid=20,
	barrierid=20, 
	enemyid=30,
	numberofbullets=2, 
	pdx=startofscreen+26,
	pdy=22,
	enemymovecounter=0,
	enemygencounter=0,
	barriergencounter=0;
	char SlowMo = 'F';
	//-------------INTs--------------------------------
	
	scorefile >> highscore;

	bulletclass bullet[2][10000];
	barrierclass barrier[10000];
	enemyclass enemy[10000];
	int nextscorecooldown=0, sleeptimer=16;
//	
	START:
	startofscreen=15,
	sideofscreen=55;
	locate (startofscreen+sideofscreen, 2);
	cout << "                   ";
	locate (startofscreen+sideofscreen, 0);
	cout << "                   ";	
	locate (0, 0);		
	changecolor("blue");
	guncooldown=0, 
	enemycooldown=0, 
	hitreg=0, 
	hitscan=0, 
	pausetimer=0, 
	bulletid=20,
	barrierid=20, 
	enemyid=30,
	enemymovecounter,
	numberofbullets=2, 
	pdx=startofscreen+26,
	pdy=22,
	enemygencounter=0,
	barriergencounter=0;
	score=0, dead=false;
	
	
	for (int i=0; i<10000; i++)
	{
		barrier[i].x=startofscreen+randomnumber();
		barrier[i].alive=false;
	}
	
	for (int i=0; i<10000; i++)
	{
		enemy[i].x=startofscreen+randomnumber();
		enemy[i].alive=false;
		enemy[i].direction=rand()%2;
		enemy[i].move=0;
	}
	
	for (int i=0; i<10000; i++)
	{
		bullet[0][i].alive = false;
		bullet[1][i].alive = false;
	}

	nextscorecooldown=0, sleeptimer=16;
	
	while (dead!=1)
	{

//		if (score%20==0 && sleeptimer!=1){
//			sleeptimer--;	
//		}
		Sleep(sleeptimer);
		score++;
		if(score >= highscore){
			highscore = score;	
		}	
	
		
		locate (0, 0);
		for (int ic=0; ic<30;ic++){
			cout << "               ||                                                 ||\n";
		}
		
		locate (startofscreen+sideofscreen, 2);
		cout << "score: " << score/10;
		
		locate (startofscreen+sideofscreen, 0);
		cout << "highscore: " << highscore/10;

		locate (0, 0);
//---end of PERM block

		if (GetKeyState('A') < 0 && GetKeyState('D') < 0)
		{	
		}
		else if (GetKeyState('A') < 0){
			if (pdx > startofscreen+4){
				if (pdx==startofscreen+5)
				{
					dead=1;
				}
				pdx--;
			}
		}
		else if (GetKeyState('D') < 0){
			if (pdx < startofscreen+48){
				if (pdx==startofscreen+47)
					{
						dead=1;
					}
				pdx++;
			}
		}

		if (GetKeyState('W') < 0 && GetKeyState('S') < 0)
		{	
		}
		else if (GetKeyState('W') < 0){
			if (pdy > 10){
				pdy--;
			}
		}
		else if (GetKeyState('S') < 0){
			if (pdy < 23){
					pdy++;			
			}
		}

		if (score>50)
		{		
			plane (pdx, pdy);
		}
		else 
		{
			if (score%2==0)
			{
				plane (pdx, pdy);
			}
			subtitle("SPAWN PROTECTION ACTIVE", startofscreen);
		}	
		
		if (GetKeyState(' ') < 0){
			if (guncooldown==8){
				guncooldown=0;
				bullet[0][bulletid].x = pdx+2;
				bullet[0][bulletid].y = pdy;
				bullet[0][bulletid].alive=true;
			 	bullet[1][bulletid].x = pdx-2;  
				bullet[1][bulletid].y = pdy;
				bullet[1][bulletid].alive=true;
				bulletid++;
			}	
		}
			
		if (GetKeyState(SlowMo) < 0){		
			if (sleeptimer<100)
			{
				sleeptimer+=10;
			}
		}
		else
		{
			if (sleeptimer>16)
			{
				sleeptimer-=10;
			}
		}
		
		if (GetKeyState('P') < 0){		
			if (pausetimer < 15)
			{
			locate(startofscreen+sideofscreen, 29);
			cout << "PAUSING";
			locate(startofscreen+sideofscreen+7+pausetimer/5, 29);
			cout << ".";
			pausetimer++;
			}
			else 
			{
	
				pausetimer=0;
			}
			if (pausetimer>=15)
			{
				goto PAUSE;
			}
		}
		else 
		{
			locate(startofscreen+sideofscreen, 29);
			cout << "                             "; 
			pausetimer=0;
		}
		
		UNPAUSE:
	
		//enemy functionality
		for (int j=(enemyid-20); j<=enemyid; j++)
		{
			if (enemy[j].y>=0 && enemy[j].y < 30 )
			{
				if (enemy[j].alive == true)
				{
					if (enemy[j].move==2)
					{
						if (enemy[j].direction==0)
						{
							if (enemy[j].x > startofscreen+4)
							{
								enemy[j].x-=1;
							}
							else
							{
								enemy[j].direction=1;
							}
						}
						else
						{
							if (enemy[j].x < sideofscreen+startofscreen-7)
							{
								enemy[j].x+=1;
							}
							else
							{
								enemy[j].direction=0;
							}
						}
						enemy[j].move=0;
					}
					else
					{
						enemy[j].move++;
					}
					drawenemy(enemy[j].x, enemy[j].y);	
				}
				enemy[j].y++;
			}
			else
			{
				enemy[j].alive=false;	
			}
		}
		locate (0, 0);

		//barrier functionality
		for (int j=(barrierid-20); j<=barrierid; j++)
		{
			if (barrier[j].y>=0 && barrier[j].y < 30 )
			{
				if (barrier[j].alive == true )
				{
					drawbarrier(barrier[j].x, barrier[j].y);
				}
				barrier[j].y++;
			}
			else
			{
				barrier[j].alive=false;	
			}
		}
		locate (0, 0);

		//bullet functionality
		for (int j=(bulletid-10); j<=bulletid; j++)
		{
			for (int h=0; h<numberofbullets; h++)
			{
				if (bullet[h][j].y>0)
				{		
					bullet[h][j].y--;
					
					if (bullet[h][j].alive == true)
					{
						locate (bullet[h][j].x, bullet[h][j].y);
						cout << "|";
					}
				}
				else
				{
					bullet[h][j].alive=false;	
				}
			}
		}
		locate (0, 0);	


		if (score > 50)
		{
			for (int xmult=-2; xmult<3; xmult++)
			{
				for (int j=(barrierid-20); j<=barrierid; j++)
				{
					if (barrier[j].alive==true )
					{
						if (barrier[j].y+1 == pdy && barrier[j].x+xmult==pdx)
						{
							dead=true;
						}
						for (int planexmult=-2; planexmult<3; planexmult++)
						{
							for (int ymult=0; ymult<2; ymult++)
							{
								if (barrier[j].y-ymult == pdy && barrier[j].x+xmult==pdx+planexmult)
								{
									dead=true;
								}
							}
						}
						for (int planexmult=-1; planexmult<2; planexmult++)
						{
							if (barrier[j].y-2 == pdy && barrier[j].x+xmult==pdx+planexmult)
							{
								dead=true;
							}
						}	
					}
				}
			
				//enemy x plane hitreg functionality
				for (int j=(enemyid-20); j<=enemyid; j++)
				{
					if (enemy[j].alive==true )
					{
						if (enemy[j].y+1 == pdy && enemy[j].x+xmult==pdx)
						{
							dead=true;
						}
						for (int planexmult=-2; planexmult<3; planexmult++)
						{
							for (int ymult=0; ymult<2; ymult++)
							{
								if (enemy[j].y-ymult == pdy && enemy[j].x+xmult==pdx+planexmult)
								{
									dead=true;
								}
							}
						}
						for (int planexmult=-1; planexmult<2; planexmult++)
						{
							if (enemy[j].y-2 == pdy && enemy[j].x+xmult==pdx+planexmult)
							{
								dead=true;
							}
						}
					}
				}
			}
		}



		//bullet hitreg functionality		
		for (int j=(bulletid-10); j<=bulletid; j++)
		{
			for (int h=0; h<numberofbullets; h++)
			{
				if (bullet[h][j].y>0 && bullet[h][j].alive==true)
				{		
					for (int xmult=-2; xmult<3; xmult++)
					{
						for (int i=(barrierid-20); i<=barrierid; i++)
						{
							if (barrier[i].y > 0 && barrier[i].alive==true)
							{
								if (bullet[h][j].x == barrier[i].x+xmult && bullet[h][j].y == barrier[i].y )
								{
									bullet[h][j].alive=false;
								}
								if (bullet[h][j].x == barrier[i].x+xmult && bullet[h][j].y == barrier[i].y+1 )
								{
									bullet[h][j].alive=false;
								}
							}
						}
						for (int i=(enemyid-20); i<=enemyid; i++)
						{
							if (enemy[i].y > 0 && enemy[i].alive==true)
							{
								if (bullet[h][j].x == enemy[i].x+xmult && bullet[h][j].y == enemy[i].y )
								{
									enemy[i].alive=false;
								}
								if (bullet[h][j].x == enemy[i].x+xmult && bullet[h][j].y == enemy[i].y+1 )
								{
									enemy[i].alive=false;
								}
							}
						}
					}
				}
				else
				{
					bullet[h][j].alive=false;
				}
			}
		}
		
	
	
		if (guncooldown != 8)
		{
			guncooldown++;
		}
		if (score > 5 )
		{
			if (barriergencounter>=rand()%200+1)
			{
				barrier[barrierid].alive=true;
				barrier[barrierid].y=0;
				barrier[barrierid-16].alive=false;
				barrierid++;
				barriergencounter=0;
			}
			else
			{
				barriergencounter++;
			}

			if (enemygencounter>=rand()%100+1)
			{	
				if (barriergencounter!=0)
				{	
					enemy[enemyid].alive=true;
					enemy[enemyid].y=0;
					enemy[enemyid-30].alive=false;
					enemyid++;
					enemygencounter=0;
				}
			}
			else
			{
				enemygencounter++;
			}
		}
//---end main game WHILE
	}
	
	plane (pdx, pdy);
	
	subtitle("GAME OVER", startofscreen);	
	save (score, highscore);

	changecolor("red");
	Sleep(2000); 
	
	if (GetKeyState('X') < 0){
		quit(score, highscore, startofscreen);
		return 0;
	}

	dead=0;
	goto START;
	
	//INF PAUSE LOOP
	if (1==2)
	{
		PAUSE:
		plane (pdx, pdy);
		changecolor("gray");
		locate(startofscreen+sideofscreen, 29);
		cout << "PROGRESS SAVED. Press P to unpause and X to exit";
		subtitle ("||  PAUSE", startofscreen);
		locate(0, 0);
		save (score, highscore);
		Sleep(500); 
		while(true)
		{
			if (GetKeyState('P') < 0){	
				locate(startofscreen+sideofscreen, 29);
				cout << "                                                       ";
				subtitle ("|                                                 |", startofscreen);
				locate(0, 0);
				changecolor("blue");	
				Sleep(100);
				goto UNPAUSE;
			}
			if (GetKeyState('X') < 0){	
			quit(score, highscore, startofscreen);
			return 0;
			}
		}
	}
	
	
	

}