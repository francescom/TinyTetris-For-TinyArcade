

///////////////////////////////////////////////////
///////////////////////////
///////////
//////
///         Play any digitized sample as unsigned 8bit array
//
//

#include "AudioItemRawData8bit.h"


AudioItemRawData8bit::AudioItemRawData8bit() : AudioItem() {}
AudioItemRawData8bit::AudioItemRawData8bit(uint32_t myPos,unsigned char *mydata,uint32_t myLen) : AudioItem(myPos) {
  init(myPos,mydata,myLen);
}
void AudioItemRawData8bit::init(uint32_t myPos,unsigned char *mydata,uint32_t myLen) {
  AudioItem::init(myPos);
  data=mydata;
  length=myLen;
}

uint16_t AudioItemRawData8bit::getSampleAt(uint32_t pos) {
  if(!enabled) return 0;

  soundHeadPos=pos;
  uint32_t oldLoopPos=loopPos;
  loopPos=pos%length;
  
  if(soundHeadPos>=length) {
    if(loop) {
      if(loopPos!=oldLoopPos+1) {
        onLoop();
      }
    } else {
      onEnded();
      stop();
      return 0;
    }
  }
  return data[pos%length];
 }

