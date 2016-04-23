#include "AudioItem.h"
#include <stdint.h>
#include "Arduino.h"

#include "tcaudio.h"
#include "AudioTimeline.h"



  AudioItem::AudioItem() {
    timelinePosition=0;
  }
  AudioItem::AudioItem(uint32_t myPos) {
    init(myPos);
  }
  void AudioItem::init(uint32_t myPos) {
    timelinePosition=myPos;
    length=0;
  }
  AudioItem::~AudioItem() {
  }
  void AudioItem::dispose() {}
  void AudioItem::stop() {
    // enabled=false;  
    playing=false;
  }
  void AudioItem::onEnded() {
    if(removeAtEnd && !loop) {
      remove=true;
      // stop();
      if(parent!=nullPtr) parent->compactChannels();
    }
  }
  void AudioItem::onLoop() {
    SerialUSB.println("[loop]");
  }

  void AudioItem::start() {
    enabled=true;  
    playing=true;
 }
 uint16_t AudioItem::getSampleAt(uint32_t pos) {
    soundHeadPos=pos;
    loopPos=(length!=0)?pos%length:pos;
    return 0;
  }
  uint16_t AudioItem::getNextSample() {
     return getSampleAt(++soundHeadPos);
  }
  boolean AudioItem::loadIf() {return false;}



  uint16_t AudioItem::eightBitsToUnsigned16(unsigned char b) { // TODO: optimize
    return (uint16_t)b*257; // 65535/255
  }
  int16_t AudioItem::eightBitsToSigned16(unsigned char b) { // TODO: optimize, *really*
    return (int16_t)((int32_t)b*257-32768); // 65535/255
  }
  
  uint16_t AudioItem::eightBitsToUnsigned12(unsigned char b) { // TODO: optimize
    return (uint16_t)b*16; // 4095/255 (yes *16 should be <<4)
  }
  int16_t AudioItem::eightBitsToSigned12(unsigned char b) { // TODO: optimize
    return (int16_t)b*16-2048; // -2048 -> 2047
}
