
#include "AudioTimeline.h"




AudioTimeline::AudioTimeline() : AudioItem(0) {
  clear();
}
AudioTimeline::AudioTimeline(uint32_t myPos) : AudioItem(myPos) {
  clear();
}

void AudioTimeline::clear() {
  for(unsigned char i=0;i<channels;i++) allItems[i]=nullPtr;
  channels=0;
}

uint32_t AudioTimeline::now() {
  return soundHeadPos;
}

int AudioTimeline::addItem(AudioItem * theItem) {
  if(channels>=MAX_TIMELINE_CHANNELS) {
     error=true; // MMmmm.. this will kill all the timeline playing forever for one channel overflow... 8| ok for debugging, but..
     errorTxt="Channels exausted";
     SerialUSB.print("X");
     return -1;
  }
  theItem->parent=this;
  allItems[channels++]=theItem;
  SerialUSB.print("+");
  SerialUSB.println(channels);
  return channels;
}

int AudioTimeline::addTemporaryItemNow(AudioItem * theItem) {
  return addTemporaryItemAt(theItem,now());
}

int AudioTimeline::addTemporaryItemAt(AudioItem * theItem,uint32_t frame) {
  if(channels>=MAX_TIMELINE_CHANNELS) {
     error=true; // MMmmm.. this will kill all the timeline playing forever for one channel overflow... 8| ok for debugging, but..
     errorTxt="Channels exausted";
     SerialUSB.print("X");
     return -1;
  }
  
  /* UNSURE */
  if(theItem->parent==this) return -1;
  
  /* VERY UNSURE */
  if(theItem->parent!=nullPtr) theItem->parent->removeItem(theItem);
  
  /* END */
  
  
  theItem->timelinePosition=frame;
  theItem->soundHeadPos=0;
  theItem->loop=false;
  theItem->removeAtEnd=true;
  theItem->playing=true;
  theItem->enabled=true;

  // compactChannels();
  return addItem(theItem);
}

void AudioTimeline::removeItem(AudioItem * theItem) {
  uint_fast8_t move=0;
  for(uint_fast8_t i=0;i<channels;i++) {
    if(move) allItems[i-move]=allItems[i];
    if(allItems[i]==theItem) {
      allItems[i]->remove=false;
      allItems[i]->parent=nullPtr;
      allItems[i]=nullPtr;
      move++;
    }
  }
  channels-=move;
}
void AudioTimeline::compactChannels() {
  uint_fast8_t move=0;
  for(uint_fast8_t i=0;i<channels;i++) {
    if(move) allItems[i-move]=allItems[i];
    if(allItems[i]==nullPtr || allItems[i]->remove ) {
      allItems[i]->remove=false;
      allItems[i]->parent=nullPtr;
      allItems[i]=nullPtr;
      move++;
    }
  }
  channels-=move;
}

void AudioTimeline::dispose() { // not a distructor, just useless at the moment
  for(unsigned char i=0;i<channels;i++) {
    if(allItems[MAX_TIMELINE_CHANNELS]!=nullPtr) allItems[i]->dispose();
  }
}
uint16_t AudioTimeline::getSampleAt(uint32_t pos) {
  uint16_t sample=zeroSound();

  uint32_t iHaveLength=0;
  uint32_t itemHasLength=0;

  fixedFrame(pos);
  
  if(loop && length!=0) iHaveLength=length;
  
  for(unsigned char i=0;i<channels;i++) {

    if(!allItems[i]->enabled || allItems[i]->error || allItems[i]->remove) continue;
  
   if(allItems[i]->length!=0 && !allItems[i]->loop) itemHasLength=allItems[i]->length;

   uint32_t itemStart=allItems[i]->timelinePosition;
   uint32_t itemHasEnd=itemHasLength?itemStart+itemHasLength:0L;

   if(iHaveLength && itemStart>iHaveLength) continue; // starts out (=after end) of timeline


    if(iHaveLength && itemHasLength) { // Timeline is limited and sound too
      if(itemHasEnd>iHaveLength) itemHasEnd=itemHasEnd%iHaveLength; // don't if equal or end will be 0
      if(itemHasEnd>=itemStart) {
        if(pos<itemStart || pos>itemHasEnd) continue; // out of sound time
        
        
        sample=mixAdd(sample,allItems[i]->getSampleAt(pos-itemStart));
        
      } else { // soumd starts inside timeline and ends out of timeline the end is % (modulus) length so it becomes smaller than start
        // problems if single sound is longer than the timeline, but this is for effect sounds, should not happen
        if(pos<itemStart && pos>itemHasEnd) continue; // out of sound time: between end(<) and start(>)
        sample=mixAdd(sample,allItems[i]->getSampleAt(pos+(iHaveLength-itemStart)));
      }
    } else if(itemHasLength) {  // Sound is limited but timeline is unlimited
      if(pos<itemStart || pos>itemHasEnd) continue; // out of sound time
      sample=mixAdd(sample,allItems[i]->getSampleAt(pos-itemStart));
    } else if(iHaveLength) { // Timeline is limited and sound is infinite or looping
      if(pos<itemStart) continue; // out of sound time
      sample=mixAdd(sample,allItems[i]->getSampleAt(pos-itemStart));
    } else {
      if(pos<itemStart && loopCount==0) continue; // out of sound time
      sample=mixAdd(sample,allItems[i]->getSampleAt(pos-itemStart));
    }
  }
  return trim(reduceVolume(sample));
}

uint32_t AudioTimeline::nextFrame() {
  if(!loop || length==0) return ++soundHeadPos;
  if(++soundHeadPos>=length) {
    soundHeadPos=0;
    loopCount++;
  }
  return soundHeadPos;
}
uint32_t AudioTimeline::fixedFrame(uint32_t pos) { // does not handle loopCount correctly as this expects pos<length already
  if(!loop || length==0) return soundHeadPos=pos;
  soundHeadPos=pos%length; // just in case
  if(pos>length) loopCount=pos/length; // just in case
  return soundHeadPos;
}
uint16_t AudioTimeline::getNextSample() {
  return getSampleAt(nextFrame());
}
boolean AudioTimeline::loadIf() {
  boolean res=false;
  for(unsigned char i=0;i<channels;i++) {
    if(allItems[i]->enabled && allItems[i]->playing && !allItems[i]->error) res=res || allItems[i]->loadIf() ;
  }
  return res;
}

