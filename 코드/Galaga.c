#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define test printf("---\n");

typedef struct dotMatrix{
    unsigned short field[16];
}dotM;


void print(dotM dotMat){
    for(int y=0;y<16;y++){ //도트 매트릭스 출력
        for(int x=15; x>=0;x--){
            int result = dotMat.field[y] >> x & 1;
            printf("%d", result);
        }
        printf("\n");
    }
    printf("\n\n");    
}

void spawnPlayer(dotM *dotMat, int *currentPos){
    dotMat->field[13] |= (int)pow(2, 8);
    dotMat->field[14] |= (int)pow(2, 7);
    dotMat->field[14] |= (int)pow(2, 8);
    dotMat->field[14] |= (int)pow(2, 9);
    *currentPos = 8;
}

void moveLeft(dotM *dotMat, int *currentPos){
    if(dotMat->field[13] < pow(2, 14)){
        dotMat->field[13] <<= 1;
        dotMat->field[14] <<= 1;
        *(currentPos)+=1;
    }
}

void moveRight(dotM *dotMat, int *currentPos){
    if(dotMat->field[13] > 2){
        dotMat->field[13] >>= 1;
        dotMat->field[14] >>= 1;
        *(currentPos)-=1;
    }
}

void spawnEnemy(dotM *dotMat, int level){
    
    switch (level)
    {
    case 1:
        dotMat->field[1] |= (int)pow(2, 7);
        dotMat->field[1] |= (int)pow(2, 8);
        dotMat->field[1] |= (int)pow(2, 9);
        dotMat->field[2] |= (int)pow(2, 8);
        break;
    
    case 2:
        dotMat->field[1] |= (int)pow(2, 3);
        dotMat->field[1] |= (int)pow(2, 4);
        dotMat->field[1] |= (int)pow(2, 5);
        dotMat->field[2] |= (int)pow(2, 4);
        dotMat->field[1] |= (int)pow(2, 10);
        dotMat->field[1] |= (int)pow(2, 11);
        dotMat->field[1] |= (int)pow(2, 12);
        dotMat->field[2] |= (int)pow(2, 11);
        break;
    
    case 3:
        dotMat->field[1] |= (int)pow(2, 2);
        dotMat->field[1] |= (int)pow(2, 3);
        dotMat->field[1] |= (int)pow(2, 4);
        dotMat->field[2] |= (int)pow(2, 3);
        dotMat->field[1] |= (int)pow(2, 6);
        dotMat->field[1] |= (int)pow(2, 7);
        dotMat->field[1] |= (int)pow(2, 8);
        dotMat->field[2] |= (int)pow(2, 7);
        dotMat->field[1] |= (int)pow(2, 11);
        dotMat->field[1] |= (int)pow(2, 12);
        dotMat->field[1] |= (int)pow(2, 13);
        dotMat->field[2] |= (int)pow(2, 12);
        break;

    case 4:
        dotMat->field[1] |= (int)pow(2, 1);
        dotMat->field[1] |= (int)pow(2, 2);
        dotMat->field[1] |= (int)pow(2, 3);
        dotMat->field[2] |= (int)pow(2, 2);
        dotMat->field[1] |= (int)pow(2, 5);
        dotMat->field[1] |= (int)pow(2, 6);
        dotMat->field[1] |= (int)pow(2, 7);
        dotMat->field[2] |= (int)pow(2, 6);
        dotMat->field[1] |= (int)pow(2, 9);
        dotMat->field[1] |= (int)pow(2, 10);
        dotMat->field[1] |= (int)pow(2, 11);
        dotMat->field[2] |= (int)pow(2, 10);
        dotMat->field[1] |= (int)pow(2, 13);
        dotMat->field[1] |= (int)pow(2, 14);
        dotMat->field[1] |= (int)pow(2, 15);
        dotMat->field[2] |= (int)pow(2, 14);
        break;
    }
    

}

void enemyFire(dotM *dotMat, unsigned short *enemyBul){
    unsigned short mask = dotMat->field[2];
    dotMat->field[3] = mask;
    *enemyBul |= mask;
}

void fire(dotM *dotMat, int currentPos, unsigned short *myBullet){
    dotMat->field[12] |= (int)pow(2, currentPos);
    *myBullet |= (int)pow(2, currentPos);
}

