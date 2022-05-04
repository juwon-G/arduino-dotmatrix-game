#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/*int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

int getch(void) 
{
	int ch;
	struct termios buf, save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;

}
*/
typedef struct dotMatrix {
	unsigned short field[16];
}dotM;


unsigned char myCurrentPos = 8;
float gamespeed = 1;


void print(dotM *dotMat) {
	for (int y = 0; y < 16; y++) { //도트 매트릭스 출력
		for (int x = 15; x >= 0;x--) {
			int result = dotMat->field[y] >> x & 1;
			printf("%d", result);
		}
		printf("\n");
	}
	printf("\n\n");
}


void moveLeft(dotM* dotMat) {
	if (myCurrentPos == 1)
		return;
	myCurrentPos--;
}

void moveRight(dotM* dotMat) {
	if (myCurrentPos == 16)
		return;
	myCurrentPos++;
}


void spawnBall(dotM *dotMat) {
	srand((unsigned int)time(NULL));
	unsigned short ballMask = 0x8000;
	unsigned char dotX = rand() % 16;
	ballMask >>= dotX;
	dotMat -> field[0] |= ballMask;
}


void spawnDot(dotM *dotMat) {
	dotMat->field[15] = 0;
	unsigned short myMask = 0x8000;
	myMask >>= myCurrentPos -1;
	dotMat->field[15] |= myMask;
}


void dotfall(dotM *dotmat) {
	unsigned short mask;
	for (int t = 13; t>=0; t--) {
		mask = 0;
		mask |= dotmat->field[t];
		dotmat->field[t] = 0;
		dotmat->field[t+1] |= mask;
	}
}

int main(void) {
	dotM *dotMatrix;
	dotMatrix = (dotM*)malloc(sizeof(dotM));

	for (int i = 0;i < 16;i++) {
		dotMatrix->field[i] = 0;
	}
	int input;

	while(1) {
		system("clear");
		spawnBall(dotMatrix);
		spawnDot(dotMatrix);
		dotfall(dotMatrix);
		print(dotMatrix);
		if (dotMatrix->field[14] == dotMatrix->field[15]) {
			printf("\nGame over\n");
			exit(1);
		}
		dotMatrix->field[14] = 0;
		scanf("%d", &input);
		if (input == 1) {
			moveLeft(dotMatrix);
		}
		else if (input == 3) {
			moveRight(dotMatrix);
		}
	//	sleep(gamespeed);
	}
	return 0;
}
