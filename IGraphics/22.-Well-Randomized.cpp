# include "iGraphics.h"
# include <time.h>

int flyer, randomNumber;
int birdX = 500 , birdY = 325, birdDY = 10, birdW=100, birdH=100 ;
int screenW = 1200 , screenH = 650;
int obsH[4] = {220,260,300,350};
int obsX1[4]= {1300,1600,1300+700,1300+900}; //obs for obstacles
int obsX2[4] = {1800, 1800 + 1200, 1800 + 750, 1800 + 1000};
int obsY1 = 0 ,obsY2[4],  obsDX = 10;
char string[30] = "All Photos/obs1.bmp" ;
int control , check1[4], check2[4];
int boxX[5] = {470,470,470,470,470}, boxY[5]={373 ,300 ,223 ,150 ,70 } ;
char text[5][20]={ "PLAY" , "OPTIONS" , "HOW TO PLAY" , "HIGH SCORES" , "QUIT" };
int textX[5]={ 100, 70, 50, 50, 100},boxDy=20, blue[5];
int maxSpeed = 18 ;
int tickX = 220, tickY = 340, easyX = 280 , mediumX = 522 , hardX = 764, allY = 337, backX=1080, backY = 40, blueBack ;
int arrowX = 25, arrowY=35, arrowClicked;
int gravityDY = 3 ;
int bulletX=0 , bulletY=200 , bulletDX=16 ;
int lifeBonusX = 10800 , lifeBonusY = 250 , lifeBonusDX = 12, life = 3;
int appleX = 2000, appleY = 250 , appleDX = 15 ;
int brickX=3*screenW , brickY=250, brickDX = 14, brickW=50, brickH=50;
int prevGameX = 387 , prevGameY = 360 , newGameX = 387 , newGameY = 246, whichGameBoxW = 430, whichGameBoxH = 70 ;
int nameX=470 , nameY=220;
int creditX=1050 , creditY=50;
int numberOfSlots=5, ifHigh , slotX1=110,slotX2=450,slotX3=815, slotY=400, slotGap=60;
int paused , start ;
int sec=0;
int endObs, birdMove, bringSky, won;
int liveX=20,liveY=300;
int numberOfTimers=9;
int difficultyMode=1,gameFinished;
int winningScore;
int moveX[12], screenParts=12, partW=200;
char fullScore[20];
char  lifeCount[100];
char lives[10]="LIVES : ";
char second[10]="SCORE : ";
char count[10000000]="0";

char screenNumber[12][30]={"Moving Screen/0.bmp", "Moving Screen/1.bmp", "Moving Screen/2.bmp", "Moving Screen/3.bmp",
	                       "Moving Screen/4.bmp", "Moving Screen/5.bmp", "Moving Screen/6.bmp", "Moving Screen/7.bmp",
						   "Moving Screen/8.bmp", "Moving Screen/9.bmp", "Moving Screen/10.bmp","Moving Screen/11.bmp"};

char scoreboards[3][40]={"High Scores1.txt","High Scores2.txt","High Scores3.txt"};
float playTime, maxPlayTime=300;


typedef struct a
{
	char name[20];
	int  personalScore;
	char scorePrint[20];
}playerData;

playerData highscores1[5],highscores2[5],highscores3[5];
playerData nowPlaying;

char input[20];
int inputLen;
int BirdYMax=screenH-100;


struct saveData
{
  int birdY ;
  int obs1X[4];
  int obs2X[4] ;
  int obsDX ;

  int appleX ;
  int appleY ;

  int lifeBonusX ;
  int lifeBonusY ;

  int bulletX ;
  int bulletY ;
  int life ;
  int score;
  playerData player;
  int tick;
  int maxSpeed;
  int difficulty;
  int brickX;
  int brickY;
};

struct saveData a ;



//name of all functions
void randomNumberGenerator(void);//helps the bamboos come randomly
void distanceChecker (void);//prevents the collision of two neighbouring obstacle while appearing randomly
void bambooSpeed(void);//speeds up the bamboos gradually
void birdTimer(void);//makes the bird flap wings
void obstacleMove (void);//controls the obstacles
void bulletMove (void);
void ifCollissions(void);//detects if the bird is in contact with any obstacle
void gravity (void); //adds gravity on the motion of the bird 
void newBullet (void) ; //when a bullet hits the bird or passes the screen, brings a new bullet
void newLifeBonus (void); // when the bird collects a heart or the heart passes the screen, brings a new life bonus
void newApple (void); // when the bird collects an apple or the apple passes the screen, brings a new apple
void reviveBird (void); // when you loose a life , it will revive you by setting you in a temporarily safe position
void resetAll (void); // when the game is over, it will reset all the values of the variables.
void scoring(void) ;// determines the tme the bird sustained
void saveIfHighScore(void);
void pauseAll(void);
void resumeAll (void);
void startGame (void);
void saveGame (void);
void loadGame (void);
void increaseLife(void);//increases life after getting hearts
void decreaseLife(void);//decreases life after losing
void refreshFile (void); //cleans all the datas saved before
void takeName (void);
int ifPrevSaved (void);//check if there exixts any previouly saved datas or not
void endForest (void);

