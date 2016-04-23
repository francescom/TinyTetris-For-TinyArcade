#ifndef __LoopingBuffer_h
#define __LoopingBuffer_h
#include <stdint.h>
#include "Arduino.h"
#include <SdFat.h>


// The LoopingBuffer

// I called it a looping buffer (don't know if it has another name)
// a buffer that is not uniform from 0 to 1023 (or any size), but folds into itself,
// usually divided in two chunks, one from
// some "z" (the zero) to 1023 and one from 0 to (z-1)
// With this we can go on loading while we play, like a double buffer but flexible
// and without the overhead of.. well, a double buffer
//
// If we are playing at zero+used we can load from zero to zero+used-1 (replace already played)
// but if zero+used-1>1023 we fold, we stop at 1023 and loop to zero the remaing part
//
// data to be used is always from zero+used on (may require to loop to zero to be retrieved)



class LoopingBuffer {
public:
    unsigned char * buffer;
    SdFile *theFile;
    unsigned int zero;
    unsigned int used;
    unsigned int size;
    boolean inited=false;
    boolean needsLoading=true;
    
    uint32_t fileBufferStart=0;
    uint32_t fileBufferEnd=0;
    uint32_t fileNowPlaying=0;
    
    unsigned char *result1=buffer;
    unsigned int result1Size=0;
    unsigned char *result2=buffer;
    unsigned int result2Size=0;
    
    
    LoopingBuffer();
    LoopingBuffer(unsigned char * theActualBuffer,unsigned int bufferSize,SdFile *myFile);
    void init(unsigned char * theActualBuffer,unsigned int bufferSize,SdFile *myFile);
    void reset();
    void setUsed(unsigned int newPos);
    boolean moveToNext();
    
    unsigned char getByte();
    void prepareBytes(); // unused untested
    unsigned int getBytes1stSize(); // unused untested
    unsigned int getBytes2ndSize(); // unused untested
    
    void seek(uint32_t pos);
    void loadFromSdFile();
};



#endif // __LoopingBuffer_h
