#include <stdio.h>
#include <stdlib.h>

unsigned field[16];

char myCurrentPos = 8; // 나의 채 가운데 좌표
char upNdown = 1; // 공 이동 상태 위 -1, 아래 1
char leftNright = 0; // 공 이동 상태 왼쪽 -1, 가운데 0, 오른쪽 1
char ballX = 8; // 공 x좌표
char ballY = 14; // 공 y좌표
char expectX; // 공 예상 x좌표
char expectY; // 공 예상 y좌표
char actionCase; // 블럭 1, 나 2
char startPoint = 1;
char score = 0;

void print() {
    for (int y = 0;y < 16;y++) { //도트 매트릭스 출력
        for (int x = 15; x >= 0;x--) {
            int result = field[y] >> x & 1;
            printf("%d", result);
        }
        printf("\n");
    }
    printf("\n\n");
}

void spawnBlocks() {

    for(int i=0;i<6;i++)
        field[i] |= 0xffff;
    
}

void spawnBar() {

    field[15] = 0;
    unsigned barMask = 0xe000;
    barMask >>= myCurrentPos - 2;
    field[15] |= barMask;
}

void spawnBall() {

    unsigned ballMask = 0x8000;
    ballMask >>= ballX - 1;
    field[ballY] |= ballMask;
}

void moveLeft() {
    if (myCurrentPos == 2)
        return;
    myCurrentPos--;
}

void moveRight() {
    if (myCurrentPos == 15)
        return;
    myCurrentPos++;
}

