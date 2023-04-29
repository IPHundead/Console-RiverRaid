#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <thread>
//#include "atariplane.ico"

//#define _WIN32_WINNT 0x0500

void resizeatari()
{
	HWND console = GetConsoleWindow(); 
	MoveWindow(console, 150, 100, 1185, 662, TRUE);
}


const short int fps=30;
void fpswait()
{	
	Sleep(1000/fps); 	
}

using namespace std;

const string scorefilelocation = "data/score.txt";
const string settingsfilelocation = "data/settings.txt";
const string skinfilelocation = "data/cosmetics.txt";


class file_read
{
	public:
		string stringtype(string filename, int line)
		{
			line-=1;
			string textsave="", myText;
	
			int shouldread=-1, linecounter=0, linenumber=0;
	
			ifstream openedfile(filename);
			while (getline (openedfile, myText)) 
			{
				textsave="";
				linecounter++;
				shouldread=-1;
				if (linenumber==line)
				{
					for (int i=0; i<myText.size(); i++)
					{
				  		if (shouldread==1)
				  		{
				  			if (myText[i] != '"')
				  			{
								textsave+=myText[i];
							}
						}
				  		if (myText[i]=='"')
				  		{
				  			shouldread*=-1;
				  		}	
					}
						openedfile.close();
						return textsave;
				}
				linenumber++;
			}
			openedfile.close();
			return "?";
		}
		
		char chartype(string filename, int line)
		{
			char x=stringtype(filename, line)[0];	
			return x;
		}
		
		int inttype(string filename, int line)
		{
			try
			{
				return stoi(stringtype(filename, line));
			}
			catch(...)
			{
				system("CLS");
				system("color 0f");
				cerr << "ERROR! please extract the file before running the program, or check if the data files are correct.\n\n (true error : STOI function returned exception. \n possible causes : \n file does not exist, \n files have been changed by the user, \n the folder the program is running is has not been extracted, \n or there is a string literal in the place of an integer in a data file.)\n [Error id #1] \n press any key to continue...";
				getch();
				exit(0);
			}
		}
		
		long long int long2type(string filename, int line)
		{
			return stoll(stringtype(filename, line));
		}
};

class file_write
{
	public:
		int chartype(string filename, int reqline, char input)
		{
			string textsaved="";

			string linetext="", linetext2="";
			int linenumber=0, totalsize=0;
			fstream openedfile(filename);
			while (getline (openedfile, linetext))
			{
				linenumber++;
				if (linenumber==reqline)
				{
					textsaved = linetext;
					break;
				}
				else
				{
					
					totalsize+=linetext.size();	
				}
				linetext="";
				totalsize+=2;
			}
			string lineclear="";
			for (int i=0; i<textsaved.size(); i++)
			{
				lineclear.append(" ");
			}
			openedfile.close();
			fstream writefile(filename);
			writefile.seekp(totalsize);
			writefile << lineclear;
			writefile.seekp(totalsize);
			for (int i=0; i<textsaved.size(); i++)
			{
				if (textsaved[i] != '"')
				{
					writefile << textsaved[i];
				}
				else
				{
					break;
				}	
			}
			writefile << "\"" << input <<  "\"";	
			writefile.close();
			return 0;
		}

		
	
		
		int long2type(string filename, int reqline, long input)
		{
			string textsaved="";

			string linetext="", linetext2="";
			int linenumber=0, totalsize=0;
			fstream openedfile(filename);
			while (getline (openedfile, linetext))
			{
				linenumber++;
				if (linenumber==reqline)
				{
					textsaved = linetext;
					break;
				}
				else
				{	
					totalsize+=linetext.size();	
				}
				linetext="";
				totalsize+=2;
			}
			string lineclear="";
			for (int i=0; i<textsaved.size(); i++)
			{
				lineclear.append(" ");
			}
			openedfile.close();
			fstream writefile(filename);
			writefile.seekp(totalsize);
			writefile << lineclear;
			writefile.seekp(totalsize);
			for (int i=0; i<textsaved.size(); i++)
			{
				if (textsaved[i] != '"')
				{
					writefile << textsaved[i];
				}
				else
				{
					break;
				}	
			}
			writefile << "\"" << input <<  "\"";	
			writefile.close();
			return 0;
		}
		
		int inttype(string filename, int reqline, int input)
		{
			long2type(filename, reqline, input);
			return 0;
		}

};

file_write file_write;
file_read file_read;


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
	bool isjet=false;
};



struct ammo{
	int max=30,current=max, reload=100;
	long long int isave=0;
	bool turn=0;
	
	void resetammo()
	{
		max=30, current=max, reload=100, isave=0, turn=0;
	}
};

