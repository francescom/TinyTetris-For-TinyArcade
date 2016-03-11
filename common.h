#ifndef COMMON_H
#define COMMON_H



#define TS_BUT_BL 0
#define TS_BUT_TL 1
#define TS_BUT_TR 2
#define TS_BUT_BR 3

#define JOY_BUT_L 4
#define JOY_BUT_R 5


#define TinyArcadePinX 42
#define TinyArcadePinY 1
#define TinyArcadePin1 45
#define TinyArcadePin2 44


inline int getButtonState(int buttons,int pos) {
  return (buttons>>pos & 0x1);
}



#endif
