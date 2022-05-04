#include <MsTimer2.h>
#include <LedControl.h>

LedControl Dote = LedControl(7,6,5,1); // 도트매트릭스 DIN 7, CLK 6, CS 5

int left = 8;  // 버튼
int up = 9;
int down = 10;
int right = 11;

unsigned char field[8];
char myCurrentPos = 4;



void setDot(void){
  for (int i = 0;i < 8;i++) {
  field[i] = 0;
  }
}

void avoid_the_ball(void) {
  spawnball();
  spawndot();
  dotfall();
  printdot();
}

void setup () {
  pinMode(left, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  Dote.shutdown(0,false);
  Dote.setIntensity(0,7);
  Dote.clearDisplay(0);
  
  setDot();
  spawnball();
  spawndot();
  MsTimer2::set(500,avoid_the_ball);
  MsTimer2::start();
}

void spawnball(void) {
  unsigned char ballMask = 0200;
  unsigned char dotX = random(8);
  ballMask >>= dotX;
  field[0] |= ballMask;
}

void dotfall(void) {
  unsigned char mask;
  for (int t = 6; t>=0; t--) { 
    mask = 0;
    mask |= field[t];
    field[t] = 0;
    field[t+1] |= mask;
  }
}

void printdot(void) {
  for (int y = 0; y < 8; y++) {
    Dote.setRow(0,y,field[y]);
  }
}

void moveLeft(void) {
  if (myCurrentPos == 1)
    return;
  myCurrentPos--;
}

void moveRight(void) {
  if (myCurrentPos == 8)
    return;
  myCurrentPos++;
}

void spawndot(void) {
  field[7] = 0;
  unsigned char myMask = 0200;
  myMask >>= myCurrentPos -1;
  field[7] |= myMask;
}


void loop() {
    

  
}
