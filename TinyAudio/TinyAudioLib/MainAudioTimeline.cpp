
#include "MainAudioTimeline.h"

#include "tcaudio.h"


  
MainAudioTimeline::MainAudioTimeline() : AudioTimeline() {
  lastSampleIndex=sampleIndex;
}
int MainAudioTimeline::playEffect(AudioItem * theItem) {
  addTemporaryItemNow(theItem);
}

void MainAudioTimeline::doLoop() {
  if(!playing) return;
  loadIf();
  saveSampleIndex=sampleIndex;
  if(saveSampleIndex==lastSampleIndex) return;
  if(saveSampleIndex>lastSampleIndex) {
    
    for(uint32_t i=lastSampleIndex;i<saveSampleIndex;i++) {
      audioBuffer[i]=getSampleAt(nextFrame());
    }
  } else {
    for(uint32_t i=lastSampleIndex;i<bufferSize;i++) audioBuffer[i]=getSampleAt(nextFrame());
    for(uint32_t i=0;i<saveSampleIndex;i++) audioBuffer[i]=getSampleAt(nextFrame());
  }
  lastSampleIndex=saveSampleIndex;
}


