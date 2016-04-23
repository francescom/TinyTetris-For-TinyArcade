#ifndef __AudioItemNote_h
#define __AudioItemNote_h

#include "AudioItem.h"
#include <stdint.h>
#include "Arduino.h"


///////////////////////////////////////////////////
///////////////////////////
///////////
//////
///         Play any frequency as obtaind from a frequency, an amplitude and a function(frequency,amplitude,frame)
//          To get a note frequency, like middle C, we can use frequencyFromNote(-9)
//


typedef uint16_t (*noteGeneratorFunction)(uint_fast16_t freq,uint_fast8_t amplitude,uint32_t frame); // Freq is multiplied by 10x



class AudioItemNote: public AudioItem {
  public:
  noteGeneratorFunction generator=nullPtr;
  int frequency=4400; // note A4 is 440 // // Freq is multiplied by 10x
  uint8_t amplitude=100;
  
  AudioItemNote();
  AudioItemNote(uint32_t myPos,noteGeneratorFunction myFunc);
  AudioItemNote(uint32_t myPos,noteGeneratorFunction myFunc,int startingFreq,uint8_t startingAmplitude);
  void init(uint32_t myPos,noteGeneratorFunction myFunc);
  void init(uint32_t myPos,noteGeneratorFunction myFunc,int startingFreq,uint8_t startingAmplitude);
  uint16_t getSampleAt(uint32_t pos);
};


#endif // __AudioItemNote_h