void bulletUpdate(dotM *dotMat, unsigned short *myBullet, unsigned short *enemyBullet, unsigned short *bulletCrash ){
    unsigned short mask;
    unsigned short killMask = 0;
    int check = 0;
    if(*bulletCrash > 0){
        for(int z=3;z<13;z++){
        dotMat->field[z] &= ~*bulletCrash;
        }
        *bulletCrash = 0;
    }
    if((*myBullet & *enemyBullet) > 0){
        *bulletCrash = *myBullet & *enemyBullet;
        for(int c=3;c<13;c++){
            dotMat->field[c] |= *bulletCrash;
        }
        *myBullet &= ~*bulletCrash;
        *enemyBullet &= ~*bulletCrash;
    }
    for(int i=3;i<12; i++){

        mask = *myBullet & (dotMat -> field[i+1]);
        dotMat->field[i] |=  mask;
        dotMat->field[i+1] &= ~mask;
        mask = 0;
    }

    killMask = dotMat->field[1] & dotMat->field[3] & *myBullet;
    mask = dotMat->field[3] & *myBullet;
    if(killMask > 0){
        if((mask & dotMat->field[2]) > 0){
            mask |= (mask << 1);
            mask |= (mask >> 1);
            dotMat->field[2] &= ~mask;
            dotMat->field[1] &= ~mask;
        }else if(((mask>>1) & dotMat->field[2]) > 0){
            mask >>= 1;
            mask |= (mask << 1);
            mask |= (mask >> 1);
            dotMat->field[2] &= ~mask;
            dotMat->field[1] &= ~mask;
        }else if(((mask<<1) & dotMat->field[2]) > 0){
            mask <<= 1;
            mask |= (mask << 1);
            mask |= (mask >> 1);
            dotMat->field[2] &= ~mask;
            dotMat->field[1] &= ~mask;
        }

    }   
    for(int x=4;x<=12;x++){
        if((dotMat->field[x] & mask)> 0){
                check++;
                break;
        }
    }
    if(check == 0)
        *myBullet &= ~dotMat->field[3];
    dotMat->field[3] &= ~*myBullet;
    dotMat->field[3] &= ~mask;
    mask = 0;
    killMask = 0;
    //**********

    //Enemy
    for(int i=12;i>3; i--){
        mask = *enemyBullet & (dotMat -> field[i-1]);
        dotMat->field[i] |=  mask;
        dotMat->field[i-1] &= ~mask;
        mask = 0;
    }
    killMask = dotMat->field[14] & dotMat->field[12] & *enemyBullet;
    mask = dotMat->field[12] & *enemyBullet;
    if(killMask > 0){
        printf("GAMEOVER!!\n");
        exit(1);
    }   
    for(int x=3;x<=13;x++){
        if((dotMat->field[x] & mask)> 0){
                check++;
                break;
        }
    }
    if(check == 0)
        *enemyBullet &= ~dotMat->field[12];
    dotMat->field[12] &= ~*enemyBullet;
    dotMat->field[12] &= ~mask;
    mask = 0;

    
}

int main(void){
    srand(time(NULL));
    dotM dotMatrix;
    for(int i=0;i<16;i++){
        dotMatrix.field[i] = 0;
    }
    unsigned short enemyBullet = 0;
    unsigned short myBullet = 0;
    unsigned short bulletCrash = 0;
    int playerPos;
    int level = 1;

    print(dotMatrix);
    spawnPlayer(&dotMatrix, &playerPos);
    spawnEnemy(&dotMatrix, 1);
    int input;
    while(1){
        scanf("%d", &input);
        if(input == 1){
            moveLeft(&dotMatrix, &playerPos);
        }else if(input == 3){
            moveRight(&dotMatrix, &playerPos);
        }else if(input == 2){
            fire(&dotMatrix, playerPos, &myBullet);
        }
        if(rand()%7 == 0){
            enemyFire(&dotMatrix, &enemyBullet);
        }
        if(dotMatrix.field[2] == 0){
            if(level > 4){
                printf("Complete!\n");
                break;
            }
            level++;
            spawnEnemy(&dotMatrix, level);
        }
        bulletUpdate(&dotMatrix,&myBullet, &enemyBullet, &bulletCrash);
        print(dotMatrix);
    }
    return 0;
}