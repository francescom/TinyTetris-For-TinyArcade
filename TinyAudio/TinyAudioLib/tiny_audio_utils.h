
#include <SPI.h>



extern PROGMEM  prog_uchar sine256[];


volatile prog_uchar getSineFromMem(uint_fast8_t pos);


// Freq is multiplied by 10x
uint16_t playSawFreq(uint_fast16_t f,uint_fast8_t a,uint32_t x);


// Freq is multiplied by 10x
uint16_t playSineFreq(uint_fast16_t f,uint_fast8_t a,uint32_t x);
// find a nice instrument play function (organ is the bast candidate 440Hz+660Hz*.2 http://music.stackexchange.com/questions/8214/how-can-i-replicate-the-sound-of-an-instrument )




// Middle C is -9
// Freq is multiplied by 10x
int frequencyFromNote(int halfStepsFromA4);