void actionBlock(char x, char y) {

    unsigned actionMask = 0xc000;
    actionMask >>= x / 2 * 2;
    field[y / 2 * 2] -= actionMask;
    field[y / 2 * 2 + 1] -= actionMask;
    
    score += 4;
}
void action() {

    unsigned actionMask = 0x8000;
    switch (actionCase) {
    case 2:
        if (field[15] & (0x8000 >> expectX - 1)) {
            upNdown *= (-1);
            printf("%d", ((short)expectX - myCurrentPos) * leftNright);
            if ((((short)expectX - myCurrentPos) * leftNright) == -1) {
                leftNright *= (-1);
            }
        }
        else {
            printf("\nGame over\n");
            exit(1);
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

    if (ballX == 1 || ballX == 16) {
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
    unsigned ballMask = 0x8000;
    unsigned UnDMask = 0x8000;
    unsigned LnRMask = 0x8000;
    ballMask >>= ballX - 1;
    field[ballY] -= ballMask;

    ballX += leftNright;
    ballY += upNdown;

    expectX = ballX + leftNright;
    expectY = ballY + upNdown;

    if (expectX == 17)
        expectX = 15;
    if (expectX == 0)
        expectX = 2;

    UnDMask >>= ballX - 1;
    LnRMask >>= expectX - 1;

    if (ballY == 0)
        upNdown *= (-1);

    if (ballY == 14)
        actionCase = 2;
    else if (field[expectY] & UnDMask && field[ballY] & LnRMask)
        actionCase = 3;
    else if (field[expectY] & UnDMask)
        actionCase = 4;
    else if (field[ballY] & LnRMask)
        actionCase = 5;
    else if (field[expectY] & LnRMask)
        actionCase = 6;
    
    printf("%d", actionCase);
}

int main(void) {
    
    for (int i = 0;i < 16;i++) {
        field[i] = 0;
    }
    spawnBlocks();

    int input;
    while (1) {
        system("clear");

        printf("\nmy:%hhd U&D:%hhd L&R:%hhd X:%hhd Y:%hhd eX:%hhd eY:%hhd\n\n", myCurrentPos, upNdown, leftNright, ballX, ballY,expectX, expectY);
        spawnBar();
        spawnBall();
        print();
        if (score == 96) {
            printf("clear!");
            exit(1);
        }
        scanf("%d", &input);
        if (input == 1) {
            moveLeft();
        }
        else if (input == 3) {
            moveRight();
        }
        spawnBar();

        ballMove();
       
    }
    return 0;
}

/*
#include <stdio.h>
#include <stdlib.h>

typedef struct dotMatrix {
    unsigned field[16];
}dotM;

char myCurrentPos = 8; // 나의 채 가운데 좌표
char upNdown = 1; // 공 이동 상태 위 -1, 아래 1
char leftNright = 0; // 공 이동 상태 왼쪽 -1, 가운데 0, 오른쪽 1
char ballX = 8; // 공 x좌표
char ballY = 14; // 공 y좌표
char expectX; // 공 예상 x좌표
char expectY; // 공 예상 y좌표
char actionCase; // 블럭 1, 나 2
char startPoint = 1;
char score = 0;

void print(dotM* dotMat) {
    for (int y = 0;y < 16;y++) { //도트 매트릭스 출력
        for (int x = 15; x >= 0;x--) {
            int result = dotMat->field[y] >> x & 1;
            printf("%d", result);
        }
        printf("\n");
    }
    printf("\n\n");
}

void spawnBlocks(dotM* dotMat) {

    for(int i=0;i<6;i++)
        dotMat->field[i] |= 0xffff;

}

void spawnBar(dotM* dotMat) {

    dotMat->field[15] = 0;
    unsigned barMask = 0xe000;
    barMask >>= myCurrentPos - 2;
    dotMat->field[15] |= barMask;
}

void spawnBall(dotM* dotMat) {

    unsigned ballMask = 0x8000;
    ballMask >>= ballX - 1;
    dotMat->field[ballY] |= ballMask;
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

void actionBlock(dotM* dotMat, char x, char y) {

    unsigned actionMask = 0xc000;
    actionMask >>= x / 2 * 2;
    dotMat->field[y / 2 * 2] -= actionMask;
    dotMat->field[y / 2 * 2 + 1] -= actionMask;

    score += 4;
}
void action(dotM* dotMat) {

    unsigned actionMask = 0x8000;
    switch (actionCase) {
    case 2:
        if (dotMat->field[15] & (0x8000 >> expectX - 1)) {
            upNdown *= (-1);
            printf("%d", ((short)expectX - myCurrentPos) * leftNright);
            if ((((short)expectX - myCurrentPos) * leftNright) == -1) {
                leftNright *= (-1);
            }
        }
        else {
            printf("\nGame over\n");
            exit(1);
        }
        break;
    case 3:
        upNdown *= (-1);
        leftNright *= (-1);
        actionBlock(dotMat, ballX - 1, expectY);
        actionBlock(dotMat, expectX - 1, ballY);
        break;
    case 4:
        upNdown *= (-1);
        actionBlock(dotMat, ballX-1, expectY);
        break;
    case 5:
        leftNright *= (-1);
        actionBlock(dotMat, expectX - 1, ballY);
        break;
    case 6:
        upNdown *= (-1);
        leftNright *= (-1);
        actionBlock(dotMat, expectX - 1, expectY);
        break;
    }
}

void ballMove(dotM* dotMat) {

    if (ballX == 1 || ballX == 16) {
        leftNright *= (-1);
    }

    if (actionCase) {
        action(dotMat);
        actionCase = 0;
    }

    if (startPoint) {
        startPoint = 0;
        upNdown *= (-1);
        leftNright = ballX - myCurrentPos;

    }
    unsigned ballMask = 0x8000;
    unsigned UnDMask = 0x8000;
    unsigned LnRMask = 0x8000;
    ballMask >>= ballX - 1;
    dotMat->field[ballY] -= ballMask;

    ballX += leftNright;
    ballY += upNdown;

    expectX = ballX + leftNright;
    expectY = ballY + upNdown;

    if (expectX == 17)
        expectX = 15;
    if (expectX == 0)
        expectX = 2;

    UnDMask >>= ballX - 1;
    LnRMask >>= expectX - 1;

    if (ballY == 0)
        upNdown *= (-1);

    if (ballY == 14)
        actionCase = 2;
    else if (dotMat->field[expectY] & UnDMask && dotMat->field[ballY] & LnRMask)
        actionCase = 3;
    else if (dotMat->field[expectY] & UnDMask)
        actionCase = 4;
    else if (dotMat->field[ballY] & LnRMask)
        actionCase = 5;
    else if (dotMat->field[expectY] & LnRMask)
        actionCase = 6;

    printf("%d", actionCase);
}

int main(void) {
    dotM* dotMatrix;
    dotMatrix = (dotM*)malloc(sizeof(dotM));

    for (int i = 0;i < 16;i++) {
        dotMatrix->field[i] = 0;
    }
    spawnBlocks(dotMatrix);

    int input;
    while (1) {
        system("clear");

        printf("\nmy:%hhd U&D:%hhd L&R:%hhd X:%hhd Y:%hhd eX:%hhd eY:%hhd\n\n", myCurrentPos, upNdown, leftNright, ballX, ballY,expectX, expectY);
        spawnBar(dotMatrix);
        spawnBall(dotMatrix);
        print(dotMatrix);
        if (score == 96) {
            printf("clear!");
            exit(1);
        }
        scanf("%d", &input);
        if (input == 1) {
            moveLeft(dotMatrix);
        }
        else if (input == 3) {
            moveRight(dotMatrix);
        }
        spawnBar(dotMatrix);

        ballMove(dotMatrix);

    }
    return 0;
}


*/