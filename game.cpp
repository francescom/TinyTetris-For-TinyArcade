// #define prog_char const char PROGMEM


#include "Arduino.h"
#include "game.h"
#include "colors.h"
#include "tinygraphics.h"
#include "common.h"

#include <TinyScreen.h>
extern TinyScreen display;



bool audioIsPaused=false; // this is exported outside
bool gameIsOver=false;
bool audioOn=true; // this saves value after GameOvers



bool kLarger=false;

uint8_t xOrigin = 48; // 96x64 screen
uint8_t yOrigin = 60;

uint8_t blockSize=3;
const uint8_t maxFieldWidth=10;
const uint8_t maxFieldHeight=20;
uint8_t fieldWidth=10;
uint8_t fieldHeight=20;
bool bottomBorder=true;
uint16_t startingSpeed=800;

uint32_t score=0;
uint32_t lines=0;
uint16_t level=1;

void setupParams() {
	if(kLarger) {
	
		xOrigin = 48; // 96x64 screen
		yOrigin = 64;
		
		blockSize=4;
		fieldWidth=10;
		fieldHeight=16;
		bottomBorder=false;
		// startingSpeed=500;
	
	} else {
		xOrigin = 48; // 96x64 screen
		yOrigin = 60;
		
		blockSize=3;
		fieldWidth=10;
		fieldHeight=20;
		bottomBorder=true;
		// startingSpeed=500;
	}
	kLarger=!kLarger;
}

struct part {
  signed char x;
  signed char y;
};

struct parts {
  part items[4];
  part center;
  uint8_t color;
};

struct piece {
  part pos;
  parts structure;
};

parts defaults[7]={
	{{{0,-1},{0,0},{0,1},{0,2}},{0,0},1},
	{{{-1,0},{0,0},{1,0},{0,1}},{0,0},2},
	{{{-1,0},{0,0},{1,0},{1,1}},{0,0},3},
	{{{-1,0},{0,0},{1,0},{-1,1}},{0,0},4},
	{{{-1,1},{-1,0},{0,0},{0,-1}},{0,0},5},
	{{{-1,-1},{-1,0},{0,0},{0,1}},{0,0},6},
	{{{-1,1},{-1,0},{0,0},{0,1}},{-1,1},7}
};
long colors[]={0x000000,0xff0000,0x00ff00,0x0000ff,0xffff00,0xff00ff,0x00ffff,0xff7f00};
int colorMove=0;


uint8_t field[maxFieldWidth][maxFieldHeight];



piece currentPiece;
piece nextPiece;

uint8_t gameStatus=0;

uint32_t moveTime;
uint32_t moveTime2;
uint32_t dropTime;
uint16_t gameSpeed=startingSpeed;






uint8_t fieldGet(signed char x, signed char y) {
	if(x<0 || x>=fieldWidth ) return 0x0F;
	if(y>=fieldHeight) return 0;
	if(y<0 ) return 0x0F;
	return field[x][y];
}

void fieldSet(signed char x, signed char y, uint8_t val) {
	if(x<0 || x>=fieldWidth) {
		return;
	}
	if(y<0 || y>=fieldHeight) {
		return;
	}
	field[x][y]=val;
}

void serialPrintField() {
	for(uint8_t i=0;i<fieldHeight;i++) {
		for(uint8_t j=0;j<fieldWidth;j++) {
			SerialUSB.print(field[j][i]);
			SerialUSB.print(" ");
		}
		SerialUSB.println("\n");
	}
}


void clearBlock(int x,int y) {
	if(x<0 || x>=fieldWidth ) return;
	if(y<0 || y>=fieldHeight) return;
	x=xOrigin-fieldWidth*blockSize/2+x*blockSize;
	y=yOrigin-(y+1)*blockSize;
	fillRect(x,y,x+blockSize,y+blockSize,0x000000);
}

void drawBlockAt(int x,int y,long col) {  // x, y in screen coordinates
	fillRect(x+1,y+1,x+blockSize-1,y+blockSize-1,colorTint(col,-20));
	frameRect(x,y,x+blockSize,y+blockSize,colorTint(col,-70));
	drawLine(x,y,x+blockSize-2,y,colorTint(col,+50));
	drawLine(x,y,x,y+blockSize-2,colorTint(col,+50));
}