struct planeclass:ammo{
	int x;
	int y;
	bool dead=false;
	int guncooldown=0;
};
void clearinputbuffer()
{
	cin.clear();
	cin.ignore(256, '\n');
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void ShowConsoleCursor (bool showFlag)
{
    HANDLE out = GetStdHandle (STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo (out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo (out, &cursorInfo);
}

void locate ( int x, int y )
{
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition ( GetStdHandle ( STD_OUTPUT_HANDLE ), position );
}

int randomnumber(int sizeofscreen)
{
	int randn;
	randn=rand()%sizeofscreen+1;
	if (randn<4)
	{
		return 4;
	}
	else if (randn > sizeofscreen-5)
	{
		return sizeofscreen-5;
	}
	return randn;
}

void changecolor(string color)
{
	if (color == "blue")
	{
		system("color 9f");
	}
	else if (color == "dark blue")
	{
		system("color 1f");
	}
	else if (color == "green")
	{
		system("color A0");
	}
	else if (color == "dark green")
	{
		system("color 2f");
	}
	else if (color == "white")
	{
		system("color f0");
	}
	else if (color == "aqua")
	{
		system("color bf");
	}
	else if (color == "dark aqua")
	{
		system("color 3f");
	}
	else if (color == "pink")
	{
		system("color cf");
	}
	else if (color == "purple")
	{
		system("color 50");
	}
	else if(color == "light purple")
	{
		system("color d0");
	}
	else if (color == "red")
	{
		system("color 4f");
	}
	else if (color == "black")
	{
		system("color 0f");
	}
	else if (color == "dark yellow")
	{
		system("color 6f");
	}
	else if (color == "yellow")
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

void drawplane ( int planex, int planey)
{
	locate ( planex, planey-1);	cout <<    "A";
	locate ( planex-2, planey); cout <<  "<=H=>";
	locate ( planex, planey+1); cout <<    "Y";
	locate ( planex-1, planey+2); cout << "<A>";
	locate (0, 0);
}

void drawbarrier (int barrierx,int barriery)
{	
	locate (barrierx-2, barriery);
	cout << "<=X=>";
}

void drawenemy (int enemyx,int enemyy)
{
	locate (enemyx-2, enemyy);
	cout << "-=H=-";	
}

void drawjet (int enemyx,int enemyy, bool direc)
{
	locate (enemyx-2, enemyy);
	
	if (direc==0)
	{
		cout << "-==-X";
	}
	else
	{
		cout << "X-==-";
	}
}

void subtitle(string sub, int startofscreen, int sizeofscreen, string river)
{
	string error="Not an odd number of characters";

	
	locate (startofscreen+sizeofscreen/2-river.length()/2, 28);
	cout << river;
	
	if (sub.length()%2!=0)
	{
		locate (startofscreen+sizeofscreen/2-sub.length()/2, 28);
		cout << sub;
	}
	else 
	{
		locate (startofscreen+sizeofscreen/2-error.length()/2, 28);
		cout << error ;
	}
	locate (0, 0);
}

void save(int score, int highscore)
{
	if(score >= highscore){
		fstream scorefile(scorefilelocation);
  		scorefile << score;
	    scorefile.close();
	}
}

void quit(int score, int highscore, int startofscreen, int sizeofscreen, string river, string mode)
{
	changecolor("black");
	if (mode=="campaign")
	{
		save (score, highscore);
	}
	subtitle ("GAME QUIT", startofscreen, sizeofscreen, river);
	Sleep(1000);
}

void sortthis(int tarr[], int tarrsize)
{
	int temp=0;
	for (int j=0; j<=tarrsize;j++)
	{
		for (int i=1; i<tarrsize; i++)
		{
			if (tarr[i] > tarr[i-1])
			{
				temp=tarr[i-1];
				tarr[i-1]=tarr[i];
				tarr[i]=temp;
			}
		}
	}
}

string easytohard(int number, int maxnumber)
{
	if (number < maxnumber/5)
	{
		return "BOT";
	}
	else if (number >= maxnumber/5 && number < (maxnumber/5)*2)
	{
		return "EASY";
	}
	else if (number >= (maxnumber/5)*2 && number < (maxnumber/5)*3)
	{
		return "MEDIUM";
	}
	else if (number >= (maxnumber/5)*3 && number < (maxnumber/5)*4)
	{
		return "HARD";
	}
	else if (number >= (maxnumber/5)*4 && number < maxnumber)
	{
		return "EXPERT";
	}
	else if (number == maxnumber)
	{
		return "IMPOSSIBLE";
	}
	else
	{
		return "? (CHEATS DETECTED)";
	}
}

void drawlogo(int x, int y)
{
	string atarilogo[9+1] = {
	R"(        ____        ___________        ____            ________     _______ )",
	R"(       / \ \\      |          ||      / \ \\          |  ___  \\   |      ||)",
	R"(      / //\ \\     |___    ___||     / //\ \\         | ||  | ||   |_    _||)",
	R"(     / //  \ \\        |  ||        / //  \ \\        | ||__| ||     |  ||  )",
	R"(    / //    \ \\       |  ||       / //    \ \\       |     __//     |  ||  )",
	R"(   / //------\ \\      |  ||      / //------\ \\      | |\ \\        |  ||  )",
	R"(  / /=========\ \\     |  ||     / /=========\ \\     | ||\ \\      _|  ||_ )",
	R"( / //          \ \\    |  ||    / //          \ \\    | || \ \\    |      ||)",
	R"(/_//            \_\\   |__||   /_//            \_\\   |_||  \_\\   |______||)"};
	
	for (int i=0; i<9; i++)
	{
		locate (x, y+i);
		cout << atarilogo[i];
		
	}
}

class bossclass
{
	public:
		int x;
		int y;
		int bulletcountdown;
		int direction;
};

//-------------------------------------------------------------
//#############################################################
//-------------------------------------------------------------



	
bool showloadingscreen=file_read.inttype(settingsfilelocation, 9);
bool showmenuloadingscreen=file_read.inttype(settingsfilelocation, 10);

char SlowMo = 'F';
char controlright = file_read.chartype(settingsfilelocation, 1);
char controlleft = file_read.chartype(settingsfilelocation, 2);
char controlup = file_read.chartype(settingsfilelocation, 3);
char controldown = file_read.chartype(settingsfilelocation, 4);
char controlfire = file_read.chartype(settingsfilelocation, 5);
char controlpause = file_read.chartype(settingsfilelocation, 6);
char controlreload = file_read.chartype(settingsfilelocation, 7);


int game(const string mode, const bool infinite, int level, string defaultcolor, const int startofscreen, const int sizeofscreen, int sleeptimer)
{	
	int sleepsaver=sleeptimer;
	
	srand(time(0));
	fstream scorefile(scorefilelocation);
	int endofscreen=startofscreen+sizeofscreen;
	int sleeptimersave=sleeptimer;
	bool dead=false, stopspawning=false;
	long long int score=0, highscore;
	int 
	numberofbullets=0, 
	enemygenrandom=10,
	enemygendivider=25,
	barriergenrandom=10,
	barriergendivider=30;
	
	
	

	
	scorefile >> highscore;

	
	int ebgdd=enemygendivider-barriergendivider;
	int numberofplanes=0;
	
	
	if (mode=="twoplayerclassic")
	{
		numberofplanes=2;
	}
	else
	{
		numberofplanes=1;
	}
	
	planeclass plane[numberofplanes+1];
	numberofbullets=numberofplanes*2;
	
	bulletclass bullet[numberofbullets][10000];
	static barrierclass barrier[10000];
	static enemyclass enemy[10000];
	static ammo ammo;
	
	string river="       ||";
	for (int i=0; i<sizeofscreen-4; i++)
	{
		river = river + " ";
	}
	river=river+"||       ";

	system("CLS");
	int textlocation = endofscreen+1+7;
	while (true)
	{
		//used to be START:
		sleeptimer=sleepsaver;
		//arguments
		enemygendivider=40, 
		barriergendivider=50,
		ebgdd=enemygendivider-barriergendivider;
		if (ebgdd < 0)
		{
			ebgdd*=-1;
		}	
		changecolor(defaultcolor);

		//arguments end

		locate (0, 0);
		int	enemycooldown=0, 
		hitreg=0, 
		hitscan=0, 
		pausetimer=0, 
		bulletid=20,
		barrierid=20, 
		enemyid=30,
		enemymovecounter, 
		enemygencounter=0,
		barriergencounter=0,
		difficulty=0;
		score=0, dead=false;
		
		
		for (int i=0; i<numberofplanes;i++)
		{
			plane[i].resetammo();
			plane[i].y=22;
		}
		
		if (numberofplanes==1)
		{
			plane[0].x=startofscreen+sizeofscreen/2;
		}
		else
		{
			plane[0].x=startofscreen+sizeofscreen/2-sizeofscreen/4;
			plane[1].x=startofscreen+sizeofscreen/2+sizeofscreen/4;
		}
		
		
		for (int i=0; i<10000; i++)
		{
			barrier[i].x=startofscreen+randomnumber(sizeofscreen);
			barrier[i].alive=false;
		}
		
		//generating enemies and jets
		for (int i=0; i<10000; i++)
		{
			if (rand()%5==1)
			{
				enemy[i].isjet=true;
			}
			enemy[i].direction=rand()%2;
			
			if (enemy[i].isjet==true)
			{
				if (enemy[i].direction==0)
				{
					enemy[i].x=endofscreen-5+7-2 -10+rand()%11;
				}
				else
				{
					enemy[i].x=startofscreen+4-7+2 +10-rand()%11;
				}
			}
			else
			{
				enemy[i].x=startofscreen+randomnumber(sizeofscreen);
			}
			
			
			enemy[i].alive=false;		
			enemy[i].move=0;
		}
		

		
		for (int h=0; h<numberofbullets; h++)
		{
			for (int i=0; i<10000; i++)
			{	
				bullet[h][i].alive = false;
			}
		}
		
		
	
		
		while (dead!=true)
		{

			thread fpsregulator(fpswait);
			if (infinite==true)
			{
				if (score%150==0 && score!=0)
				{
					if (barriergendivider>10)
					{
						barriergendivider--;
						difficulty++;
					}
					if (enemygendivider>10+ebgdd)
					{
						enemygendivider--;
						difficulty++;
					}
				}
			}	

			//Sleep(sleeptimer);
			score++;
			if(score >= highscore){
				highscore = score;	
			}	
		
			
			locate (0, 0);
			
			
//			for (int ic=0; ic<30;ic++){
//				cout <<"                                                                                                                       |";
//			}
			
			for (int ic=0; ic<30;ic++){
				locate(startofscreen-7, ic); //-7 is for the empty space before river.
				cout << river;

			}
			
			
			
			
			if (mode=="campaign")
			{
				locate (textlocation, 0);
				cout << "                   ";	
				locate (textlocation, 0);
				cout << "HIGHSCORE: " << highscore/10;
			}
			
			locate (textlocation, 2);
			cout << "                   ";
			locate (textlocation, 2);
			cout << "SCORE: " << score/10;
			
			if (infinite==true)
			{
				locate (textlocation, 8);
				cout << "                          ";
				locate (textlocation, 8);
				cout << "DIFFICULTY: " << easytohard(difficulty, 77);
			}
			
			locate (textlocation, 5);
			cout << "            ";
			if (mode!="twoplayerclassic")
			{
				locate (textlocation, 5);
				cout << "AMMO: " << plane[0].current;
			}
			else
			{
				locate (textlocation, 5);
				cout << "PLANE 1 AMMO:   ";
				locate (textlocation + 14, 5);
				cout << plane[0].current;
				
				
				locate (textlocation, 6);
				cout << "PLANE 2 AMMO:   ";
				locate (textlocation+14, 6);
				cout << plane[1].current;
			}
			
			locate (0, 0);
		 	//---end of PERM block
		 	
		 	if (mode == "twoplayerclassic")
			{
				if (GetKeyState(controlleft) < 0 && GetKeyState(controlright) < 0)
				{	
				}
				if(GetKeyState(VK_LEFT) < 0 && GetKeyState(VK_RIGHT) < 0)
				{
				}
				if (GetKeyState(controlleft) < 0){
					if (plane[0].x > startofscreen+4){
						if (plane[0].x==startofscreen+5)
						{
							dead=1;
						}
						plane[0].x--;
					}
				}
				if (GetKeyState(controlright) < 0){
					if (plane[0].x < endofscreen-5){
						if (plane[0].x==endofscreen-6)
							{
								dead=1;
							}
						plane[0].x++;
					}
				}
				if (GetKeyState(VK_LEFT) < 0){
					if (plane[1].x > startofscreen+4){
						if (plane[1].x==startofscreen+5)
						{
							dead=1;
						}
						plane[1].x--;
					}
				}
				if (GetKeyState(VK_RIGHT) < 0){
					if (plane[1].x < endofscreen-5){
						if (plane[1].x==endofscreen-6)
							{
								dead=1;
							}
						plane[1].x++;
					}
				}
		
				if (GetKeyState(controlup) < 0 && GetKeyState(controldown) < 0)
				{	
				}
				if (GetKeyState(VK_UP) < 0 && GetKeyState(VK_DOWN) < 0)
				{	
				}
				if (GetKeyState(controlup) < 0){
					if (plane[0].y > 10){
						plane[0].y--;
					}
				}
				if (GetKeyState(controldown) < 0){
					if (plane[0].y < 23){
						plane[0].y++;			
					}
				}
				if (GetKeyState(VK_UP) < 0){
					if (plane[1].y > 10){
						plane[1].y--;
					}
				}
				if (GetKeyState(VK_DOWN) < 0){
					if (plane[1].y < 23){
						plane[1].y++;			
					}
				}
			}
			else if (mode == "twoplayerconflict")
			{
				if (GetKeyState(controlleft) < 0 && GetKeyState(controlright) < 0)
				{
					if (GetKeyState(controlup) < 0 && GetKeyState(controldown) < 0)
					{
						pausetimer=150;
					}
				}
				if(GetKeyState(VK_LEFT) < 0 && GetKeyState(VK_RIGHT) < 0)
				{
					if (GetKeyState(VK_UP) < 0 && GetKeyState(VK_DOWN) < 0)
					{
						pausetimer=150;
					}
				}
				if (GetKeyState(controlleft) < 0 && GetKeyState(VK_LEFT) < 0){
					if (plane[0].x > startofscreen+4){
						if (plane[0].x==startofscreen+5)
						{
							dead=1;
						}
						plane[0].x--;
					}
				}
				if (GetKeyState(controlright) < 0 && GetKeyState(VK_RIGHT) < 0){
					if (plane[0].x < endofscreen-5){
						if (plane[0].x==endofscreen-6)
							{
								dead=1;
							}
						plane[0].x++;
					}
				}
		
				if (GetKeyState(controlup) < 0 && GetKeyState(controldown) < 0)
				{
					if (GetKeyState(controlleft) < 0 && GetKeyState(controlright) < 0)
					{
						pausetimer=150;
					}
				}
				if (GetKeyState(VK_UP) < 0 && GetKeyState(VK_DOWN) < 0)
				{
					if(GetKeyState(VK_LEFT) < 0 && GetKeyState(VK_RIGHT) < 0)
					{
						pausetimer=150;
					}
				}
				if (GetKeyState(controlup) < 0 && GetKeyState(VK_UP) < 0){
					if (plane[0].y > 10){
						plane[0].y--;
					}
				}
				if (GetKeyState(controldown) < 0 && GetKeyState(VK_DOWN) < 0){
					if (plane[0].y < 23){
							plane[0].y++;			
					}
				}
			}
			else
			{
				if (GetKeyState(controlleft) < 0 && GetKeyState(controlright) < 0)
				{	
				}
				if(GetKeyState(VK_LEFT) < 0 && GetKeyState(VK_RIGHT) < 0)
				{
				}
				if (GetKeyState(controlleft) < 0 || GetKeyState(VK_LEFT) < 0){
					if (plane[0].x> startofscreen+4){
						if (plane[0].x==startofscreen+5)
						{
							dead=1;
						}
						plane[0].x--;
					}
				}
				if (GetKeyState(controlright) < 0 || GetKeyState(VK_RIGHT) < 0){
					if (plane[0].x < endofscreen-5){
						if (plane[0].x==endofscreen-6)
						{
							dead=1;
						}
						plane[0].x++;
					}
				}
						             
		
				if (GetKeyState(controlup) < 0 && GetKeyState(controldown) < 0)
				{	
				}
				if (GetKeyState(VK_UP) < 0 && GetKeyState(VK_DOWN) < 0)
				{	
				}
				if (GetKeyState(controlup) < 0 || GetKeyState(VK_UP) < 0){
					if (plane[0].y > 10){
						plane[0].y--;
					}
				}
				if (GetKeyState(controldown) < 0 || GetKeyState(VK_DOWN) < 0){
					if (plane[0].y < 23){
						plane[0].y++;			
					}
				}
			}
			
			for (int pc=0; pc<numberofplanes; pc++)
			{
				if (score>50)
				{	
					if (numberofplanes==1)
					{
						drawplane (plane[pc].x, plane[pc].y);
					}
					else
					{
						drawplane (plane[pc].x, plane[pc].y);
						locate(plane[pc].x, plane[pc].y);
						cout << pc+1;
					}
				}
				else 
				{
					if (score%2==0)
					{
						if (numberofplanes==1)
						{
							drawplane (plane[pc].x, plane[pc].y);
						}
						else
						{
							drawplane (plane[pc].x, plane[pc].y);
							locate(plane[pc].x, plane[pc].y);
							cout << pc+1;
						}
					}
					//subtitle("SPAWN PROTECTION ACTIVE", startofscreen, sizeofscreen, river);
				}
			}
			
	
			if (GetKeyState(VK_OEM_COMMA) < 0 && numberofplanes==2)
			{
				if (plane[1].guncooldown%8==0 && plane[1].current !=0){
					plane[1].guncooldown++;
					bullet[2][bulletid].x = plane[1].x+2;
					bullet[2][bulletid].y = plane[1].y;
					bullet[2][bulletid].alive=true;
				 	bullet[3][bulletid].x = plane[1].x-2;  
					bullet[3][bulletid].y = plane[1].y;
					bullet[3][bulletid].alive=true;
					bulletid++;
					plane[1].current--;
				}
			}
			if (GetKeyState(controlfire) < 0)
			{
				if (plane[0].guncooldown%8==0 && plane[0].current !=0){
					plane[0].guncooldown++;
					bullet[0][bulletid].x = plane[0].x+2;
					bullet[0][bulletid].y = plane[0].y;
					bullet[0][bulletid].alive=true;
				 	bullet[1][bulletid].x = plane[0].x-2;  
					bullet[1][bulletid].y = plane[0].y;
					bullet[1][bulletid].alive=true;
					bulletid++;
					plane[0].current--;
				}	
			}
			
			for (int pc=0;pc<2;pc++)
			{
				if (plane[pc].guncooldown % 8 != 0)
				{
					plane[pc].guncooldown++;
				}	
			}
			
			if (GetKeyState(controlreload) < 0){
				if (plane[0].current !=0 && plane[0].current!=plane[0].max)
				{
					plane[0].current=0;
				}	
			}
			
			if (GetKeyState(VK_OEM_PERIOD) < 0){
				if (plane[1].current !=0 && plane[1].current!=plane[1].max)
				{
					plane[1].current=0;
				}	
			}
			
			for (int pc=0; pc<numberofplanes; pc++)
			{
				if (plane[pc].current == 0)
				{
					if (plane[pc].turn == 0)
					{
						plane[pc].isave=score;
						plane[pc].turn=1;
					}
					subtitle("RELOADING", startofscreen, sizeofscreen, river);
					if (score-200>=plane[pc].isave)
					{
						plane[pc].current=plane[pc].max;
						plane[pc].turn=0;
						
					}
				}
			}


		
			
			if (pausetimer>=15)
			{
				for (int pc=0; pc<numberofplanes; pc++)
				{
					if (numberofplanes==1)
					{
						drawplane (plane[pc].x, plane[pc].y);
					}
					else
					{
						drawplane (plane[pc].x, plane[pc].y);
						locate(plane[pc].x, plane[pc].y);
						cout << pc+1;
					}
				}
				changecolor("gray");
				locate(endofscreen, 29);
				cout << "GAME PAUSED.";
				if (pausetimer>100)
				{
					subtitle ("Do not hold all the keys at the same time", startofscreen, sizeofscreen, river);
				}
				else
				{
					subtitle ("||  PAUSE", startofscreen, sizeofscreen, river);
				}
				locate(0, 0);
				save (score, highscore);
				Sleep(500); 
				while(true)
				{
					if (GetKeyState(controlpause) < 0 || GetKeyState(VK_SPACE) < 0)
					{	
						locate(endofscreen, 29);
						cout << "                                                       ";

						locate(endofscreen, 29);
						cout << "3...";
						locate(0, 0);
						Sleep(1000);
						locate(endofscreen, 29);
						cout << "2...";
						locate(0, 0);
						Sleep(1000);
						locate(endofscreen, 29);
						cout << "1...";
						locate(0, 0);
						Sleep(1000);
						locate(endofscreen, 29);
						cout << "    ";
						locate(0, 0);
						pausetimer=0;
					//	subtitle (river, startofscreen, sizeofscreen, river);
						locate(0, 0);
						changecolor(defaultcolor);	
						Sleep(100);
						break;
					}
					if (GetKeyState(VK_ESCAPE) < 0)
					{	
						quit(score, highscore, startofscreen, sizeofscreen, river, mode);
						fpsregulator.join();	
						return 0;
					}
				}
			}
			else if (pausetimer==0)
			{
				locate(endofscreen, 29);
				cout << "                ";
			}
		
				
			if (GetKeyState(controlpause) < 0){
				if (pausetimer < 15)
				{
					locate(endofscreen, 29);
					cout << "PAUSING";
					locate(endofscreen+7+pausetimer/5, 29);
					cout << ".";
					pausetimer++;
				}	
			}
			else 
			{
				pausetimer=0;
			}	
			
			//enemy functionality
			for (int j=(enemyid-20); j<=enemyid; j++)
			{
				if (enemy[j].y>=0 && enemy[j].y < 30 )
				{
					if (enemy[j].alive == true)
					{
						if (enemy[j].move==2 || enemy[j].isjet==true)
						{
							if (enemy[j].direction==0)
							{
								if (enemy[j].isjet==false)
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
									if (enemy[j].x < startofscreen+4-7) //-7 is for the empty space before RIVER string.
									{		
										enemy[j].alive=false;
									}
									enemy[j].x-=1;
								}
							}
							else
							{
								if (enemy[j].isjet==false)
								{
									if (enemy[j].x < endofscreen-5)
									{
										enemy[j].x+=1;
									}
									else
									{
										enemy[j].direction=0;
									}
								}
								else
								{
									if (enemy[j].x > endofscreen-5+7) //7 is for the empty space before RIVER string.
									{		
										enemy[j].alive=false;
									}
									enemy[j].x+=1;
								}
							}
							enemy[j].move=0;
						}
						else
						{
							enemy[j].move++;
						}
						if (enemy[j].isjet==false)
						{
							drawenemy(enemy[j].x, enemy[j].y);	
						}
						else
						{
							drawjet(enemy[j].x, enemy[j].y, enemy[j].direction);
						}
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
					for (int pc=0; pc<numberofplanes; pc++)
					{	
						for (int j=(barrierid-20); j<=barrierid; j++)
						{
							if (barrier[j].alive==true )
							{
								if (barrier[j].y+1 == plane[pc].y && barrier[j].x+xmult==plane[pc].x)
								{
									dead=true;
								}
								for (int planexmult=-2; planexmult<3; planexmult++)
								{
									for (int ymult=0; ymult<2; ymult++)
									{
										if (barrier[j].y-ymult == plane[pc].y && barrier[j].x+xmult==plane[pc].x+planexmult)
										{
											dead=true;
										}
									}
								}
								for (int planexmult=-1; planexmult<2; planexmult++)
								{
									if (barrier[j].y-2 == plane[pc].y && barrier[j].x+xmult==plane[pc].x+planexmult)
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
								if (enemy[j].y+1 == plane[pc].y && enemy[j].x+xmult==plane[pc].x)
								{
									dead=true;
								}
								for (int planexmult=-2; planexmult<3; planexmult++)
								{
									for (int ymult=0; ymult<2; ymult++)
									{
										if (enemy[j].y-ymult == plane[pc].y && enemy[j].x+xmult==plane[pc].x+planexmult)
										{
											dead=true;
										}
									}
								}
								for (int planexmult=-1; planexmult<2; planexmult++)
								{
									if (enemy[j].y-2 == plane[pc].y && enemy[j].x+xmult==plane[pc].x+planexmult)
									{
										dead=true;
									}
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
			
		
			if (score > 500 && mode=="bossfight")
			{
				stopspawning=true;
				score--;
			}
		
			if (score > 5 && stopspawning != true)
			{
				if (barriergencounter>=barriergenrandom)
				{
					barriergenrandom = rand()%barriergendivider+1;
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
	
				if (enemygencounter>=enemygenrandom)
				{	
					if (barriergencounter!=0)
					{	
						enemygenrandom = rand()%enemygendivider+1;
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
			//always 60 fps babyyyyyyyyyyy
			fpsregulator.join();
	//---end main game WHILE
		}
		
		for (int pc=0; pc<numberofplanes; pc++)
		{
			if (numberofplanes==1)
			{
				drawplane (plane[pc].x, plane[pc].y);
			}
			else
			{
				drawplane (plane[pc].x, plane[pc].y);
				locate(plane[pc].x, plane[pc].y);
				cout << pc+1;
			}
		}
		
		subtitle("GAME OVER", startofscreen, sizeofscreen, river);	
		changecolor("red");
		Sleep(2000); 
		
		
		if (mode == "scored")
		{
			changecolor("blue");
			return score;
		}

	


		
		if (GetKeyState(VK_ESCAPE) < 0){
			quit(score, highscore, startofscreen, sizeofscreen, river, mode);
			
			return 0;
		}
	
		if (mode=="campaign")
		{
			save(score, highscore);
		}
	
		dead=0;
		//returning to start
	}
	
		
	
	
	if (mode == "scored")
	{
		return score;
	}
	return 0;
}

int loadingscreen(bool quick=false)
{
	if (showloadingscreen==true)
	{
		
		system("CLS");
		changecolor("black");
		drawlogo(22, 10);
		locate(0, 24);
		for (int i=0; i<120; i++)
		{
			cout << "-";	
		}
		locate(0, 26);
		for (int i=0; i<120; i++)
		{
			cout << "-";	
		}
		locate(0, 25);
		if (!quick)
		{
			Sleep(1000);
			for (int i=0; i<120; i++)
			{
				if (i==4)
				{
					Sleep(700);
				}
				if (i==6)
				{
					Sleep(700);
				}
				if (i<20)
				{
					Sleep(25);
				}
				if (i==80)
				{
					Sleep(500);
				}
				if (i<80)
				{
					Sleep(10);
				}
				cout << "#";
			}
			Sleep(1000);
		}
		else
		{
			Sleep(300);
			for (int i=0; i<120; i++)
			{
			//	Sleep(1);
				cout << "#";
			}
			Sleep(50);
		}
		
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		
		if (!quick)
		{
		
			string pass="Press Any Key To Continue";
			locate(59-pass.size()/2, 23);
			cout << pass;
			_getch();
		}
		Sleep(80);
		system("CLS");	
	}
	return 0;		
}

struct menubp {
	bool selected=false;
	int x;
	int y;
	int blinktimer=0;
	string text;
	
	void draw(int i)
	{
		locate(1, 2*i+5);
		if (selected!=true)
		{
			cout << "  " << text << "  ";
		}
		else
		{
			if (blinktimer<8)
			{
				cout << "--" << text << "--";
			}
			else
			{
				cout << "  " << text << "  ";
			}
			blinktimer++;
		
			if (blinktimer == 16)
			{
				blinktimer=0;
			}
		}
	}
};

struct menuitem:menubp {
};

int capitalizer(char letter)
{
	if (letter >= 97 && letter <=122)
	{
		letter-=32;			
	}
	return letter;
}


char changecontrol (char control, int line)
{
	while (true)
	{
		control = _getch();
		if (control != '\r' && control != 27)
		{
			control = capitalizer(control);
			file_write.chartype(settingsfilelocation, line, control);
			return control;
		}
		changecolor("red");
		Sleep(100);
		changecolor("blue");
		locate(0, 1);
		cout << "that key is not supported.\n";
	}
}

int controlsscreen()
{
	int nomi=8, currentselected=0;
	menuitem menuitem[nomi];
	Sleep(20);
	system("CLS");
	locate (0, 0);

	menuitem[0].text="RIGHT";
	menuitem[1].text="LEFT";
	menuitem[2].text="UP";
	menuitem[3].text="DOWN";
	menuitem[4].text="FIRE";
	menuitem[5].text="PAUSE";
	menuitem[6].text="RELOAD";
	menuitem[7].text="RETURN";

	currentselected=0;
	menuitem[currentselected].selected=true;

	locate(0, 0);
	while (true)
	{
		for (int i=0; i<nomi; i++)
		{
			locate(0, 0);
			menuitem[i].draw(i);
			cout << " ";
			switch (i)
			{	
				case 0:
					if ( controlright == ' ')
					{
						cout << "SPACE";
					}
					else
					{
						cout << controlright ;
					}
					break;
				case 1:
					if ( controlleft == ' ')
					{
						cout << "SPACE";
					}
					else
					{
						cout << controlleft;
					}
					break;
				case 2:
					if ( controlup == ' ')
					{
						cout << "SPACE";
					}
					else
					{
						cout << controlup ;
					}
					break;
				case 3:
					if ( controldown == ' ')
					{
						cout << "SPACE";
					}
					else
					{
						cout << controldown ;
					}
					break;
				case 4:
					if ( controlfire == ' ')
					{
						cout << "SPACE";
					}
					else
					{
						cout << controlfire ;
					}
					break;
				case 5:
					if ( controlpause == ' ')
					{
						cout << "SPACE";
					}
					else
					{
						cout << controlpause ;
					}
					break;
				case 6:
					if ( controlreload == ' ')
					{
						cout << "SPACE";
					}
					else
					{
						cout << controlreload ;
					}
					break;
			}
			locate(0, 0);
		}

		Sleep(50);
		if (GetKeyState(VK_RETURN) < 0 || GetKeyState(VK_SPACE) < 0)
		{
			if (currentselected == nomi-1)
			{
				Sleep(100);
				system("CLS");
				return 0;
			}
			else
			{
				Sleep(50);
				system("CLS");
				locate (0, 0);
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				locate (0, 0);
				cout << "change " << menuitem[currentselected].text << " to: ";
				if (currentselected == 0)
				{
					controlright = changecontrol(controlright, 1);		
				}
				else if (currentselected == 1)
				{
					controlleft = changecontrol(controlleft, 2);
				}
				else if (currentselected == 2)
				{
					controlup = changecontrol(controlup, 3);
				}
				else if (currentselected == 3)
				{
					controldown = changecontrol(controldown, 4);
				}
				else if (currentselected == 4)
				{
					controlfire = changecontrol(controlfire, 5);
				}            
				else if (currentselected == 5)
				{
					controlpause = changecontrol(controlpause, 6);
				}            
				else if (currentselected == 6)
				{
					controlreload = changecontrol(controlreload, 7);
				}
				
				Sleep(70);          
				system("CLS");
			}
		}
		else if (GetKeyState(VK_ESCAPE) < 0)
		{
			Sleep(75);
			system("CLS");
			return 0;
		}
		else if (GetKeyState(VK_UP) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==0)
			{
				currentselected=nomi;
			}
			currentselected--;
			menuitem[currentselected].selected=true;	
			Sleep(150);	
		}
		else if (GetKeyState(VK_DOWN) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==nomi-1)
			{
				currentselected=-1;
			}
			currentselected++;
			menuitem[currentselected].selected=true;
			Sleep(150);
		}
	}
}

int settingsscreen()
{
	int nomi=5, currentselected;
	menuitem menuitem[nomi+1];
	Sleep(20);
	system("CLS");

	menuitem[0].text="CONTROLS";
	menuitem[1].text="SHOW LOADING SCREENS";
	menuitem[2].text="SHOW MENU LOADING SCREEN";
	menuitem[3].text="RESET HIGHSCORE";
	menuitem[nomi-1].text="RETURN";

	currentselected=0;
	menuitem[currentselected].selected=true;



	long long int menuhighscore=0;
	ifstream tempfile(scorefilelocation);	
	tempfile >> menuhighscore;
	tempfile.close();
	
	locate(0, 0);
	while (true)
	{
		for (int i=0; i<nomi; i++)
		{
			locate(0, 0);
			menuitem[i].draw(i);
			if (i==1)
			{
				cout << " ";
				if (showloadingscreen==false)
				{
					cout << "FALSE";
				}
				else
				{
					cout << "TRUE ";
				}
			}
			if (i==2)
			{
				cout << " ";
				if (showmenuloadingscreen==false)
				{
					cout << "FALSE";
				}
				else
				{
					cout << "TRUE ";
				}
			}
			if (i==3)
			{
				
				cout << "  " << (float)menuhighscore/10;
			}
			locate(0, 0);
		}

		Sleep(50);
		if (GetKeyState(VK_RETURN) < 0 || GetKeyState(VK_SPACE) < 0)
		{
			if (currentselected == 0)
			{
				Sleep(70);
				controlsscreen();
			}
			else if (currentselected == 1)
			{
				if (showloadingscreen == true)
				{
					showloadingscreen = false;
				}
				else
				{
					showloadingscreen=true;
				}
				file_write.inttype(settingsfilelocation, 9, showloadingscreen);
				Sleep(70);
			}
			else if (currentselected == 2)
			{
				if (showmenuloadingscreen == true)
				{
					showmenuloadingscreen = false;
				}
				else
				{
					showmenuloadingscreen=true;
				}
				file_write.inttype(settingsfilelocation, 10, showmenuloadingscreen);
				Sleep(70);
			}
			else if (currentselected == 3)
			{
				//reset high score
				system("CLS");
				cout << "Are you sure you want to reset your highscore? Y\\N";
				Sleep(200);
				clearinputbuffer();

				while (true)
				{	
					int tempgetch=_getch();
					if (tempgetch == 'y')
					{
						ofstream tempscorefile(scorefilelocation);
						tempscorefile<<"0" << flush;
						tempscorefile.close();
						system("CLS");
						cout << "SCORE RESET!";
						
						ifstream tempfile(scorefilelocation);	
						tempfile >> menuhighscore;
						tempfile.close();
						
						Sleep(1000);
						break;
					}
					else if (tempgetch == 'n')
					{
						break;	
					}
					else
					{
						locate(0, 1);
						cout << "Press either Y for yes or N for no. (check if your language is set to ENGLISH)";
					}
				}
				Sleep(100);
				system("CLS");
			}
			else if (currentselected == nomi-1)
			{
				Sleep(100);
				return 0;
			}
			else
			{
				changecolor("red");
				Sleep(100);
				changecolor("blue");
			}
		}
		else if (GetKeyState(VK_ESCAPE) < 0)
		{
			Sleep(75);
			system("CLS");
			return 0;
		}
		else if (GetKeyState(VK_UP) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==0)
			{
				currentselected=nomi;
			}
			currentselected--;
			menuitem[currentselected].selected=true;	
			Sleep(150);	
		}
		else if (GetKeyState(VK_DOWN) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==nomi-1)
			{
				currentselected=-1;
			}
			currentselected++;
			menuitem[currentselected].selected=true;
			Sleep(150);
		}
	}
}

int multiplayerscreen()
{
	int nomi=0, currentselected;
	unsigned short int norounds;
	string pass="Press Any Key To Continue";
	changecolor("black");
	system("CLS");
	cout << endl;
	locate (0, 0);
	//clearinputbuffer();
	locate(0, 0);
	cout << "How Many Rounds?";
	clearinputbuffer();
	cin >> norounds;
	clearinputbuffer();
	system("CLS");
	locate(0, 0);
	cout << "How Many Players?\n";
	cin >> nomi;
	clearinputbuffer();
	
	menuitem menuitem[nomi+2];
	Sleep(100);
	nomi++;
	for (int i=1; i<nomi; i++)
	{
		system("CLS");
		locate (0, 0);
		cout << "Enter player " << i << "'s Name\n";
		cin >> menuitem[i].text;
		clearinputbuffer();
	}
	menuitem[0].text="START";	
	system("CLS");
	nomi++;
	menuitem[nomi-1].text="RETURN";
	currentselected=0;
	menuitem[currentselected].selected=true;
	static long int scoresaver[100][100]={0};
	static long int scoreshower[100][100]={0};
	long long scoresummer=0;
	Sleep(200);
		
	locate(0, 0);
	changecolor("blue");
	while (true)
	{
		for (int i=0; i<nomi; i++)
		{
			locate(0, 0);
			menuitem[i].draw(i);
			locate(0, 0);
		}

		Sleep(50);
		if (GetKeyState(VK_RETURN) < 0 || GetKeyState(VK_SPACE) < 0)
		{
			if (currentselected == 0)
			{
				loadingscreen();
				for (int i=0; i<norounds; i++)
				{
					system("CLS");
					changecolor("black");
					cout << "Round " << i+1;
					Sleep(2500);
					for (int j=1; j<nomi-1; j++)
					{
						changecolor("black");
						system("CLS");
						cout << "Its " << menuitem[j].text << "'s Turn!\n\n";
						FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
						Sleep(400);
						locate(59-pass.size()/2, 23);
						cout << pass;
						_getch();
						loadingscreen(true);
						system("CLS");
						scoresaver[i][j] = game("scored", true, 0, "blue", 15, 50, 30);	
						Sleep(50);
						if (scoresaver[i][j] == 0)
						{
							system("CLS");
							changecolor("blue");
							return 0;
						}
					}
				}
				system("CLS");
				
				for (int j=1, spacer=0; j<nomi-1; j++)
				{
					scoresummer=0;
					cout << menuitem[j].text << "'s score:";
					spacer=0;
					for (int i=0; i<norounds; i++)
					{
						locate (25+spacer, j-1);
						spacer+=7;
						cout << (float)scoresaver[i][j]/10.0;
						scoresummer+=scoresaver[i][j];
					}
					locate (25+spacer+3, j-1);
					cout << (float)scoresummer/10.0 << endl;
				}
				while (true)
				{
					Sleep(150);
					if (GetKeyState(VK_ESCAPE) < 0 || GetKeyState(VK_SPACE) < 0 || GetKeyState(VK_RETURN) < 0)
					{
						Sleep(200);
						system("CLS");
						break;
					}
				}
			}
			else if (currentselected == nomi-1)
			{
				Sleep(100);
				return 0;
			}
			else
			{
				changecolor("red");
				Sleep(100);
				changecolor("blue");
			}
		}
		else if (GetKeyState(VK_ESCAPE) < 0)
		{
			Sleep(100);
			system("CLS");
			return 0;
		}
		else if (GetKeyState(VK_UP) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==0)
			{
				currentselected=nomi;
			}
			currentselected--;
			menuitem[currentselected].selected=true;	
			Sleep(150);	
		}
		else if (GetKeyState(VK_DOWN) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==nomi-1)
			{
				currentselected=-1;
			}
			currentselected++;
			menuitem[currentselected].selected=true;
			Sleep(150);
		}
	}
}

int twoplayerscreen()
{
	int nomi=3, currentselected;
	menuitem menuitem[nomi];

	system("CLS");

	menuitem[0].text="CLASSIC";
	menuitem[1].text="CONFLICT";
	menuitem[2].text="RETURN";

	currentselected=0;
	menuitem[currentselected].selected=true;

	locate(0, 0);
	while (true)
	{
		for (int i=0; i<nomi; i++)
		{
			locate(0, 0);
			menuitem[i].draw(i);
			locate(0, 0);
		}

		Sleep(50);
		if (GetKeyState(VK_RETURN) < 0 || GetKeyState(VK_SPACE) < 0)
		{
			if (currentselected == 0)
			{
				Sleep(50);
				loadingscreen();
				game("twoplayerclassic", true, 0, "blue", 15, 80, 30);
				system("CLS");
				changecolor("blue");
				return 0;
			}
			if (currentselected == 1)
			{
				Sleep(50);
				loadingscreen();
				game("twoplayerconflict", true, 0, "blue", 15, 50, 30);
				system("CLS");
				changecolor("blue");
				return 0;
			}
			else if (currentselected == nomi-1)
			{
				Sleep(100);
				return 0;
			}
			else
			{
				changecolor("red");
				Sleep(100);
				changecolor("blue");
			}
		}
		else if (GetKeyState(VK_ESCAPE) < 0)
		{
			Sleep(75);
			system("CLS");
			return 0;
		}
		else if (GetKeyState(VK_UP) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==0)
			{
				currentselected=nomi;
			}
			currentselected--;
			menuitem[currentselected].selected=true;	
			Sleep(150);	
		}
		else if (GetKeyState(VK_DOWN) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==nomi-1)
			{
				currentselected=-1;
			}
			currentselected++;
			menuitem[currentselected].selected=true;
			Sleep(150);
		}
	}
}

void drawcredit(string text, int y, bool outline=false)
{
	if (outline==true)
	{
		locate (59-text.size()/2, y-1);
		for (int i=0; i<text.size(); i++)
		{
			cout << "-";
		}	
		locate (59-text.size()/2, y+1);
		for (int i=0; i<text.size(); i++)
		{
			cout << "-";
		}
	}
	
	locate (59-text.size()/2, y);
	cout << text;
}

int creditsscreen()
{
	ShowConsoleCursor(false);

	
	
	system("CLS");
	changecolor("black");
	Sleep(3000);
	changecolor("white");
	drawcredit ("Atari River Raid, Console Remake", 14, true);
	Sleep(1000);
	drawcredit ("@2023", 15, false);
	Sleep(3000);


	
	system("CLS");
	changecolor("black");
	Sleep(2000);
	changecolor("dark blue");
	locate(0, 0);
	drawcredit ("Developed By:", 12, true);
	Sleep(1000);
	drawcredit ("[IPH_undead]", 16, true);
	Sleep(3000);
	
	system("CLS");
	changecolor("black");
	Sleep(2000);
	changecolor("aqua");
	locate(0, 0);
	drawcredit ("Game Design:", 11, true);
	Sleep(1000);
	drawcredit ("[IPH_undead]", 15);
	Sleep(1000);
	drawcredit ("Kourosh Khomamizadeh", 17);
	Sleep(3000);
	
	system("CLS");
	changecolor("black");
	Sleep(2000);
	changecolor("yellow");
	locate(0, 0);
	int header=7;
	drawcredit ("Special Thanks To:", header, true);
	Sleep(1000);
	drawcredit ("My Family", header+4);
	Sleep(1000);
	drawcredit ("[Jibel]", header+6);
	Sleep(1000);
	drawcredit ("Kourosh Khomamizadeh", header+8);
	Sleep(1000);
	drawcredit ("Mohammad Amin Abdinia", header+10);
	Sleep(1000);
	drawcredit ("Mahdiyar Ferdowsi", header+12);
	Sleep(1000);
	drawcredit("Parsa Mousavi", header+14);
	Sleep(3000);
	

	system("cls");
	changecolor("black");	
	Sleep(2000);
	changecolor("blue");
	return 0;
}

bool skip=false;
void hwrite(string text)
{	
	for (int i=0;i<text.size();i++)
	{
		if (GetKeyState(VK_SPACE) < 0 || GetKeyState(VK_RETURN) < 0)
			skip=true;
			
		cout << text[i];
		
		if (skip!=true)
			Sleep(1);
	}
	
	cout << endl;
	
	if (skip!=true)
		Sleep(300);
}

int helpscreen()
{
	Sleep(100);
	system("CLS");
	locate(0, 0);
	hwrite("(PRESS ENTER OR SPACE TO SKIP)");
	hwrite("");
	hwrite("");
	hwrite("Welcome to ATARI!");
	hwrite("GAME INFO");
	hwrite("you can control your plane with ASWD by default, and shoot by using SPACE and reload with R.");
	hwrite("these controls can be changed in the SETTINGS menu later.");
	hwrite("There are 3 types of enemies in the game:");
	hwrite("Barriers");
	hwrite("Enemies");
	hwrite("Jets");
	hwrite("Barriers do not move, and they absorb bullets");
	hwrite("Enemies Can move around, however they get killed if shot");
	hwrite("Jets are extremely quick, but they go in only one direction");
	hwrite("");
	hwrite("");
	hwrite("GAME MODES:");
	hwrite("");
	hwrite("");

	
	hwrite("(THIS PAGE IS NOT FINISHED. I WILL FINISH IT WHEN THE GAME IS DONE.)");
	
	
	
	hwrite("(PRESS ESC TO EXIT)");
	
	if (skip==true)
	{	
		locate(70, 0);
		cout << " ";
	}
	while(true)
	{
		Sleep(70);
		if (GetKeyState(VK_ESCAPE) < 0)
		{
			skip=false;
			return 0;
		}
	}
	
}

int main(int argc, char *argv[])
{	
	int nomi=9, currentselected; //nomi: number of menu items
	menuitem menuitem[nomi+1];


	resizeatari();
	
	ShowConsoleCursor(false);
	
	menuitem[0].text="ENDLESS";
	menuitem[1].text="BOSS FIGHT";
	menuitem[2].text="DOUBLE PLAYER";
	menuitem[3].text="MULTI PLAYER";
	menuitem[4].text="SETTINGS";
	menuitem[5].text="CREDITS";
	menuitem[6].text="HELP";
	menuitem[7].text="SKINS";
	menuitem[8].text="EXIT TO DESKTOP";

	
	if (showmenuloadingscreen==true)
	{
		changecolor("black");
		Sleep(200);
		drawlogo(30, 10);
		Sleep(400);
		for (int i=0; i<nomi; i++)
		{
			Sleep(200);
			locate(0, 0);
		menuitem[i].draw(i);
			locate(0, 0);
		}
		Sleep(200);
	}

	
	changecolor("blue");

	currentselected=0;
	menuitem[currentselected].selected=true;
	

//	locate(119, 29);
//	cout << "X";
//	locate(59, 14);
//	cout << "C";
//	locate(59-2, 10);
//	cout << menuitem[1].text;

	long long int inmenutime=0;
	bool firsttime=true;
	bool firsttimeremoved=false;
	locate(0, 0);
	while (true)
	{
		
		drawlogo(30, 10);
		if (firsttime==true)
		{
			inmenutime++;
			if (inmenutime>150)
			{
				locate(0, 0);
				cout << "Use the UP and DOWN Arrow keys to navigate the menu, and ENTER/SPACE to select an item.";
			}
		}
		else
		{
			if (firsttimeremoved==false)
			{
				locate(0, 0);
				cout << "                                                                                             ";
				firsttimeremoved=true;
			}
		}
		
		for (int i=0; i<nomi; i++)
		{
		
			locate(0, 0);
			menuitem[i].draw(i);
			locate(0, 0);
		}
		

		Sleep(50);
		if (GetKeyState(VK_RETURN) < 0 || GetKeyState(VK_SPACE) < 0)
		{
			firsttime=false;
			if (currentselected == 0)
			{
				loadingscreen();
				system("CLS");
				game("campaign", true , 0, "blue", 15, 50, 30);
				system("CLS");
				changecolor("blue");
			}
			else if (currentselected == 2)
			{
				Sleep(50);
				twoplayerscreen();
				system("CLS");
			}
			else if (currentselected == 3)
			{
				Sleep(50);
				multiplayerscreen();
				Sleep(50);
				system("CLS");
			}
			else if (currentselected == 4)
			{
				Sleep(50);
				settingsscreen();
				system("CLS");
			}
			else if (currentselected == 5)
			{
				Sleep(50);
				creditsscreen();
				system("CLS");
			}
			else if (currentselected == 6)
			{
				Sleep(50);
				helpscreen();
				system("CLS");
			}
//			else if (currentselected == 7)
//			{
//				sleep(70);
//				//skinscreen();
//				system("CLS");
//			}
			else if (currentselected == nomi-1)
			{
				return 0;
			}
			else
			{
				changecolor("red");
				Sleep(100);
				changecolor("blue");
			}
		}
		else if (GetKeyState(VK_UP) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==0)
			{
				currentselected=nomi;
			}
			currentselected--;
			menuitem[currentselected].selected=true;	
			Sleep(150);
			firsttime=false;
		}
		else if (GetKeyState(VK_DOWN) < 0)
		{
			menuitem[currentselected].selected=false;
			menuitem[currentselected].blinktimer=0;
			if (currentselected==nomi-1)
			{
				currentselected=-1;
			}
			currentselected++;
			menuitem[currentselected].selected=true;
			Sleep(150);
			firsttime=false;
		}
	}
}
