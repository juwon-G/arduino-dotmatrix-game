#include <LedControl.h>
#include <MsTimer2.h>

LedControl Dote = LedControl(7,6,5,1);

int left = 8;  // 버튼
int up = 9;
int down = 10;
int right = 11;

unsigned char field[8];

char myCurrentPos = 4; // 나의 채 가운데 좌표
char upNdown = 1; // 공 이동 상태 위 -1, 아래 1
char leftNright = 0; // 공 이동 상태 왼쪽 -1, 가운데 0, 오른쪽 1
char ballX = 4; // 공 x좌표
char ballY = 6; // 공 y좌표
char expectX; // 공 예상 x좌표
char expectY; // 공 예상 y좌표
char actionCase; // 블럭 1, 나 2
char startPoint=1;
char score = 0;
char gamebool=0;
int input;

void setDot(){
  for (int i = 0;i < 8;i++) {
  field[i] = 0;
  }
}

void breakout(void){
  spawnBall();
  spawnBar();
  printDot();
  ballMove();
}

void printDot() {
    for (int y = 0;y < 8;y++) { //도트 매트릭스 출력
      Dote.setRow(0,y,field[y]);
    }
}

void spawnBlocks() {

    for(int i=0;i<4;i++)
        field[i] |= 0xff;
    
}

void spawnBar() {

    field[7] = 0;
    unsigned char barMask = 0xe0;
    barMask >>= myCurrentPos - 2;
    field[7] |= barMask;
}

void spawnBall() {

    unsigned char ballMask = 0x80;
    ballMask >>= ballX - 1;
    field[ballY] |= ballMask;
}

void moveLeft() {
    if (myCurrentPos == 2)
        return;
    myCurrentPos--;
}

void moveRight() {
    if (myCurrentPos == 7)
        return;
    myCurrentPos++;
}

void actionBlock(char x, char y) {

    unsigned char actionMask = 0xc0;
    actionMask >>= x / 2 * 2;
    field[y / 2 * 2] -= actionMask;
    field[y / 2 * 2 + 1] -= actionMask;
    
    score += 4;
    if (score == 32) {
      gamebool=1;
        }
}

void action() {

    unsigned char actionMask = 0x80;
    switch (actionCase) {
    case 2:
        if (field[7] & (0x80 >> expectX - 1)) {
            upNdown *= (-1);
            if (((expectX - myCurrentPos) * leftNright) == -1) {
                leftNright *= (-1);
            }
        }
        else {
          gamebool=1;
        }
        break;
    case 3:
        upNdown *= (-1);
        leftNright *= (-1);
        actionBlock(ballX - 1, expectY);
        actionBlock(expectX - 1, ballY);
        break;
    case 4:
        upNdown *= (-1);
        actionBlock(ballX-1, expectY);
        break;
    case 5:
        leftNright *= (-1);
        actionBlock(expectX - 1, ballY);
        break;
    case 6:
        upNdown *= (-1);
        leftNright *= (-1);
        actionBlock(expectX - 1, expectY);
        break;
    }
}

void ballMove() {

    if (ballX == 1 || ballX == 8) {
        leftNright *= (-1);
    }

    if (actionCase) {
        action();
        actionCase = 0;
    }

    if (startPoint) {
        startPoint = 0;
        upNdown *= (-1);
        leftNright = ballX - myCurrentPos;

    }
    unsigned char ballMask = 0x80;
    unsigned char UnDMask = 0x80;
    unsigned char LnRMask = 0x80;
    ballMask >>= ballX - 1;
    field[ballY] -= ballMask;

    ballX += leftNright;
    ballY += upNdown;

    expectX = ballX + leftNright;
    expectY = ballY + upNdown;

    if (expectX == 9)
        expectX = 7;
    if (expectX == 0)
        expectX = 2;

    UnDMask >>= ballX - 1;
    LnRMask >>= expectX - 1;

    if (ballY == 0)
        upNdown *= (-1);

    if (ballY == 6)
        actionCase = 2;
    else if (field[expectY] & UnDMask && field[ballY] & LnRMask)
        actionCase = 3;
    else if (field[expectY] & UnDMask)
        actionCase = 4;
    else if (field[ballY] & LnRMask)
        actionCase = 5;
    else if (field[expectY] & LnRMask)
        actionCase = 6;
        
}

void setup() {
  pinMode(left, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  Dote.shutdown(0,false);
  Dote.setIntensity(0,7);
  Dote.clearDisplay(0);
  
  setDot();
  spawnBlocks();
  spawnBar();
  spawnBall();
  MsTimer2::set(500,breakout);
  MsTimer2::start();
}

void loop() {

  if(gamebool){
    setDot();
    printDot();
    MsTimer2::stop();
  }
if(digitalRead(left) ==LOW){
    moveLeft();
  spawnBar();
  spawnBall();
  printDot();
  delay(200);
  }
  else if(digitalRead(right) ==LOW){
    moveRight();
  spawnBar();
  spawnBall();
  printDot();
  delay(200);
  }

}