void drawBlock(int x,int y,long col) { // x, y in field/blocks coordinates
	if(x<0 || x>=fieldWidth ) return;
	if(y<0 || y>=fieldHeight) return;

	if(col==0) return clearBlock(x,y);
	x=xOrigin-fieldWidth*blockSize/2+x*blockSize;
	y=yOrigin-(y+1)*blockSize;
	
	drawBlockAt(x,y,col);
}

void showLevel() {
	display.setCursor(1,15);
	display.print("");
	display.print(level);
	display.print("");
}
void showScore() {
	display.setCursor(1,3);
	display.print("");
	display.print(score);
	display.print("");
}

void drawBackground() {
	int r,g,b;
	long color,color2;
	fillRect(0,0,96,64,0x000000);
	for(int y=0;y<64;y+=4) {
		r=128+(int)(127*cos((float)(y-0+colorMove)*6.28/64));
		g=128+(int)(127*cos((float)(y-21+colorMove)*6.28/64));
		b=128+(int)(127*cos((float)(y-42+colorMove)*6.28/64));
		color=rgbColor(r,g,b);
		for(int x=0;x<96;x+=4) {
			color2=colorTint(color,16-max(0,8*(abs(x-48)-32)));
			frameRect(x, y, x+3, y+3, color2);	

		}
	}
	
	colorMove+=2;
	int left=xOrigin-blockSize*fieldWidth/2;
	int top=yOrigin-blockSize*fieldHeight;
	int right=xOrigin+blockSize*fieldWidth/2;
	int bottom=yOrigin;

	
	fillRect(left,0,right,65,0x000000);

	drawLine(left-1,0,left-1,63,colorTint(0xffffff,-10));
	drawLine(left-2,0,left-2,63,colorTint(0xffffff,0));
	drawLine(left-3,0,left-3,63,colorTint(0xffffff,-30));

	drawLine(right,0,right,63,colorTint(0xffffff,-30));
	drawLine(right+1,0,right+1,63,colorTint(0xffffff,-10));
	drawLine(right+2,0,right+2,63,colorTint(0xffffff,0));

	if(bottomBorder) {
		drawLine(left-1,61,right,61,colorTint(0xffffff,-10));
		drawLine(left-2,62,right+1,62,colorTint(0xffffff,0));
		drawLine(left-3,63,right+2,63,colorTint(0xffffff,-30));
	}
	showScore();
	showLevel();

}

bool testBlock(signed char x,signed char y) {
	if(x<0) return true;
	if(x>=fieldWidth) return true;
	if(y<0) return true;
	
	/*per ora*/
	return fieldGet(x,y)!=0;
}


bool testPiece() {
	for(int i=0;i<4;i++) {
		if(testBlock(currentPiece.pos.x+currentPiece.structure.items[i].x,currentPiece.pos.y+currentPiece.structure.items[i].y)) {
			return true;
		}
	}
	return false;
}
void drawField(int fromLine) {
	int left=xOrigin-blockSize*fieldWidth/2;
	int top=yOrigin-blockSize*fieldHeight;
	int right=xOrigin+blockSize*fieldWidth/2;
	int bottom=yOrigin;
	byte block=0;
	
	
	//fillRect(left,0,right,63,0x000000);
	for(uint8_t i=fromLine;i<fieldHeight;i++) {
		for(uint8_t j=0;j<fieldWidth;j++) {
			block=fieldGet(j,i);
			drawBlock(j,i,colors[block]);
		}
	}
}
void stripLine(signed char l) {
	for(signed char y=l;y<fieldHeight;y++) {
		for(signed char x=0;x<fieldWidth;x++) fieldSet(x,y,fieldGet(x,y+1));
	}
}
void cleanField() {
	for(signed char y=0;y<fieldHeight;y++) {
		for(signed char x=0;x<fieldWidth;x++) fieldSet(x,y,0);
	}
}

