#ifndef __tcaudio_h
#define __tcaudio_h

#include <stdint.h>
#include "Arduino.h"

const uint16_t bufferSize = 2048;
extern uint16_t audioBuffer[bufferSize];
extern volatile uint32_t sampleIndex;
extern volatile boolean audioInterruptPaused;


void tcConfigure(uint32_t sampleRate);
bool tcIsSyncing();
void tcStartCounter();
void tcReset();
void tcDisable();

#ifdef __cplusplus
extern "C" {
#endif

extern void Audio_Handler (void);

#ifdef __cplusplus
}
#endif




#endif // __tcaudio_h
