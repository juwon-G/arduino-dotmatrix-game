#include <stdio.h>
#include <stdlib.h>

typedef struct dotMatrix {
    unsigned short field[16];
}dotM;

char myCurrentPos = 8; // 나의 채 가운데 좌표
char comCurrentPos = 8; // 컴퓨터 채 가운데 좌표
char upNdown = 1; // 공 이동 상태 위 -1, 아래 1
char leftNright = 0; // 공 이동 상태 왼쪽 -1, 가운데 0, 오른쪽 1
char ballX = 8; // 공 x좌표
char ballY = 8; // 공 y좌표
char actionCase; // 컴퓨터 1, 나 2
char movePoint;
char score = 0;

void print(dotM *dotMat) {
    for (int y = 0;y < 16;y++) { //도트 매트릭스 출력
        for (int x = 15; x >= 0;x--) {
            int result = dotMat->field[y] >> x & 1;
            printf("%d", result);
        }
        printf("\n");
    }
    printf("\n\n");
}

void spawnBar(dotM* dotMat) {

    dotMat->field[0] = 0;
    dotMat->field[15] = 0;
    unsigned short comMask = 0xe000;
    unsigned short myMask = 0xe000;
    comMask >>= comCurrentPos - 2;
    myMask >>= myCurrentPos - 2;
    dotMat->field[0] |= comMask;
    dotMat->field[15] |= myMask;

}

void spawnBall(dotM* dotMat) {

    unsigned short ballMask = 0x8000;
    ballMask >>= ballX - 1;
    dotMat->field[ballY] |= ballMask;
    
}

void moveCom(dotM* dotMat) {
    movePoint = comCurrentPos + leftNright;
    if ( (movePoint < 2) || (movePoint > 15) )
        return;

    comCurrentPos = movePoint;

}


void moveLeft(dotM* dotMat) {
    if (myCurrentPos == 2)
        return;
    myCurrentPos--;
}

void moveRight(dotM* dotMat) {
    if (myCurrentPos == 15)
        return;
    myCurrentPos++;
}

void action() {
    switch (actionCase) {
    case 1:
        if (abs(ballX - comCurrentPos) > 1) {
            printf("\nGame over\n");
            exit(1);
        }

        upNdown *= (-1);
        leftNright = ballX - comCurrentPos;

        break;
    case 2:
        if (abs(ballX - myCurrentPos) > 1) {
            printf("\nGame over\n");
            exit(1);
        }

        upNdown *= (-1);
        leftNright = ballX - myCurrentPos;
        break;
    }
}

void ballMove(dotM* dotMat) {
    if (actionCase) {
        action();
        actionCase = 0;
    }
    if(ballX == 1 || ballX == 16){
        leftNright *= (-1);
    }

    dotMat->field[ballY] = 0;
    ballX += leftNright;
    ballY += upNdown;

    if (ballY == 1)
        actionCase = 1;
    else if (ballY == 14)
        actionCase = 2;

}

int main(void) {
    dotM *dotMatrix;
    dotMatrix = (dotM*)malloc(sizeof(dotM));

    for (int i = 0;i < 16;i++) {
        dotMatrix->field[i] = 0;
    }

    int input;
    while (1) {
        system("clear");

        printf("my:%hhd com:%hhd U&D:%hhd L&R:%hhd X:%hhd Y:%hhd\n\n", myCurrentPos, comCurrentPos, upNdown, leftNright, ballX, ballY);
        spawnBar(dotMatrix);
        spawnBall(dotMatrix);
        print(dotMatrix);
        scanf("%d", &input);
        if (input == 1) {
            moveLeft(dotMatrix);
        }
        else if (input == 3) {
            moveRight(dotMatrix);
        }
        ballMove(dotMatrix);
        moveCom(dotMatrix);
        
    }
    return 0;
}

