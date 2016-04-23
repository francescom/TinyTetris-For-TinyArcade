#ifndef __MainAudioTimeline_h
#define __MainAudioTimeline_h



#include "AudioTimeline.h"

class MainAudioTimeline: public AudioTimeline {
  public:
  uint32_t saveSampleIndex;
  uint32_t lastSampleIndex;
  
  MainAudioTimeline();
  int playEffect(AudioItem * theItem);
  
  void doLoop();
};


#endif // __MainAudioTimeline_h