bool testLine(signed char l) {
	for(signed char i=0;i<fieldWidth;i++) {
		if(fieldGet(i,l)==0) {return false;} // SerialUSB.print(" l=");SerialUSB.print((int)l);SerialUSB.print(" i=");SerialUSB.print((int)i);
		// else {SerialUSB.print(" v=");SerialUSB.print((int)fieldGet(i,l));}
	}
	// SerialUSB.print(" l=");SerialUSB.print((int)l);SerialUSB.println("");
	return true;
}

void cleanCurrentLines() {
	signed char l,min=fieldHeight+10,max=-fieldHeight-10;
	uint8_t countHits=0;
	
	for(int i=0;i<4;i++) {
		l=currentPiece.pos.y+currentPiece.structure.items[i].y;
		if(l<min) min=l;
		if(l>max) max=l;
	}
	drawField(min);
	for(int i=max;i>=min;i--) {
		if(testLine(i)) {
			countHits++;
			stripLine(i);
			drawField(i);
			score+=5+2*countHits;
			lines++;
			if(level*10<lines && level<=10) {
				level++;
				gameSpeed=startingSpeed/level+100;
				showLevel();
			}
			showScore();
		}
	}
	int left=xOrigin-blockSize*fieldWidth/2;
	int top=yOrigin-blockSize*fieldHeight;
	int right=xOrigin+blockSize*fieldWidth/2;
	int bottom=yOrigin;
	// fillRect(left,0,right,60,0x000000);
}
void freezePiece() {
	for(int i=0;i<4;i++) {
		fieldSet(currentPiece.pos.x+currentPiece.structure.items[i].x,currentPiece.pos.y+currentPiece.structure.items[i].y,currentPiece.structure.color);
	}
	cleanCurrentLines();
}

void rotatePieceLeft() {
	int t;
	for(int i=0;i<4;i++) {
		t=currentPiece.structure.items[i].y;
		currentPiece.structure.items[i].y=currentPiece.structure.items[i].x;
		currentPiece.structure.items[i].x=-t;
	}
}
void rotatePieceRight() {
	int t;
	for(int i=0;i<4;i++) {
		t=currentPiece.structure.items[i].y;
		currentPiece.structure.items[i].y=-currentPiece.structure.items[i].x;
		currentPiece.structure.items[i].x=t;
	}
}

void drawPiece() {
	for(int i=0;i<4;i++) {
		drawBlock(currentPiece.pos.x+currentPiece.structure.items[i].x,currentPiece.pos.y+currentPiece.structure.items[i].y,colors[currentPiece.structure.color]);
	}
}
void drawNext(int x,int y) {
	fillRect(x-blockSize*3,y-blockSize*3,x+blockSize*3,y+blockSize*3,0x000000);
	frameRect(x-blockSize*3,y-blockSize*3,x+blockSize*3,y+blockSize*3,0x777777);
	for(int i=0;i<4;i++) {
		drawBlockAt(x+blockSize*nextPiece.structure.items[i].x,y-blockSize*nextPiece.structure.items[i].y,colors[nextPiece.structure.color]);
	}
}
void clearPiece() {
	for(int i=0;i<4;i++) {
		drawBlock(currentPiece.pos.x+currentPiece.structure.items[i].x,currentPiece.pos.y+currentPiece.structure.items[i].y,colors[0]);
	}
}


void initPiece() {
	currentPiece=nextPiece;
	int currentPieceNum=random(7);
	nextPiece.structure=defaults[currentPieceNum];
	// currentPiece.pos.x=fieldWidth/2;
	// currentPiece.pos.y=fieldHeight+2;
	drawNext(96-blockSize*4,blockSize*4);
}



void resetGame() {
	setupParams();
	colorMove=0;
	gameSpeed=startingSpeed;
	score=0;
	lines=0;
	level=0;
	gameStatus=0;

	int currentPieceNum=random(7);
	nextPiece.structure=defaults[currentPieceNum];
	nextPiece.pos.x=fieldWidth/2;
	nextPiece.pos.y=fieldHeight+2;
	
	cleanField();
	drawBackground();
	initPiece();
	audioIsPaused=!audioOn;
	gameIsOver=false;
}


void gameSetup() {

fieldSet(5,5,3);
serialPrintField();
SerialUSB.println(fieldGet(5,5));


  
	randomSeed(analogRead(0));
	
	
	dropTime=millis();
	moveTime=millis();
	moveTime2=millis();
	
	resetGame();

}

