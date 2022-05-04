#include <MsTimer2.h>
#include <LedControl.h>

LedControl Dote = LedControl(7, 6, 5, 1);

int left = 8;
int up = 9;
int down = 10;
int right = 11;

unsigned char field[8];

char myCurrentPos = 4; // pingpong, breakout
char comCurrentPos = 4; // pingpong
char upNdown = 1; // 공 이동 상태 위 -1, 아래 1
char leftNright = 0; // 공 이동 상태 왼쪽 -1, 가운데 0, 오른쪽 1
char ballX = 4; // 공 x좌표
char ballY = 4; // 공 y좌표
char expectX; // 공 예상 x좌표
char expectY; // 공 예상 y좌표
char actionCase; // 컴퓨터 1, 나 2
char movePoint;
char startPoint = 1;
char score = 0;
char gamebool = 0;
char enemyBullet = 0;
char myBullet = 0;
char bulletCrash = 0;

int playerPos;
int level = 1;
int input;
int gamechoose = 0;

void gamestart() {
  for (int y = 0; y < 8; y++) { //도트 매트릭스 출력
    Dote.setRow(0, y, 0x44);
  }
}

void setDot() {
  for (int i = 0; i < 8; i++) {
    field[i] = 0;
  }
}

void printDot() {
  for (int y = 0; y < 8; y++) { //도트 매트릭스 출력
    Dote.setRow(0, y, field[y]);
  }
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


void pingpong(void) {
  spawnBall();
  PspawnBar();
  printDot();
  PballMove();
  moveCom();
}

void moveCom(void) {
  movePoint = comCurrentPos + leftNright;
  if ( (movePoint < 2) || (movePoint > 7) )
    return;

  comCurrentPos = movePoint;
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

void Paction() {
  switch (actionCase) {
    case 1:
      if (abs(ballX - comCurrentPos) > 1) {
        gamebool = 1;
      }

      upNdown *= (-1);
      leftNright = ballX - comCurrentPos;

      break;
    case 2:
      if (abs(ballX - myCurrentPos) > 1) {
        gamebool = 1;
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
  if (ballX == 1 || ballX == 8) {
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


void breakout(void) {
  spawnBall();
  spawnBar();
  printDot();
  ballMove();
}

void spawnBlocks() {

  for (int i = 0; i < 4; i++)
    field[i] |= 0xff;

}

void spawnBar() {

  field[7] = 0;
  unsigned char barMask = 0xe0;
  barMask >>= myCurrentPos - 2;
  field[7] |= barMask;
}

void actionBlock(char x, char y) {

  unsigned char actionMask = 0xc0;
  actionMask >>= x / 2 * 2;
  field[y / 2 * 2] -= actionMask;
  field[y / 2 * 2 + 1] -= actionMask;

  score += 4;
  if (score == 32) {
    gamebool = 1;
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
        gamebool = 1;
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
      actionBlock(ballX - 1, expectY);
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

void spawnBall(void) {

  unsigned char ballMask = 0x80;
  ballMask >>= ballX - 1;
  field[ballY] |= ballMask;

}


void avoid_the_ball(void) {
  spawnball();
  spawndot();
  dotfall();
  printDot();
}

void spawnball(void) {
  unsigned char ballMask = 0200;
  unsigned char dotX = random(8);
  ballMask >>= dotX;
  field[0] |= ballMask;
}

void dotfall(void) {
  unsigned char mask;
  for (int t = 6; t >= 0; t--) {
    mask = 0;
    mask |= field[t];
    field[t] = 0;
    field[t + 1] |= mask;
  }
}

void spawndot(void) {
  field[7] = 0;
  unsigned char myMask = 0200;
  myMask >>= myCurrentPos - 1;
  field[7] |= myMask;
}

void GALspawnEnemy(int level) {

  switch (level)
  {
    case 1:
      field[0] |= (int)pow(2, 3);
      field[0] |= (int)pow(2, 4);
      field[0] |= (int)pow(2, 5);
      field[1] |= (int)pow(2, 4);
      break;

    case 2:
      field[0] |= (int)pow(2, 1);
      field[0] |= (int)pow(2, 2);
      field[0] |= (int)pow(2, 3);
      field[1] |= (int)pow(2, 2);
      field[0] |= (int)pow(2, 5);
      field[0] |= (int)pow(2, 6);
      field[0] |= (int)pow(2, 7);
      field[1] |= (int)pow(2, 6);
      break;
  }

}

void GALspawnPlayer(int *currentPos) {
  field[6] |= (int)pow(2, 4);
  field[7] |= (int)pow(2, 3);
  field[7] |= (int)pow(2, 5);
  field[7] |= (int)pow(2, 4);
  *currentPos = 4;
}

void GALmoveLeft(int *currentPos) {
  if (field[6] < pow(2, 6)) {
    field[6] <<= 1;
    field[7] <<= 1;
    *(currentPos) += 1;
  }
}

void GALmoveRight(int *currentPos) {
  if (field[6] > pow(2, 1)) {
    field[7] >>= 1;
    field[6] >>= 1;
    *(currentPos) -= 1;
  }
}
void GALenemyFire(char *enemyBul) {
  char mask = field[1];
  field[2] = (mask);
  *enemyBul |= (mask);
}

void GALfire(int currentPos, char *myBullet) {
  field[5] |= ((int)pow(2, currentPos) + 1);
  *myBullet |= ((int)pow(2, currentPos) + 1);
}

void GALbulletUpdate(void) {
  char mask;
  char killMask = 0;
  int check = 0;
  if (bulletCrash > 0) {
    for (int z = 2; z < 5; z++) {
      field[z] &= ~bulletCrash;
    }
    bulletCrash = 0;
  }
  if ((myBullet & enemyBullet) > 0) {
    bulletCrash = myBullet & enemyBullet;
    for (int c = 2; c < 5; c++) {
      field[c] |= bulletCrash;
    }
    myBullet &= ~bulletCrash;
    enemyBullet &= ~bulletCrash;
  }
  for (int i = 2; i < 5; i++) {

    mask = myBullet & (field[i + 1]);
    field[i] |=  mask;
    field[i + 1] &= ~mask;
    mask = 0;
  }

  killMask = field[0] & field[2] & myBullet;
  mask = field[2] & myBullet;
  if (killMask > 0) {
    if ((mask & field[1]) > 0) {
      mask |= (mask << 1);
      mask |= (mask >> 1);
      field[1] &= ~mask;
      field[0] &= ~mask;
    } else if (((mask >> 1) & field[1]) > 0) {
      mask >>= 1;
      mask |= (mask << 1);
      mask |= (mask >> 1);
      field[1] &= ~mask;
      field[0] &= ~mask;
    } else if (((mask << 1) & field[1]) > 0) {
      mask <<= 1;
      mask |= (mask << 1);
      mask |= (mask >> 1);
      field[1] &= ~mask;
      field[0] &= ~mask;
    }

  }
  for (int x = 2; x <= 5; x++) {
    if ((field[x] & mask) > 0) {
      check++;
      break;
    }
  }
  if (check == 0)
    myBullet &= ~field[2];
  field[2] &= ~myBullet;
  field[2] &= ~mask;
  mask = 0;
  killMask = 0;
  //**********

  //Enemy
  for (int i = 5; i > 2; i--) {
    mask = enemyBullet & (field[i - 1]);
    field[i] |=  mask;
    field[i - 1] &= ~mask;
    mask = 0;
  }
  killMask = field[7] & field[5] & enemyBullet;
  mask = field[5] & enemyBullet;
  if (killMask > 0) {
    gamebool = 1;
  }
  for (int x = 2; x <= 5; x++) {
    if ((field[x] & mask) > 0) {
      check++;
      break;
    }
  }
  if (check == 0)
    enemyBullet &= ~field[5];
  field[5] &= ~enemyBullet;
  field[5] &= ~mask;
  mask = 0;
  printDot();

}

void setup() {
  pinMode(left, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  Dote.shutdown(0, false);
  Dote.setIntensity(0, 7);
  Dote.clearDisplay(0);

}

void loop() {
  switch (gamechoose) {
    case 0:
      gamestart();
      if (digitalRead(left) == LOW) {
        gamechoose = 1;
        setDot();
        PspawnBar();
        spawnBall();
        MsTimer2::set(500, pingpong);
        MsTimer2::start();
        delay(200);
      }
      else if (digitalRead(up) == LOW) {
        gamechoose = 2;
        ballY = 6;
        setDot();
        spawnBlocks();
        spawnBar();
        spawnBall();
        MsTimer2::set(500, breakout);
        MsTimer2::start();
        delay(200);
      }
      else if (digitalRead(down) == LOW) {
        gamechoose = 3;
        setDot();
        spawnball();
        spawndot();
        MsTimer2::set(500, avoid_the_ball);
        MsTimer2::start();
        delay(200);
      }
      else if (digitalRead(right) == LOW) {
        gamechoose = 4;
        setDot();
        GALspawnEnemy(1);
        GALspawnPlayer(&playerPos);
        printDot();
        MsTimer2::set(500, GALbulletUpdate);
        MsTimer2::start();
        delay(200);
      }
      break;
    case 1:
      if (gamebool) {
        setDot();
        printDot();
        MsTimer2::stop();
      }
      if (digitalRead(left) == LOW) {
        moveLeft();
        PspawnBar();
        spawnBall();
        printDot();
        delay(200);
      }
      else if (digitalRead(right) == LOW) {
        moveRight();
        PspawnBar();
        spawnBall();
        printDot();
        delay(200);
      }
      break;
    case 2:
      if (gamebool) {
        setDot();
        printDot();
        MsTimer2::stop();
      }
      if (digitalRead(left) == LOW) {
        moveLeft();
        spawnBar();
        spawnBall();
        printDot();
        delay(200);
      }
      else if (digitalRead(right) == LOW) {
        moveRight();
        spawnBar();
        spawnBall();
        printDot();
        delay(200);
      }
      break;
    case 3:

      if (digitalRead(left) == LOW) {
        moveLeft();
        spawnball();
        spawndot();
        printDot();
        delay(200);
      }
      else if (digitalRead(right) == LOW) {
        moveRight();
        spawnball();
        spawndot();
        printDot();
        delay(200);
      }

      if (((field[6] >> (8 - myCurrentPos)) & 1 ) > 0) {
        MsTimer2::stop();
        setDot();
        printDot();
      }
      break;

    case 4:
      if (gamebool) {
        setDot();
        printDot();
        MsTimer2::stop();
      }
      if (digitalRead(left) == LOW) {
        GALmoveLeft(&playerPos);
        delay(200);
      }
      else if (digitalRead(right) == LOW) {
        GALmoveRight(&playerPos);
        delay(200);
      }
      else if (digitalRead(up) == LOW) {
        GALfire(playerPos, &myBullet);
        delay(200);
      }
      if (random(400) == 25) {
        GALenemyFire(&enemyBullet);
      }
      if (field[0] == 0) {
        GALspawnEnemy(2);
      }
      printDot();
      break;
  }
  while (gamebool) {}



}
