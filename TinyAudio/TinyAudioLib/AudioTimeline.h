#ifndef __AudioTimeline_h
#define __AudioTimeline_h

#include "AudioItem.h"
#include "tiny_audio_setup.h"


const unsigned int MAX_TIMELINE_CHANNELS=200;


class AudioTimeline: public AudioItem { // a timeline is really an audio item (could be nested)!
	public:
	
	AudioItem* allItems[MAX_TIMELINE_CHANNELS];
	unsigned int channels=0;
	uint32_t loopCount=0;
	
	
	AudioTimeline();
	AudioTimeline(uint32_t myPos);
	void clear();
	uint32_t now();
	int addItem(AudioItem * theItem);
	int addTemporaryItemNow(AudioItem * theItem);
	int addTemporaryItemAt(AudioItem * theItem,uint32_t frame);
	void removeItem(AudioItem * theItem);
	void compactChannels();
	void dispose();
	uint16_t getSampleAt(uint32_t pos);
	uint32_t nextFrame();
	uint32_t fixedFrame(uint32_t pos);
	uint16_t getNextSample();
	boolean loadIf();
	
	static inline uint16_t mixAdd(uint16_t a,uint16_t b) {
		return a+b-HALF_OF_DAC;
	}
	static inline uint16_t zeroSound() {
		return HALF_OF_DAC;
	}
	static inline uint16_t trim(uint16_t s) {
		return ((s>MIN_OF_DAC)?((s<MAX_OF_DAC)?s:MAX_OF_DAC):MIN_OF_DAC);
	}
};


#endif // __AudioTimeline

