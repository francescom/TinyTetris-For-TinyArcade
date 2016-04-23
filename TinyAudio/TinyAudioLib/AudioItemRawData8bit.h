#ifndef __AudioItemRawData8bit_h
#define __AudioItemRawData8bit_h

///////////////////////////////////////////////////
///////////////////////////
///////////
//////
///         Play any digitized sample given as unsigned 8bit array
//
//

#include "AudioItem.h"
#include <stdint.h>
#include "Arduino.h"


class AudioItemRawData8bit: public AudioItem {
   public:
   unsigned char *data;
   
   AudioItemRawData8bit();
   AudioItemRawData8bit(uint32_t myPos,unsigned char *mydata,uint32_t myLen);
   void init(uint32_t myPos,unsigned char *mydata,uint32_t myLen);
  uint16_t getSampleAt(uint32_t pos);
};



#endif // __AudioItemRawData8bit_h
