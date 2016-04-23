
///////////////////////////////////////////////////
///////////////////////////
///////////
//////
///         Play any wave as obtaind from a function(frame) 
//
//


#include "AudioItemWave.h"



AudioItemWave::AudioItemWave() : AudioItem() {
}
AudioItemWave::AudioItemWave(uint32_t myPos,waveGeneratorFunction myFunc) : AudioItem() {
  init(myPos,myFunc);
}
void AudioItemWave::init(uint32_t myPos,waveGeneratorFunction myFunc) {
  AudioItem::init(myPos);
  generator=myFunc;
}

uint16_t AudioItemWave::getSampleAt(uint32_t pos) {
  if(!enabled) return 0;
  if(generator!=nullPtr) return generator(pos);
}

