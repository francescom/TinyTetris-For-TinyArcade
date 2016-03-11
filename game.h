// #define prog_char const char PROGMEM
const char audioPin=9;


#ifndef GAME
#define GAME




/* ..Declarations.. */

void gameSetup();
void gameLoop(int lX,int lY,int rX,int rY,int buttons);

void playNote(unsigned int frequency);
void playShotHits();

extern bool audioIsPaused;
extern bool gameIsOver;

#endif
