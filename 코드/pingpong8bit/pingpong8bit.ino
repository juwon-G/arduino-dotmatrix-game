#include <MsTimer2.h>
#include <LedControl.h>

#define CLK 2 // 7 세그먼트 CLK 2
#define DIO 3 // 7 세그먼트 DIO 3

LedControl Dote = LedControl(7,6,5,1); // 도트매트릭스 DIN 7, CLK 6, CS 5

int left = 8;  // 버튼
int up = 9;
int down = 10;
int right = 11;

unsigned char field[8];

char myCurrentPos = 4; // 나의 채 가운데 좌표
char comCurrentPos = 4; // 컴퓨터 채 가운데 좌표
char upNdown = 1; // 공 이동 상태 위 -1, 아래 1
char leftNright = 0; // 공 이동 상태 왼쪽 -1, 가운데 0, 오른쪽 1
char ballX = 4; // 공 x좌표
char ballY = 4; // 공 y좌표
char actionCase; // 컴퓨터 1, 나 2
char movePoint;
char score = 0;
char gamebool= 0;
int input;

void setDot(void){
  for (int i = 0;i < 8;i++) {
  field[i] = 0;
  }
}

void pingpong(void){
  spawnBall();
  PspawnBar();
  printDot();
  PballMove();
  moveCom();
}

void printDot() {
    for (int y = 0;y < 8;y++) { //도트 매트릭스 출력
      Dote.setRow(0,y,field[y]);
    }
}

void PspawnBar(void) {

    field[0] = 0;
    field[7] = 0;
    unsigned char comMask = 0xe0;
    unsigned char myMask = 0xe0;
    comMask >>= comCurrentPos - 2;
    myMask >>= myCurrentPos - 2;
    field[0] |= comMask;
    field[7] |= myMask;

}

void spawnBall(void) {

    unsigned char ballMask = 0x80;
    ballMask >>= ballX - 1;
    field[ballY] |= ballMask;
    
}

void moveCom(void) {
    movePoint = comCurrentPos + leftNright;
    if ( (movePoint < 2) || (movePoint > 7) )
        return;

    comCurrentPos = movePoint;
}


void moveLeft(void) {
    if (myCurrentPos == 2)
        return;
    myCurrentPos--;
}

void moveRight(void) {
    if (myCurrentPos == 7)
        return;
    myCurrentPos++;
}

void Paction() {
    switch (actionCase) {
    case 1:
        if (abs(ballX - comCurrentPos) > 1) {
          gamebool=1;
        }

        upNdown *= (-1);
        leftNright = ballX - comCurrentPos;

        break;
    case 2:
        if (abs(ballX - myCurrentPos) > 1) {
          gamebool=1;
        }

        upNdown *= (-1);
        leftNright = ballX - myCurrentPos;
        break;
    }
}

void PballMove(void) {
    if (actionCase) {
        Paction();
        actionCase = 0;
    }
    if(ballX == 1 || ballX == 8){
        leftNright *= (-1);
    }

    field[ballY] = 0;
    ballX += leftNright;
    ballY += upNdown;

    if (ballY == 1)
        actionCase = 1;
    else if (ballY == 6)
        actionCase = 2;

}


void setup(){
  pinMode(left, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  Dote.shutdown(0,false);
  Dote.setIntensity(0,7);
  Dote.clearDisplay(0);

  setDot();
  PspawnBar();
  spawnBall();
  MsTimer2::set(500,pingpong);
  MsTimer2::start();
}

void loop(){

if(gamebool){
    setDot();
    printDot();
    MsTimer2::stop();
  }
  if(digitalRead(left) ==LOW){
    moveLeft();
  PspawnBar();
  spawnBall();
  printDot();
  delay(200);
  }
  else if(digitalRead(right) ==LOW){
    moveRight();
  PspawnBar();
  spawnBall();
  printDot();
  delay(200);
  }
 
}
