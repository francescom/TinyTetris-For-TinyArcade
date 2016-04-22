//-------------------------------------------------------------------------------
//  This code will collect all joystick info in RX,RY,LX,LY,leftButton,rightButton
//	RX,RY,LX,LY will be
// 		claned with smart threshold removal (movements little over threshold will give near zero values, not near threshold values)
//		max & min hardware values will be collected to calibrate joystick output
//		between -127 to 127, add 128 to get a byte 0..255 to send over serial. At the other side just subtract 128
//		leftButton,rightButton will both be 0 or 1 
//  
//  Based on the examples, ©2015 Francesco Munafò. Free to use.
//-------------------------------------------------------------------------------


#include "joystick-local.h"
#include "common.h"
#include "arduino.h"
#include <Wire.h>

namespace joy {
	
	int RX=0,RY=0,LX=0,LY=0;
	int maxRX=200,maxRY=200,maxLX=200,maxLY=200;
	int minRX=-200,minRY=-200,minLX=-200,minLY=-200;

	int leftButton=0;
	int rightButton=0;

	int sgn(int x) { return (x > 0) - (x < 0); }


	int joyCut(int val,int left,int right,int thresh,int range) {
		int ret=(int)((long)(val - left) * (range+thresh+range+thresh) / (right - left)) - (range+thresh);
		return (abs(ret)>thresh)?(abs(ret)-thresh)*sgn(ret):0;
	}



	void getJoystick() {
		const int thresh=64;
		const int range=127;

		int tRX;
		int tRY;
		int tLX;
		int tLY;


		leftButton = !digitalRead(TinyArcadePin1);
		rightButton = !digitalRead(TinyArcadePin2);
		tRX = 0;
		tRY = 0;
		tLX = -(analogRead(TinyArcadePinX)-512);
		tLY = -(analogRead(TinyArcadePinY)-512);
	
		 // Clean joystick analog values
		 // Out values will be 0 at center and -range, +range at min and max
		 // We save maximum raw values to trim current value
	
		if(tRX > maxRX) maxRX = tRX;
		if(tRY > maxRY) maxRY = tRY;
		if(tLX > maxLX) maxLX = tLX;
		if(tLY > maxLY) maxLY = tLY;

		if(tRX < minRX) minRX = tRX;
		if(tRY < minRY) minRY = tRY;
		if(tLX < minLX) minLX = tLX;
		if(tLY < minLY) minLY = tLY;
	
		LX=joyCut(tLX,minLX,maxLX,thresh,range); // Clean joystick analog values
		RX=joyCut(tRX,minRX,maxRX,thresh,range);
		LY=joyCut(tLY,minLY,maxLY,thresh,range);
		RY=joyCut(tRY,minRY,maxRY,thresh,range);
	}



}
