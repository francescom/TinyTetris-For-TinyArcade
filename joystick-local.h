#ifndef JoystickLocal_h
#define JoystickLocal_h
#include "common.h"

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

namespace joy {
	
	extern int RX,RY,LX,LY;
	extern int maxRX,maxRY,maxL,maxLY;
	extern int minRX,minRY,minLX,minLY;

	extern int leftButton;
	extern int rightButton;



	void getJoystick();



}
#endif
