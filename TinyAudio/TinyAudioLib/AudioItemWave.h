#ifndef __AudioItemWave_h
#define __AudioItemWave_h

///////////////////////////////////////////////////
///////////////////////////
///////////
//////
///         Play any wave as obtained from a function(frame) 
//
//



#include "AudioItem.h"
#include <stdint.h>
#include "Arduino.h"



typedef uint16_t (*waveGeneratorFunction)(uint32_t frame);



class AudioItemWave: public AudioItem {
  public:
  waveGeneratorFunction generator=nullPtr;
  
  AudioItemWave();
  AudioItemWave(uint32_t myPos,waveGeneratorFunction myFunc);
  void init(uint32_t myPos,waveGeneratorFunction myFunc);
  
  uint16_t getSampleAt(uint32_t pos);
};

#endif // __AudioItemWave_h
