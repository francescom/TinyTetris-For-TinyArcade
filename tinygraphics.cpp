
#include "tinygraphics.h"
#include "colors.h"
#include "Arduino.h"


#include <TinyScreen.h>
extern TinyScreen display;


void fillRect(int x,int y,int x1,int y1,long col) {
	display.drawRect(min(x,x1),min(y,y1),constrain(abs(x1-x),0,96),constrain(abs(y1-y),0,64),1,getRed(col)>>3,getGreen(col)>>3,getBlue(col)>>3);
}
void frameRect(int x,int y,int x1,int y1,long col) {
	if(y<0) {
		SerialUSB.print("out ");
		SerialUSB.print(y);
		SerialUSB.print(", ");
		SerialUSB.print(y1);
		y=0;
	}
	display.drawRect(min(x,x1),min(y,y1),constrain(abs(x1-x),0,96),constrain(abs(y1-y),0,64),0,getRed(col)>>3,getGreen(col)>>3,getBlue(col)>>3);
}
void drawLine(int x,int y,int x1,int y1,long col) {
	display.drawLine(x,y,x1,y1,getRed(col)>>3,getGreen(col)>>3,getBlue(col)>>3);
}
