#ifndef __AudioItemFile_h
#define __AudioItemFile_h

#include <SdFat.h>
#include "AudioItem.h"
#include "LoopingBuffer.h"
#include "tcaudio.h"




class AudioItemFile: public AudioItem {
    
public:
    
    SdFile myFile;
    char *fileName;
    const uint8_t bytesPerSample=1;
    static const unsigned int myBufferSize=bufferSize*2;
    unsigned char fileBuffer[myBufferSize];
    long fileSize = 0;
    LoopingBuffer loopingBuffer;
  	uint8_t boost=2; // MUST be 0,1 or 2: boost 8 bit sounds to 10 bits if 2 (to 9 bits if 1) 8bitSound<<boost
    
    
    AudioItemFile();
    AudioItemFile(uint32_t myPos,char *myFileName);
    void init(uint32_t myPos,char *myFileName);
    ~AudioItemFile();
    boolean loadIf();
    
    
    
    uint16_t getNextSample();
    // this is very slow for now as it will destroy the buffer (see TODO) use only for seeking
    uint16_t getSampleAt(uint32_t pos);
};




#endif // __AudioItemFile_h
