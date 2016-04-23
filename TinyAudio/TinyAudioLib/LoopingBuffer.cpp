#include "LoopingBuffer.h"
#include <stdint.h>
#include "Arduino.h"

#include "tcaudio.h"


int16_t playFreq(int f,uint8_t a,uint32_t x);



  LoopingBuffer::LoopingBuffer() {
  }
  LoopingBuffer::LoopingBuffer(unsigned char * theActualBuffer,unsigned int bufferSize,SdFile *myFile) {
    init(theActualBuffer,bufferSize,myFile);
   }
  void LoopingBuffer::init(unsigned char * theActualBuffer,unsigned int bufferSize,SdFile *myFile) {
    size=bufferSize;
    buffer=theActualBuffer;
    theFile=myFile;
    inited=true;
    reset();
   }
  void LoopingBuffer::reset() {
    fileBufferStart=0;
    fileBufferEnd=0+size-1;
    zero=0;
    used=0;
    fileNowPlaying=fileBufferStart+used;
    
    (*theFile).rewind();
    (*theFile).read(buffer+zero,size-zero);
    needsLoading=false;
   }
  void LoopingBuffer::setUsed(unsigned int newPos) { // this is pos inside the buffer as if not looping
    used=newPos;
    fileNowPlaying=fileBufferStart+used;
    needsLoading=used>size*3/4;
  }
  boolean LoopingBuffer::moveToNext() {
    used++;
    if(used>=size) return false;
    fileNowPlaying=fileBufferStart+used;
    needsLoading=used>size*3/4;
    return true;
  }
  
  unsigned char LoopingBuffer::getByte() {
    return buffer[(zero+used)%size];
  }
  void LoopingBuffer::prepareBytes() {
    result1=&(buffer[(zero+used)%size]);
    result1Size=getBytes1stSize();
    result2=buffer; // if result2Size is zero, don't use this
    result2Size=getBytes2ndSize();
 }
  unsigned int LoopingBuffer::getBytes1stSize() {
    unsigned int starting=(zero-1)%size;
    unsigned int ending=(zero+used)%size;

    if(ending>starting) return (ending-starting);
    else return (size-starting);
  }
  unsigned int LoopingBuffer::getBytes2ndSize() {
    unsigned int starting=(zero-1)%size;
    unsigned int ending=(zero+used)%size;

    if(ending>starting) return 0;
    else return (ending);
  }
  
  void LoopingBuffer::seek(uint32_t pos) { // invalidate the whole buffer (TODO: be smart, maybe new position is in buffer)
     SerialUSB.println("seek");
     fileBufferStart=pos;
    fileBufferEnd=pos+size-1;
    zero=0;
    used=0;
    fileNowPlaying=fileBufferStart+used;
    
    (*theFile).seekSet(pos);
    (*theFile).read(buffer+zero,size-zero);
    needsLoading=false;
  }
  void LoopingBuffer::loadFromSdFile() {
     fileBufferStart=fileBufferStart+used;
    if(zero+used<=size) {
      (*theFile).read(buffer+zero,used); // from buffer+zero to buffer+zero+used-1
      zero=zero+used;
      used=0;
    } else {
      (*theFile).read(buffer+zero,size-zero);
      (*theFile).read(buffer,zero=((zero+used)%size)); // from buffer+0 to (buffer+zero+used)%size-1 // and sets new zero at the same time
      used=0;
    }
    fileBufferEnd=(*theFile).curPosition();
    needsLoading=false;
  }

