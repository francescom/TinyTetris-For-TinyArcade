#ifndef __AudioTimeline_h
#define __AudioTimeline_h

#include "AudioItem.h"


const unsigned int MAX_TIMELINE_CHANNELS=100;

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
};


#endif // __AudioTimeline

