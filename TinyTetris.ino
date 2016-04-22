
// This is TinyTetris for TinyArcade





#include <TinyScreen.h>

#include "common.h"

#include <Wire.h>
#include <SPI.h>
#include "joystick-local.h"
#include "colors.h"
#include "game.h"
#include "tinygraphics.h"



unsigned long millisTime;



TinyScreen display = TinyScreen(TinyScreenPlus);


void setup () {
  analogWrite(A0, analogRead(A0));//Set audio output to avoid picking up noise, even though audio isn't used
  pinMode(TinyArcadePin2, INPUT_PULLUP);
  pinMode(TinyArcadePin1, INPUT_PULLUP);
  display.begin();
  display.setFont(thinPixel7_10ptFontInfo);
	
	
	// SerialUSB.begin(115200);
  delay(1000);
	display.begin();
	// display.setFlip(1); Requires key flipping
	display.setBrightness(15);
	display.setFont(thinPixel7_10ptFontInfo);
 
	millisTime=millis();
  SerialUSB.println("Ready.");
	gameSetup();
}

void loop() {
	joy::getJoystick();
	if(audioIsPaused) {}
  	
  	int buttons=0;

	buttons|=(joy::leftButton<<JOY_BUT_L)|(joy::rightButton<<JOY_BUT_R);

	if(millis() - millisTime > 20) {
		gameLoop(joy::LX,joy::LY,joy::RX,joy::RY,buttons);
		millisTime=millis();
	}
}