void gameOver() {
	gameStatus=99;
	gameIsOver=true;
	audioIsPaused=true;
}



void gameLoop(int lX,int lY,int rX,int rY,int buttons) {
	
	boolean btnL=getButtonState(buttons,JOY_BUT_L);
	boolean btnR=getButtonState(buttons,JOY_BUT_R);

	boolean btnBL=getButtonState(buttons,TS_BUT_BL);
	boolean btnTL=getButtonState(buttons,TS_BUT_TL);
	boolean btnTR=getButtonState(buttons,TS_BUT_TR);
	boolean btnBR=getButtonState(buttons,TS_BUT_BR);
	
	if(btnR && btnL) {
		resetGame();
		delay(1000);
	}

    int delayAfterRender=0;
    char scoreStr[20];
    String(score/10).toCharArray(scoreStr,20);
    
	if(gameStatus!=99) { // Not game over
		
		if(false) {
			audioOn=!audioOn;
			audioIsPaused=!audioOn;
			if(audioIsPaused) {
			} else {
			}
		};
		
		clearPiece();
	    
	    if((lX>64)) {
	    	if(millis() - moveTime > gameSpeed/4+100) {
		    	currentPiece.pos.x++;
		    	if(testPiece()) {
		    		currentPiece.pos.x--;
		    	} else moveTime=millis();
	    	}
	    } else if(lX<-64) {
	    	if(millis() - moveTime > gameSpeed/4+100) {
	    		currentPiece.pos.x--;
		    	if(testPiece()) {
		    		currentPiece.pos.x++;
		    	} else moveTime=millis();
	    	}
	    } else moveTime=0;
	    
	    if((btnR) ) {
	    	if(millis() - moveTime2 > gameSpeed/4+100) { // do not do a simple AND with previous line (moveTime2=0; would fail)
		    	rotatePieceRight();
		    	if(testPiece()) { // Try adjacent positions before giving up
		    		currentPiece.pos.x++; // next right
		    		if(testPiece()) {
			    		currentPiece.pos.x-=2; // next left
			    		if(testPiece()) {
			    			currentPiece.pos.x+=3; // two right (for bar)
				      		if(testPiece()) {
				    			currentPiece.pos.x-=4; // two left (for bar)
					      		if(testPiece()) {
					      			currentPiece.pos.x+=2; // no way, can't turn
					   				rotatePieceLeft();
					    		} else moveTime2=millis();
				    		} else moveTime2=millis();
				   		} else moveTime2=millis();
			   		} else moveTime2=millis();
		    	} else moveTime2=millis();
	    	}
	    } else if(btnL) {
	    	if(millis() - moveTime2 > gameSpeed/4+100) { // do not do a simple AND with previous line (moveTime2=0; would fail)
		    	rotatePieceLeft();
		    	if(testPiece()) { // Try adjacent positions before giving up
		    		currentPiece.pos.x++; // next right
		    		if(testPiece()) {
			    		currentPiece.pos.x-=2; // next left
			    		if(testPiece()) {
			    			currentPiece.pos.x+=3; // two right (for bar)
				      		if(testPiece()) {
				    			currentPiece.pos.x-=4; // two left (for bar)
					      		if(testPiece()) {
					      			currentPiece.pos.x+=2; // no way, can't turn
					   				rotatePieceRight();
					    		} else moveTime2=millis();
				    		} else moveTime2=millis();
				   		} else moveTime2=millis();
			   		} else moveTime2=millis();
		    	} else moveTime2=millis();
	    	}
	    } else { // if REALYY not pressing rotation keys next press will be faster
	    	moveTime2=0;
	    }
	    
	    if(lY<-64 || (millis() - dropTime > gameSpeed)) {
	    	if(lY<-64) score++;
	 		  currentPiece.pos.y--;
	   		if(testPiece()) {
	  			currentPiece.pos.y++;
		   		freezePiece();
		   		if(currentPiece.pos.y>=fieldHeight) {
		   			gameOver();
		   		} else initPiece();
	    	} else {
				dropTime=millis();
	    	}
	    }
	    drawPiece();
	}
    
    if(delayAfterRender>0) delay(delayAfterRender);
}

