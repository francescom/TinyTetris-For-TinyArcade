

///////////////////////////////////////////////////
///////////////////////////
///////////
//////
///         Play any frequency as obtaind from a frequency, an amplitude and a function(frequency,amplitude,frame)
//          To get a note frequency, like middle C, we can use frequencyFromNote(-9)
//


#include "AudioItemNote.h"



  
AudioItemNote::AudioItemNote() : AudioItem() {
}
AudioItemNote::AudioItemNote(uint32_t myPos,noteGeneratorFunction myFunc) : AudioItem() {
  init(myPos,myFunc);
}
AudioItemNote::AudioItemNote(uint32_t myPos,noteGeneratorFunction myFunc,int startingFreq,uint8_t startingAmplitude) : AudioItem() {
  init(myPos,myFunc,startingFreq,startingAmplitude);
}
void AudioItemNote::init(uint32_t myPos,noteGeneratorFunction myFunc) {
  AudioItem::init(myPos);
  generator=myFunc;
}
void AudioItemNote::init(uint32_t myPos,noteGeneratorFunction myFunc,int startingFreq,uint8_t startingAmplitude) {
  AudioItem::init(myPos);
  frequency=startingFreq;
  amplitude=startingAmplitude;
  generator=myFunc;
}
uint16_t AudioItemNote::getSampleAt(uint32_t pos) {
  if(!enabled) return 0;
  if(generator!=nullPtr) return reduceVolume(generator(frequency,amplitude,pos)); // Freq is multiplied by 10x
}



