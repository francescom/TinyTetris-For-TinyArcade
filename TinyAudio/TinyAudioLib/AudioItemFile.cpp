#include "AudioItemFile.h"

#include "tcaudio.h"




AudioItemFile::AudioItemFile() : AudioItem() {
 
}
AudioItemFile::AudioItemFile(uint32_t myPos,char *myFileName) : AudioItem(myPos) {
  init(myPos,myFileName);
  
}
void AudioItemFile::init(uint32_t myPos,char *myFileName) {
 
  AudioItem::init(myPos);
  fileName=myFileName;
  
  SerialUSB.print("Ready to load ");
  SerialUSB.println(myFileName);
  
  
   if(!(myFile.open(myFileName, O_READ))) {
    error=true;
    errorTxt="Audio file Load failed.";
    enabled=false;
    SerialUSB.print(errorTxt);
    SerialUSB.print(" ");
    SerialUSB.println(fileName);
    return;
  }
  fileSize = myFile.fileSize();
  length=fileSize/bytesPerSample;

  SerialUSB.print(F("Audio file "));
  SerialUSB.print(fileName);
  SerialUSB.print(F(" found, size: "));
  SerialUSB.println(fileSize);
  myFile.curPosition();
  myFile.seekSet(myFile.curPosition() );
  loopingBuffer.init(fileBuffer,myBufferSize,&myFile);
  loadIf();
}

AudioItemFile::~AudioItemFile() {
    if(myFile.isOpen()) myFile.close();
}
boolean AudioItemFile::loadIf() { // You can call this fast in a loop, not an interrupt as may load from SD
    // SerialUSB.print("L");
    if(loopingBuffer.needsLoading) {
        loopingBuffer.loadFromSdFile();
        // SerialUSB.println("Y");
       return true;
    }
    // SerialUSB.println("N");
    return false;
}

int16_t playFreq(int f,uint8_t a,uint32_t x);

uint16_t AudioItemFile::getNextSample() {
  
    if(!enabled || !playing) return 0;
    uint32_t oldLoopPos=loopPos;
    loopPos=soundHeadPos%length;

    if(soundHeadPos>=length) {
      if(loop) {
            if(loopPos!=oldLoopPos+1) {
              loopingBuffer.seek(loopPos);
              onLoop();
            }
      } else {
        onEnded();
        SerialUSB.print("ended");
        stop();
        return 0;
      }
    }

    if(loopingBuffer.moveToNext()) {
        soundHeadPos++;
        return loopingBuffer.getByte();
    } else {
        // error=true;
        // errorTxt="Head out of buffer"; //  (did you forget to load?)
        SerialUSB.print("[OOB]");
        // soundHeadPos++; // to avoid seek at next iteration
        return 0;
    }
}
// this is very slow for now as it will destroy the buffer (see TODO) use only for seeking
uint16_t AudioItemFile::getSampleAt(uint32_t pos) {
    if(!enabled || !playing) {
      SerialUSB.println("dis");
      return 0;
    }
     if(pos==soundHeadPos+1) { // just a hack to avoid destroying th buffer if you use getSampleAt instead of getNextSample
        return getNextSample();
    }
    soundHeadPos=pos;
    loopingBuffer.seek(pos);
    return loopingBuffer.getByte();
}

