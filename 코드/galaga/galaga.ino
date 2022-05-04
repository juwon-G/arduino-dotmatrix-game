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

char enemyBullet = 0;
char myBullet = 0;
char bulletCrash = 0;

int playerPos;
int level = 1;
int input;
int gamebool=0;


void printDot() {
    for (int y = 0;y < 8;y++) { //도트 매트릭스 출력
      Dote.setRow(0,y,field[y]);
    }
}

void setDot(void){
  for (int i = 0;i < 8;i++) {
  field[i] = 0;
  }
}

void GALspawnEnemy(int level){
    
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

void GALspawnPlayer(int *currentPos){
    field[6] |= (int)pow(2, 4);
    field[7] |= (int)pow(2, 3);
    field[7] |= (int)pow(2, 5);
    field[7] |= (int)pow(2, 4);
    *currentPos = 4;
}

void GALmoveLeft(int *currentPos){
    if(field[6] < pow(2, 6)){
        field[6] <<= 1;
        field[7] <<= 1;
        *(currentPos)+=1;
    }
}

void GALmoveRight(int *currentPos){
    if(field[6] > pow(2, 1)){
        field[7] >>= 1;
        field[6] >>= 1;
        *(currentPos)-=1;
    }
}
void GALenemyFire(char *enemyBul){
    char mask = field[1];
    field[2] = (mask);
    *enemyBul |= (mask);
}

void GALfire(int currentPos, char *myBullet){
    field[5] |= ((int)pow(2, currentPos)+1);
    *myBullet |= ((int)pow(2, currentPos)+1);
}

void GALbulletUpdate(void){
    char mask;
    char killMask = 0;
    int check = 0;
    if(bulletCrash > 0){
        for(int z=2;z<5;z++){
        field[z] &= ~bulletCrash;
        }
        bulletCrash = 0;
    }
    if((myBullet & enemyBullet) > 0){
        bulletCrash = myBullet & enemyBullet;
        for(int c=2;c<5;c++){
            field[c] |= bulletCrash;
        }
        myBullet &= ~bulletCrash;
        enemyBullet &= ~bulletCrash;
    }
    for(int i=2;i<5; i++){

        mask = myBullet & (field[i+1]);
        field[i] |=  mask;
        field[i+1] &= ~mask;
        mask = 0;
    }

    killMask = field[0] & field[2] & myBullet;
    mask = field[2] & myBullet;
    if(killMask > 0){
        if((mask & field[1]) > 0){
            mask |= (mask << 1);
            mask |= (mask >> 1);
            field[1] &= ~mask;
            field[0] &= ~mask;
        }else if(((mask>>1) & field[1]) > 0){
            mask >>= 1;
            mask |= (mask << 1);
            mask |= (mask >> 1);
            field[1] &= ~mask;
            field[0] &= ~mask;
        }else if(((mask<<1) & field[1]) > 0){
            mask <<= 1;
            mask |= (mask << 1);
            mask |= (mask >> 1);
            field[1] &= ~mask;
            field[0] &= ~mask;
        }

    }   
    for(int x=2;x<=5;x++){
        if((field[x] & mask)> 0){
                check++;
                break;
        }
    }
    if(check == 0)
        myBullet &= ~field[2];
    field[2] &= ~myBullet;
    field[2] &= ~mask;
    mask = 0;
    killMask = 0;
    //**********

    //Enemy
    for(int i=5;i>2; i--){
        mask = enemyBullet & (field[i-1]);
        field[i] |=  mask;
        field[i-1] &= ~mask;
        mask = 0;
    }
    killMask = field[7] & field[5] & enemyBullet;
    mask = field[5] & enemyBullet;
    if(killMask > 0){
        gamebool=1;
    }   
    for(int x=2;x<=5;x++){
        if((field[x] & mask)> 0){
                check++;
                break;
        }
    }
    if(check == 0)
        enemyBullet &= ~field[5];
    field[5] &= ~enemyBullet;
    field[5] &= ~mask;
    mask = 0;
    printDot();
    
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
  GALspawnEnemy(1);
  GALspawnPlayer(&playerPos);
  printDot();
  MsTimer2::set(500,GALbulletUpdate);
  MsTimer2::start();
}

void loop(){

if(gamebool){
    setDot();
    printDot();
    MsTimer2::stop();
  }
  if(digitalRead(left) ==LOW){
    GALmoveLeft(&playerPos);
    delay(200);
  }
  else if(digitalRead(right) ==LOW){
    GALmoveRight(&playerPos);
    delay(200);
  }
  else if(digitalRead(up) ==LOW){
      GALfire(playerPos, &myBullet);
      delay(200);
  }
  if(random(400)==25){
      GALenemyFire(&enemyBullet);
  }
  if(field[0]==0){
      GALspawnEnemy(2);
  }
  printDot();
 
}
