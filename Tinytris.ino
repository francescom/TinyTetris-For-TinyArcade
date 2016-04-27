
// This is Tinytris for the TinyArcade





#include <TinyScreen.h>

#include "common.h"

#include <SdFat.h>
#include <Wire.h>
#include <SPI.h>
#include "TinyAudio/TinyAudio.h"

#include "joystick-local.h"
#include "colors.h"
#include "game.h"
#include "tinygraphics.h"


#include "audio/brb.h"
#include "audio/ting.h"
#include "audio/click.h"
#include "audio/bip.h"

unsigned long millisTime;

SdFat sd;



TinyScreen display = TinyScreen(TinyScreenPlus);


AudioItemRawData8bit hitEffect; // (0,(unsigned char*)brb,4412);
AudioItemRawData8bit freezeEffect; // (0,(unsigned char*)brb,4412);
AudioItemRawData8bit lostEffect; // (0,(unsigned char*)brb,4412);
AudioItemRawData8bit dropLineEffect[4]; // (0,(unsigned char*)brb,4412);
AudioItemFile testStream; // (0,(char*)"/Tinytris/music.raw");

AudioItemWave testSoundWave; // (0,waveGeneratorTest);
AudioItemNote testSoundNote; // (0,playSineFreq);

MainAudioTimeline audioTimeline;




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

   if (!sd.begin(10, SPI_FULL_SPEED)) {
    SerialUSB.println("Card failed");
  //  while (1);
  }
  SPI.setClockDivider(0);

  //audio output config
  analogWrite(A0, 0);
  tcConfigure(11025);
  hitEffect.init(0,(unsigned char*)click,110);
  freezeEffect.init(0,(unsigned char*)bip,1089);
  lostEffect.init(0,(unsigned char*)ting,6457);
  dropLineEffect[0].init(0,(unsigned char*)brb,4412);
  dropLineEffect[1].init(0,(unsigned char*)brb,4412); // same data, different playing info (head position, status)
  dropLineEffect[2].init(0,(unsigned char*)brb,4412); // same data
  dropLineEffect[3].init(0,(unsigned char*)ting,6457);
  
  hitEffect.volumePerc=15;
  dropLineEffect[0].volumePerc=50;
  dropLineEffect[1].volumePerc=50;
  dropLineEffect[2].volumePerc=50;
  dropLineEffect[3].volumePerc=50;
  
  testStream.init(0,(char*)"/Tinytris/music.raw");
  testStream.loop=true;
  audioTimeline.loop=true;
  testStream.volumePerc=50;
  
  audioTimeline.length=testStream.length;
  
  audioTimeline.addItem((AudioItem*)&testStream);
  tcStartCounter();

  // audioTimeline.playEffect((AudioItem*)&dropLineEffect[0]);
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
  audioTimeline.doLoop();
}