void finalScore(void);
void iDraw()
{
    	
	iClear();

	if (control==0)//the main menu
	{
		int i;
		pauseAll();
		iShowBMP (0, 0, "All Photos/BackgroundMain.bmp");
		
		
		for(i=0;i<5;i++){
			iSetColor(200,255,blue[i]);
               iText (boxX[i]+textX[i] , boxY[i]+boxDy, text[i], GLUT_BITMAP_TIMES_ROMAN_24);
		}

		iText(creditX, creditY, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
	}


	if (control==1)//the game
	{
		iSetColor (255,255,255);
		//iShowBMP (0,0, "Background2.bmp");
		int j;
        for (j=0;j<screenParts;j++)
        {
	       iShowBMP (moveX[j],0,screenNumber[j]);
        }

		iShowBMP2 (bulletX , bulletY , "All Photos/bullet.bmp", 0); // draws the bullets
		iShowBMP2 (lifeBonusX , lifeBonusY, "All Photos/lifeBonus.bmp", 0); //draws the hearts
		iShowBMP2 (appleX , appleY, "All Photos/apple.bmp", 0); // draws the apples
		if (difficultyMode==2 || difficultyMode==3)
			iShowBMP (brickX, brickY, "All Photos/brick.bmp");//draws the bricks

		int i;

		for(i=0;i<4;i++)//draws the lower obstacles
		{
			if (check1[i]!=1)
			{
				iShowBMP2 (obsX1[i] , obsY1 , string,0);
			}
			else
			{
				obsX1[i]+=3000;
				iShowBMP2 (obsX1[i] , obsY1 , string,0);
				check1[i]=0;
			}
			string[14] ++ ; //changes the string name, therefore changes the lower obstacles
			//printf("%s\n",string);
			if (string[14]=='5')
			string[14] = '1' ;
		}

	
		for(i=0;i<4;i++)  //draws the upper ostacles
		{
			if (endObs!=1)
				obsY2[i] = screenH - obsH[i] + 35;

		//	if (check2[i]!=1)
     			iShowBMP2 (obsX2[i] , obsY2[i] , string,0);
		/*	else
			{
				obsX2[i]+=2000;
				iShowBMP2 (obsX2[i] , obsY2[i] , string,0);
				check2[i]=0;

			}*/
			string[14] ++ ; //changes the string name, therefore changes the lower obstacles

			if (string[14]=='5')
				string[14] = '1' ;
		}


		//printf("%s",lives);

		iShowBMP(0,0,"Moving Screen/frame.bmp");

		iText(liveX,liveY,lives,GLUT_BITMAP_TIMES_ROMAN_24);//lives count;

		itoa(life,lifeCount,10);
		iText(liveX+100,liveY,lifeCount,GLUT_BITMAP_TIMES_ROMAN_24);//lives count;

		iText(liveX,liveY-40,second,GLUT_BITMAP_TIMES_ROMAN_24);//scoring
		iText(liveX+100,liveY-40,count,GLUT_BITMAP_TIMES_ROMAN_24);//scoring

		iText (20 , 55,"PRESS 'P'",GLUT_BITMAP_TIMES_ROMAN_24);
		iText (20 , 25,"TO PAUSE",GLUT_BITMAP_TIMES_ROMAN_24);
		iText (20, 450, "Now Playing : ",GLUT_BITMAP_TIMES_ROMAN_24);
		iText(20,420,nowPlaying.name,GLUT_BITMAP_TIMES_ROMAN_24);

		if (bringSky==1)
			iShowBMP (0,0,"All Photos/endOfForest.bmp");

		if(flyer==1) // draws the bird
			iShowBMP2 (birdX, birdY, "All Photos/bird1.bmp", 0);
		else if((flyer == 2)||(flyer==4))
		    iShowBMP2 (birdX, birdY, "All Photos/bird2.bmp", 0);
		else 
			iShowBMP2 (birdX, birdY, "All Photos/bird3.bmp", 0);

		if (paused==1)
		{
			iShowBMP2 (0,0,"All Photos/SaveOrResume.bmp", 0);
		}
		
	}
	
	if (control == 10 )//New game or saved game ?
	{
		iShowBMP (0,0, "All Photos/newOrNot.bmp");
		
		iSetColor (0, 250, 250);
		iRectangle (prevGameX, prevGameY, whichGameBoxW, whichGameBoxH);

		iSetColor (0,250, 250);
		iRectangle (newGameX, newGameY, whichGameBoxW, whichGameBoxH);
	}

	if (control == 100)//name input
	{
		iSetColor (84,255,160);
		iShowBMP (0,0, "All Photos/nameBox1.bmp");
		if (arrowClicked==1)
			iShowBMP (0,0, "All Photos/nameBox2.bmp");
		iText (nameX , nameY, input,GLUT_BITMAP_TIMES_ROMAN_24);
	}



	if (control==2) //Options Page
	{
		iShowBMP (0,0,"All Photos/Options.bmp");
		iShowBMP2 (tickX , tickY , "All Photos/tick.bmp", 0);
		iSetColor(200,255,blueBack);
		iText (backX,backY,"BACK", GLUT_BITMAP_TIMES_ROMAN_24);
	}

	if (control==3) //Instructions Page
	{
		iShowBMP(0,0,"All Photos/Instructions.bmp");
		if (arrowClicked==1)
			iShowBMP (0,0,"All Photos/Instructions with white arrow.bmp");
	}

	if (control==4) //High Scores
	{
		iSetColor(153,217,234);
		int i;
		iShowBMP (0,0,"All Photos/highScores1.bmp");
		if (arrowClicked==1) iShowBMP (0,0,"All Photos/highScores2.bmp");
		for (i=0 ; i<numberOfSlots ; i++)


		{
			printf("%s %s\n",nowPlaying.scorePrint,highscores3[i].scorePrint);
			if((highscores1[i].personalScore==nowPlaying.personalScore) &&(difficultyMode==1))
				iSetColor (137,104,205);
			else  iSetColor(153,217,234);
			
			if(highscores1[i].personalScore!=0){
				iText (slotX1 , slotY - slotGap*i , highscores1[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
				iText (slotX1+170 , slotY - slotGap*i , highscores1[i].scorePrint, GLUT_BITMAP_TIMES_ROMAN_24);
		    }

			if((highscores2[i].personalScore==nowPlaying.personalScore) &&(difficultyMode==2)) 
				iSetColor (137,104,205);
			else  iSetColor(153,217,234);

			if(highscores2[i].personalScore!=0){
				iText (slotX2 , slotY - slotGap*i , highscores2[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
				iText (slotX2+170 , slotY - slotGap*i , highscores2[i].scorePrint, GLUT_BITMAP_TIMES_ROMAN_24);
			}
			if((highscores3[i].personalScore==nowPlaying.personalScore) &&(difficultyMode==3))
				iSetColor (137,104,205);
			else  iSetColor(153,217,234);

			if(highscores3[i].personalScore!=0){
				iText (slotX3 , slotY - slotGap*i , highscores3[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
				iText (slotX3+170 , slotY - slotGap*i , highscores3[i].scorePrint, GLUT_BITMAP_TIMES_ROMAN_24);
			}
		}
		
	}

	if (control==6)
	{
		if (arrowClicked==1)
			iShowBMP (0,0,"All Photos/credits2.bmp");
		else
			iShowBMP (0,0,"All Photos/credits1.bmp");
	}

	if (control==5) //Game Over
	{
		if (won!=1){
			iShowBMP (0,0, "All Photos/gameover.bmp");
			iSetColor (205,150,205);
			iText(440,260,"YOUR FINAL SCORE IS ", GLUT_BITMAP_TIMES_ROMAN_24);

			iText(740,260,count, GLUT_BITMAP_TIMES_ROMAN_24);
			if (ifHigh==1){
				iSetColor(255,255,255);
				iText(450, 520 , "A NEW HIGH SCORE !!  :D", GLUT_BITMAP_TIMES_ROMAN_24);
				
			}
		}
		else{
          
			iShowBMP (0,0, "All Photos/youWon.bmp");

			//iSetColor(255,1,50);
			iSetColor (205,150,205);
			iText(400,360,"YOUR SCORE IS  = ", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(650,360,count, GLUT_BITMAP_TIMES_ROMAN_24);
			iText(400,320,"LIFE BONUS = ",GLUT_BITMAP_TIMES_ROMAN_24);
			iText(650,320,lifeCount,GLUT_BITMAP_TIMES_ROMAN_24);
			iText(670,320," X 1000",GLUT_BITMAP_TIMES_ROMAN_24);
			iText(400,280,"YOUR FINAL SCORE IS  = ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(700,280,fullScore,GLUT_BITMAP_TIMES_ROMAN_24);
			if (ifHigh==1){
				iSetColor(255,255,255);
				iText(450, 520 , "A NEW HIGH SCORE !!  :D", GLUT_BITMAP_TIMES_ROMAN_24);
				
			}
		}
		//	iText(450, 620 , "Press 'h' to see SCORE BOARD", GLUT_BITMAP_TIMES_ROMAN_24);
		
	}
}

void takeName ()
{
	control = 100 ;
}

void increaseLife()//increases life after getting hearts
{
	life++;
	itoa(life,lifeCount,10);
	PlaySound("sounds/lifeBonus.wav",NULL,SND_ASYNC);

}

void decreaseLife()//decreases life after losing lives
{
	life--;
	itoa(life,lifeCount,10);
	PlaySound("sounds/lifeLost.wav",NULL,SND_ASYNC);

}

void randomNumberGenerator(void)//helps the bamboos come randomly
        {
            int low_num=300,hi_num=1000;
            
            srand(time(NULL));
            randomNumber = (rand()%(hi_num-low_num))+low_num;
        }

void bambooSpeed()//speeds up the bamboos gradually
{
	if(obsDX<=maxSpeed)
        obsDX+=1;
	printf("max %d tick  %d diff %d\n",maxSpeed,tickX,difficultyMode);
}

void birdTimer()//makes the bird flap wings
{
 
		flyer++; //flyer actually changes the picture of the bird. That's how the bird flies.
		if(flyer ==5) flyer=1;

		if (birdMove==1)
			birdX+=20;
		if (birdX>=screenW)
			endForest();
		if (bringSky==1 && birdX>=screenW/2)
		{
			iDelay(2);
			won=1;
			nowPlaying.personalScore=sec;
			PlaySound("sounds/end.wav",NULL,SND_ASYNC);
			pauseAll();
			saveIfHighScore();
			refreshFile();
			control=5;
		}
}

void endForest ()
{
	birdX=-15;
	bringSky = 1;
}

void bulletMove ()
{
	bulletX += bulletDX ;
	if (bulletX > screenW ) newBullet() ;
	if (bulletX>152 && bulletX<170 && bulletY<screenH) PlaySound("sounds/shotgun.wav",NULL,SND_ASYNC);
}

void endScreen ()
{
	pauseAll();
	birdMove=1;
	iResumeTimer (2);
}


void distanceChecker ()//prevents the collision of two neighbouring obstacle
{
	int i , j ;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j<4 ; j++)
		{
			if (abs(obsX1[i] - obsX2[j]) < 200)
			{
				check1[i]=1;
			//	check2[j]=1;
			}
			/*
			if (i==j)
				continue;

			if (abs(obsX1[i] - obsX1[j]) < 20)
				check[i]=1;

			if (abs(obsX2[i] - obsX2[j]) < 20)
				check[i]=1;
				*/
		}
	}

}


void obstacleMove ()//controls the obstacles (and all the moving things)
{
	int i ;
	for (i=0; i<screenParts ; i++)
    {
	     moveX[i]-=obsDX;
	    if (moveX[i]<=0 && i==0)
		    moveX[i]=moveX[11]+partW-25;
	    else if (moveX[i]<=0 && i!=0)
		    moveX[i]=moveX[i-1]+partW;
	}

	if (endObs==1)
	{
		for (i=0 ; i<4 ; i++)
		{
			if (obsX1[i]>screenW) obsX1[i]=20000;
			if (obsX2[i]>screenW) obsY2[i]=screenH+500;
		}
		if (bulletX<90)
			bulletY=screenH+500;
		if (appleX>screenW)
			appleY=screenH+500;
		if (brickX>screenW)
			brickY=screenH+500;
		if (lifeBonusX>screenW)
			lifeBonusY=screenH+500;
	}

	for (i=0 ; i<4 ; i++)
	{
		obsX1[i] = obsX1[i] - obsDX ;//velocity of obstacles
		if (obsX1[i] < -400 )
			obsX1[i] = screenW + randomNumber; //// + bulletX  ;
		
	}

	for (i=0 ; i<4 ; i++)
	{
		obsX2[i] = obsX2[i] - obsDX ;
		if (obsX2[i] < -300 )
			obsX2[i] = screenW +  500 + bulletX ; //+ bulletX;
	}


	brickX -= brickDX;
	if (brickX<-50)
	{
		brickX=3*screenW;
		brickY=bulletY;
	}

	lifeBonusX -= lifeBonusDX ;
	if (lifeBonusX < -50 ) {
		newLifeBonus() ;
	}

	appleX -= appleDX ;
	if (appleX < - 50 ) newApple ();
}

void scoring(){
	playTime += 0.1 ; //counts the playing time
	sec++;
	itoa(sec,count,10);
	printf("%s\n",a.player.scorePrint);

	if (playTime >= maxPlayTime)
	{
		endObs=1;
		finalScore();
	}
	if (playTime > maxPlayTime + (0.1*(screenW-210))/obsDX)
	{
		endScreen();
	}
}
void finalScore()
{
	winningScore=(sec)+(life * 1000);
	itoa(winningScore,fullScore,10);
	nowPlaying.personalScore=winningScore;
	itoa(winningScore,nowPlaying.scorePrint,10);
	
}

void  ifCollissions()//detects if the bird is in contact with anything
{
    int i;
	for (i=0 ; i<4 ; i++)
	{
		if ( obsX1[i]>=birdX+10 && obsX1[i]<=birdX+birdW-20 && birdY<=obsH[i]-47 )
		{
			decreaseLife();
			iDelay(2), reviveBird() ;
			break ;
		}
		else if ( obsX2[i]>=birdX+10 && obsX2[i]<=birdX+birdW-20 && birdY+birdH>=obsY2[i]+37 )
		{
			decreaseLife();
			iDelay(2), reviveBird() ;
			break;
		}

	}
	
	if (difficultyMode==2 || difficultyMode==3)
	{
		if (birdX+birdW-20>=brickX && birdX+10<brickX+brickW && birdY+birdH-35>=brickY && birdY+35<=brickY+brickH )
		{
			decreaseLife();
			iDelay(2), reviveBird() ;
		}
	}

	if (bulletX >= birdX && bulletX < birdX + birdW/3 && bulletY>birdY+40 && bulletY<birdY+birdH-40){
	//if in connect with a bullet
		decreaseLife();
		iDelay(2), reviveBird() ;
		
		
	}
	if(birdY<-15)//decreases a life after falling down
	{
		 decreaseLife();
		 iDelay(2), reviveBird() ;
	}

	if (birdX+birdW >= lifeBonusX && birdX < lifeBonusX+60 && birdY+birdH>=lifeBonusY && birdY<=lifeBonusY+50 )
	//if in connect with a life bonus
	{
		newLifeBonus ();
		increaseLife();
	}

	if (birdX+birdW >= appleX && birdX < appleX+60 && birdY+birdH>=appleY && birdY<=appleY+50 )
	//if in connect with an apple
	{
		newApple ();
		sec+=100;
		PlaySound("sounds/apple.wav",NULL,SND_ASYNC);
	}


	if (life == -1) 
	{
		iDelay(3);
		control = 5 ;
	}

	if (control==5)//game over
	{
		nowPlaying.personalScore=sec;
		PlaySound("sounds/end.wav",NULL,SND_ASYNC);
		pauseAll();
		saveIfHighScore();
		refreshFile();
	}
}

void newApple ()
{
	appleX = screenW + randomNumber ;
	appleY = birdY ;
}


void gravity ()
{
	birdY -= gravityDY ;
	gravityDY += 3 ;
}

void newBullet ()
{
	bulletX = -300 ;
	bulletY = birdY+20 ;
}


void newLifeBonus ()
{
	lifeBonusX = 8400 ;
	lifeBonusY = birdY ;
}

void pauseAll ()
{
	int i ;
	for (i=0 ; i<numberOfTimers ; i++)
		iPauseTimer (i);
}

void resumeAll (void)
{
	int i;
	for (i=0 ; i<numberOfTimers ; i++)
		iResumeTimer(i);
}

void saveGame ()
{
	FILE *fp ;
	fp = fopen ("Saved Game.txt", "w");

	 a.birdY = birdY,
     a.obs1X[0]=obsX1[0], a.obs1X[1]=obsX1[1], a.obs1X[2]=obsX1[2], a.obs1X[3]=obsX1[3],
   	 a.obs2X[0]=obsX2[0], a.obs2X[1]=obsX2[1], a.obs2X[2]=obsX2[2], a.obs2X[3]=obsX2[3],
	 a.obsDX = obsDX,
		
 	 a.appleX = appleX,
     a.appleY = appleY,

     a.lifeBonusX = lifeBonusX,
     a.lifeBonusY = lifeBonusY,

     a.bulletX= bulletX,
     a.bulletY= bulletY,
     a.life= life,
     a.score = sec,
	 a.player = nowPlaying;
	 a.tick=tickX;
	 a.maxSpeed=maxSpeed;
	 a.difficulty=difficultyMode;
	 a.brickX=brickX;
	 a.brickY=brickY;

	 fprintf (fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %s %d %d %d %d %d %d",
		        a.birdY,
      			a.obs1X[0], a.obs1X[1], a.obs1X[2], a.obs1X[3],
   			    a.obs2X[0], a.obs2X[1], a.obs2X[2], a.obs2X[3],
		        a.obsDX,
		
 		    	a.appleX,
        		a.appleY,

     		    a.lifeBonusX,
        		a.lifeBonusY,

        		a.bulletX,
        		a.bulletY,
        		a.life,
        		a.score ,
				a.player.name,
				a.player.personalScore,
				//a.player.scorePrint,
				a.tick,
				a.maxSpeed,
				a.difficulty,
				a.brickX,
	            a.brickY);
   
	 fclose(fp);
}


void loadGame ()
{
	FILE *fp ;
	fp = fopen ("Saved Game.txt", "r");

	fscanf (fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %s %d %d %d %d %d %d",
        &a.birdY,
        &a.obs1X[0], &a.obs1X[1], &a.obs1X[2], &a.obs1X[3],
        &a.obs2X[0], &a.obs2X[1], &a.obs2X[2], &a.obs2X[3],
        &a.obsDX,

        &a.appleX,
        &a.appleY,

        &a.lifeBonusX,
        &a.lifeBonusY,

        &a.bulletX,
        &a.bulletY,
        &a.life,
        &a.score,
		a.player.name,
		&a.player.personalScore,
		//a.player.scorePrint,
		&a.tick,
		&a.maxSpeed,
		&a.difficulty,
		&a.brickX,
	    &a.brickY) ;

 	fclose (fp);
	 
	 birdY = a.birdY;
     obsX1[0]=a.obs1X[0], obsX1[1]=a.obs1X[1], obsX1[2]=a.obs1X[2], obsX1[3]=a.obs1X[3],
     obsX2[0]=a.obs2X[0], obsX2[1]=a.obs2X[1], obsX2[1]=a.obs2X[2], obsX2[3]=a.obs2X[3],
	 obsDX = a.obsDX,
		
 	 appleX = a.appleX,
     appleY = a.appleY,

     lifeBonusX = a.lifeBonusX,
     lifeBonusY = a.lifeBonusY,

     bulletX = a.bulletX,
     bulletY = a.bulletY,
     life = a.life,
     sec = a.score;
	 nowPlaying=a.player;
	 tickX=a.tick;
	 maxSpeed=a.maxSpeed;
	 difficultyMode=a.difficulty;
	 brickX=a.brickX;
	 brickY=a.brickY;
}

void saveIfHighScore ()
{
	int i,j;
	playerData temp , max;//playerData cosists of a name and a score
	finalScore();
	FILE *fp=NULL;
	if(difficultyMode==1){

	    if (nowPlaying.personalScore > highscores1[numberOfSlots-1].personalScore)
	    {
		    ifHigh = 1;
		
		//fp = fopen (scoreboards[difficultyMode-1] , "w");
			printf("%s\n",nowPlaying.scorePrint);
		
			fp = fopen ("High Scores1.txt", "w");
			highscores1[numberOfSlots-1]= nowPlaying;

		    for (i=0 ; i<numberOfSlots-1 ; i++)
		    {
			    max = highscores1[i] ;
			    for (j=i ; j<numberOfSlots ; j++)
			    {
				    if (highscores1[j].personalScore > max.personalScore)
				    {
					    temp = highscores1[j] ;
					    highscores1[j] = max ;
					    highscores1[i] = temp ;
				   }
			   }
		    }
		    for (i=0 ; i<numberOfSlots ; i++)
		    {
			    itoa(highscores1[i].personalScore,highscores1[i].scorePrint,10);
			    fprintf (fp,"%s %d %s\n" , highscores1[i].name, highscores1[i].personalScore,highscores1[i].scorePrint);
		    }
		    fclose(fp);
		}

	}
	finalScore();
	if(difficultyMode==2){
	    if (nowPlaying.personalScore > highscores2[numberOfSlots-1].personalScore){
			ifHigh=1;
		    fp = fopen ("High Scores2.txt", "w");
		
			highscores2[numberOfSlots-1]= nowPlaying;

		    for (i=0 ; i<numberOfSlots-1 ; i++)
		    {
			    max = highscores2[i] ;
			    for (j=i ; j<numberOfSlots ; j++)
			    {
				    if (highscores2[j].personalScore > max.personalScore)
				    {
					    temp = highscores2[j] ;
					    highscores2[j] = max ;
					    highscores2[i] = temp ;
				    }
			    }
		    }

		    for (i=0 ; i<numberOfSlots ; i++)
		    {
			    itoa(highscores2[i].personalScore,highscores2[i].scorePrint,10);
			    fprintf (fp,"%s %d %s\n" , highscores2[i].name, highscores2[i].personalScore,highscores2[i].scorePrint);
		    }
		    fclose(fp);
		}

	}
	finalScore();
	if(difficultyMode==3){
	    if (nowPlaying.personalScore > highscores3[numberOfSlots-1].personalScore){
			ifHigh=1;
		    fp = fopen ("High Scores3.txt", "w");
		
			highscores3[numberOfSlots-1]= nowPlaying;

		    for (i=0 ; i<numberOfSlots-1 ; i++)
		    {
			    max = highscores3[i] ;
			    for (j=i ; j<numberOfSlots ; j++)
			    {
				    if (highscores3[j].personalScore > max.personalScore)
				    {
					    temp = highscores3[j] ;
					    highscores3[j] = max ;
					    highscores3[i] = temp ;
				    }
			    }
		    }
		    for (i=0 ; i<numberOfSlots ; i++)
		    {
			    itoa(highscores3[i].personalScore,highscores3[i].scorePrint,10);
			    fprintf (fp,"%s %d %s\n" , highscores3[i].name, highscores3[i].personalScore,highscores3[i].scorePrint);
		    }
		    fclose(fp);
		}
	}
}

int ifPrevSaved ()
{
	int i;
	FILE *fp ;
	fp = fopen("Saved Game.txt", "r");
	if (fscanf(fp, "%d", &i)==EOF)
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}

void refreshFile (void)
{
	FILE *fp;
	fp = fopen ("Saved Game.txt", "w");
	fclose(fp);
}

void startGame ()
{
	resumeAll();
	start = 1 ;
	control = 1;
}


void reviveBird ()
{
	obsX1[0] = 1300 , obsX1[1] = 1600 , obsX1[2] = 1300 + 700 , obsX1[3] = 1300 + 900 ;
	obsX2[0] = 1800 , obsX2[1] = 1800+1200 , obsX2[2] = 1800 +750 , obsX2[3] = 1800+1000 ;
	bulletY=200 , bulletX = -300;
	birdY = 250 ;
	gravityDY = 3;
	brickX=3*screenW;
}

void resetAll ()
{
	obsX1[0] = 1300 , obsX1[1] = 1600 , obsX1[2] = 1300 + 700 , obsX1[3] = 1300 + 900 ;
	obsX2[0] = 1800 , obsX2[1] = 1800+1200 , obsX2[2] = 1800 +750 , obsX2[3] = 1800+1000 ;
	obsDX = 10;
	gravityDY = 3 ;
	bulletY=200 , bulletX = 0;
	birdY = 250 ;
	lifeBonusX = 10800 , life = 3;
	appleX = 1300 ;
	birdDY=20;
	sec=0, playTime=0;
	brickX=3*screenW;
	ifHigh = 0;
	birdX=500;
	playTime=0;
	endObs=0, birdMove=0, bringSky=0, won=0;

}

void iMouseMove(int mx, int my)
{

}

void iPassiveMouseMove(int mx, int my)
{

}
/* 
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	int i;
	if (control==0)//Mouse actions on the main menu page
	{

		if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		for (i=0 ; i<5 ; i++)
		{
			if ((mx>boxX[i]) && (mx<boxX[i]+258) && (my>boxY[i]) && (my<boxY[i]+48) )
			{
				blue[i] = 255 ;	
				boxY[i] -=3 ;
				boxX[i] -=3 ;
				break;
			}
		}
		
			if (mx>creditX && mx<creditX+70 && my>creditY && my<creditY+30)
			{
				creditX-=3;
				creditY-=3;
			}

		}

		if (button==GLUT_LEFT_BUTTON && state==GLUT_UP){
		for (i=0 ; i<5 ; i++)
		{
			if ((mx>boxX[i]) && (mx<boxX[i]+258) && (my>boxY[i]) && (my<boxY[i]+48) )
			{
				blue[i] = 0 ;
				boxY[i] +=3 ;
				boxX[i] +=3 ;


				if (i==0)//selects PLAY
				{
					if (ifPrevSaved() == 1)
					{
						control = 10;
						sec=0;
						break;
					}
					else
					{
						takeName ();
					}
				}

				else if (i==1)
				{control = 2; break; }//selects OPTIONS

				else if (i==2)
				{control = 3; break; }//selects HOW TO PLAY

				else if (i==3)
				{control = 4; break; }//selects HIGH SCORES
				
				else if (i==4)//selects QUIT
					exit(0);
			}
		}

			if (mx>creditX && mx<creditX+100 && my>creditY && my<creditY+30)
			{
				creditX+=3;
				creditY+=3;
				control=6;
			}

		}
	}

	
	if (control==10)
	{
			if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
			{
				
				if (mx>prevGameX && mx<prevGameX + whichGameBoxW && my>prevGameY && my<prevGameY + whichGameBoxH)
				{
	
				//	clicked = 1;
				}

				if (mx>newGameX && mx<newGameX + whichGameBoxW && my>newGameY && my<newGameY + whichGameBoxH)
				{
				//	clicked = 2;
				}
			}

			if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
			{
				
				if (mx>prevGameX && mx<prevGameX + whichGameBoxW && my>prevGameY && my<prevGameY + whichGameBoxH)
				{
					//clicked = 0;
					loadGame ();
					startGame ();	

				}

				if (mx>newGameX && mx<newGameX + whichGameBoxW && my>newGameY && my<newGameY + whichGameBoxH)
				{
					//clicked = 0;
					takeName();
				    life=3;
					
				}
			}
	}

	if (control==100)//Mouse actions on the nameBox page
	{
		if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
		{
			if ( (mx>arrowX) && (mx<arrowX+90) && (my>arrowY) && (my<arrowY+65) )
				arrowClicked = 1 ;
		}
		if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
		{
			if ( (mx>arrowX) && (mx<arrowX+110) && (my>arrowY) && (my<arrowY+65) )
			{
				arrowClicked = 0 ;
				control=0;

			}
		}
	}


	if (control==2)//Mouse actions on the options page
	{
		if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
			{
				if ( (mx>backX) && (mx<backX+100) && (my>backY) && (my<backY+40) )
					{
						backX-=3 ;
						backY-=3 ;
						blueBack = 255 ;
					}
			}

		if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
			{
				if ( (mx>easyX) && (mx<easyX+200) && (my>allY) && (my<allY+48) )//easy
					tickX = 225 , maxSpeed = 18,difficultyMode=1 ;
				if ( (mx>mediumX) && (mx<mediumX+200) && (my>allY) && (my<allY+48) )//medium
					tickX = 470, maxSpeed = 22,difficultyMode=2 ;
				if ( (mx>hardX) && (mx<hardX+200) && (my>allY) && (my<allY+48) )//hard
					tickX = 715, maxSpeed = 27,difficultyMode=3 ;

				if ( (mx>backX) && (mx<backX+100) && (my>backY) && (my<backY+40) )//back to the main menu
					{
						backX+=3 ;
						backY+=3 ;
						blueBack = 0 ;
						control = 0;
					}
			}
	}

	if (control==3)//Mouse actions on the instructions page
	{
		if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
		{
			if ( (mx>arrowX) && (mx<arrowX+90) && (my>arrowY) && (my<arrowY+65) )
				arrowClicked = 1 ;
		}
		if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
		{
			if ( (mx>arrowX) && (mx<arrowX+100) && (my>arrowY) && (my<arrowY+65) )
			{
				arrowClicked = 0 ;
				control = 0 ;

			}
		}
	}

	if (control==4)//Mouse actions on the high score page
	{
		if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
		{
			if ( (mx>arrowX) && (mx<arrowX+90) && (my>arrowY) && (my<arrowY+65) )
				arrowClicked = 1 ;
		}
		if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
		{
			if ( (mx>arrowX) && (mx<arrowX+110) && (my>arrowY) && (my<arrowY+65) )
			{
				arrowClicked = 0 ;
				if (gameFinished==1)
				{
					PlaySound("sounds/background.wav",NULL,SND_ASYNC);
					gameFinished =0 ;
				}
				control = 0 ;

			}
		}
	}

	if (control==6)//Mouse actions on the credits page
	{
		if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
		{
			if ( (mx>arrowX) && (mx<arrowX+90) && (my>arrowY) && (my<arrowY+65) )
				arrowClicked = 1 ;
		}
		if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
		{
			if ( (mx>arrowX) && (mx<arrowX+110) && (my>arrowY) && (my<arrowY+65) )
			{
				arrowClicked = 0 ;
				control=0;

			}
		}
	}
}

void iKeyboard(unsigned char key)
{
	int i;
	if (control==1 && key=='p')
	{
		pauseAll();
		paused=1;
	}

	if (paused==1 && key=='r')
	{
		resumeAll();
		paused = 0;
	}	
	
	if(control == 100)
	{
        if(key == '\r')
		{
			strcpy(nowPlaying.name, input);
			printf("%s\n", nowPlaying.name);
			for(i = 0; i < inputLen; i++)
				input[i] = 0;
			inputLen = 0;
			startGame();
		}
		else
		{
			if(inputLen<=14){
				if (key==8)
				{
					input[inputLen-1]=0;
				    inputLen-- ;
				}
				else if (key==' ')
				{
					input[inputLen] = '_';
					inputLen++;
				}
				else
				{
					input[inputLen] = key;
					inputLen++;
				}
			}
		}
	}
	if((control==5) &(key=='h')){
		control=4;
		resetAll ();
		gameFinished=1;
		//control=0 ;
		//PlaySound("sounds/background.wav",NULL,SND_ASYNC);
	}
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
	if (control==5 && key==GLUT_KEY_HOME) 
	{
		resetAll ();
		control=0 ;
		PlaySound("sounds/background.wav",NULL,SND_ASYNC);
	}

	if (control==1)
	{
		if (key==GLUT_KEY_UP && birdY<screenH-20)
		{
			birdY = birdY + birdDY;
			gravityDY = 0 ;
			birdDY+=2 ;
		}
		if(birdY>= BirdYMax)//prevents bird to go beyond the screen
		{
			birdY=BirdYMax;
		}
		if (key==GLUT_KEY_DOWN && birdY>20)
		{
			birdY = birdY - birdDY ;
			birdDY = 10;
		}
	}
	if (paused==1 && key==GLUT_KEY_HOME)
	{
		saveGame();
		resetAll ();
		control = 0;
		PlaySound("sounds/background.wav",NULL,SND_ASYNC);
		paused = 0;
	}
}


int main()
{
	int i;
	FILE *fp=NULL;
	fp = fopen ("High Scores1.txt", "r");
	for (i=0 ; i<numberOfSlots ; i++){
		fscanf (fp,"%s %d %s", highscores1[i].name , &highscores1[i].personalScore,&highscores1[i].scorePrint);
	}
	fclose(fp);
	fp = fopen ("High Scores2.txt", "r");
	for (i=0 ; i<numberOfSlots ; i++){
		fscanf (fp,"%s %d %s", highscores2[i].name , &highscores2[i].personalScore,&highscores2[i].scorePrint);
	}
	fclose(fp);
	fp = fopen ("High Scores3.txt", "r");
	for (i=0 ; i<numberOfSlots ; i++){
		fscanf (fp,"%s %d %s", highscores3[i].name , &highscores3[i].personalScore,&highscores3[i].scorePrint);
	}
	fclose(fp);

	for (i=0 ; i<screenParts ; i++)
	{
		moveX[i] = moveX[0]+i*partW ;
	}
	printf("max %d tick  %d diff %d\n",maxSpeed,tickX,difficultyMode);
	iSetTimer (30, obstacleMove); //0
	iSetTimer (7, bulletMove); //1
	iSetTimer (100,birdTimer); //2
	iSetTimer (200, gravity ) ; //3
	iSetTimer (1, ifCollissions); //4
	iSetTimer (4000,bambooSpeed); //5
	iSetTimer (100, randomNumberGenerator); //6
	iSetTimer (.01, distanceChecker); //7
	iSetTimer (100,scoring); //8

	PlaySound("sounds/background.wav",NULL,SND_ASYNC);

	iInitialize (screenW, screenH, "Break Free");	

	return 0;
}