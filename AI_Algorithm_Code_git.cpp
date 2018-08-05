// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI �ڵ� �ۼ� ���]

1. char info[]�� �迭 �ȿ�					"TeamName:�ڽ��� ����,Department:�ڽ��� �Ҽ�"					������ �ۼ��մϴ�.
( ���� ) Teamname:�� Department:�� �� ���� �մϴ�.
"�ڽ��� ����", "�ڽ��� �Ҽ�"�� �����ؾ� �մϴ�.

2. �Ʒ��� myturn() �Լ� �ȿ� �ڽŸ��� AI �ڵ带 �ۼ��մϴ�.

3. AI ������ �׽�Ʈ �Ͻ� ���� "���� �˰����ȸ ��"�� ����մϴ�.

4. ���� �˰��� ��ȸ ���� �����ϱ⿡�� �ٵϵ��� ���� ��, �ڽ��� "����" �� �� �˰����� �߰��Ͽ� �׽�Ʈ �մϴ�.



[���� �� �Լ�]
myturn(int cnt) : �ڽ��� AI �ڵ带 �ۼ��ϴ� ���� �Լ� �Դϴ�.
int cnt (myturn()�Լ��� �Ķ����) : ���� �� �� �־��ϴ��� ���ϴ� ����, cnt�� 1�̸� ���� ���� ��  �� ����  �δ� ��Ȳ(�� ��), cnt�� 2�̸� �� ���� ���� �δ� ��Ȳ(�� ��)
int  x[0], y[0] : �ڽ��� �� ù �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
int  x[1], y[1] : �ڽ��� �� �� �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
void domymove(int x[], int y[], cnt) : �� ������ ��ǥ�� �����ؼ� ���


//int board[BOARD_SIZE][BOARD_SIZE]; �ٵ��� �����Ȳ ��� �־� �ٷλ�� ������. ��, ���������ͷ� ���� �������
// ������ ���� ��ġ�� �ٵϵ��� ������ �ǰ��� ó��.

boolean ifFree(int x, int y) : ���� [x,y]��ǥ�� �ٵϵ��� �ִ��� Ȯ���ϴ� �Լ� (������ true, ������ false)
int showBoard(int x, int y) : [x, y] ��ǥ�� ���� ���� �����ϴ��� �����ִ� �Լ� (1 = �ڽ��� ��, 2 = ����� ��, 3 = ��ŷ)


<-------AI�� �ۼ��Ͻ� ��, ���� �̸��� �Լ� �� ���� ����� �������� �ʽ��ϴ�----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <algorithm>
#include "Connect6Algo.h"
using namespace std;
#define width 19
#define height 19


// "�����ڵ�[C]"  -> �ڽ��� ���� (����)
// "AI�μ�[C]"  -> �ڽ��� �Ҽ� (����)
// ����� ���������� �ݵ�� �������� ����!
char info[] = { "TeamName:������,Department:�Ѿ���б�" };


boolean ifFree(int x, int y);

int board[width][height];

int score[width][height];
Coord scoreList[400] = { 0 };

int showBoard(int x, int y);


typedef struct {
	int x;
	int y;
	int score;
}Coord;


int tempBoard[width][height];
Coord blocking[10]; //blocking ��ǥ



void saveBoard(){ // ���� board�� tempBoard�� ����
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 19; j++)
			tempBoard[i][j] = board[i][j];
	}
}

/**
Score ������ ���� ��ǥ���Լ�
*/
bool compareCoord(const Coord &a, const Coord &b) {
	return a.score > b.score;
}


/**
Score���� ���������� ����(��������)
*/
void sortScore(){
	int count = 0;
	for (int i = 0; i < width; i++){ //score���� ��ǥ�߰�
		for (int j = 0; j < height; j++){
			if (score[i][j] != 0){
				scoreList[count].x = i;
				scoreList[count].y = j;
				scoreList[count].score = score[i][j];
				count++;
			}
		}
	}
	sort(scoreList, scoreList + count, compareCoord);

}





void myturn(int cnt) {

	int x[2], y[2];
	saveBoard();

	if (cnt == 1){
		x[0] = 9;
		y[0] = 9;
	}

	else {




	}

	// �� �κп��� �˰��� ���α׷�(AI)�� �ۼ��Ͻʽÿ�. �⺻ ������ �ڵ带 ���� �Ǵ� �����ϰ� ������ �ڵ带 ����Ͻø� �˴ϴ�.
	// ���� Sample code�� AI�� Random���� ���� ���� Algorithm�� �ۼ��Ǿ� �ֽ��ϴ�.




	// �� �κп��� �ڽ��� ���� ���� ����Ͻʽÿ�.
	// �ʼ� �Լ� : domymove(x�迭,y�迭,�迭ũ��)
	// ���⼭ �迭ũ��(cnt)�� myturn()�� �Ķ���� cnt�� �״�� �־���մϴ�.
	domymove(x, y, cnt);
}



int tempIsFree(int x, int y){
	return x >= 0 && y >= 0 && x < width && y < height && tempBoard[x][y] == 0;
}


/**
*�ӽú��忡 ���� ������
turn 1 : my turn
turn 2 : op turn
*/
void tempMove(int x, int y, int turn){
	if (tempIsFree)
		tempBoard[x][y] = turn;
}



void checkBlocking(){ // ���ŷ���� ��ġ���� , ����ü �迭�� ����
	int count = 0;
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 19; j++){
			if (tempBoard[i][j] == 3){
				blocking[count].x = i;
				blocking[count].y = j;
				count++;
			}
		}
	}
}



void changeBlocking(int n, int turn){ //n��° ���ŷ�� �ش� turn�� ���� ����

	int x = blocking[n].x;
	int y = blocking[n].y;

	tempBoard[x][y] = turn; //turn : 1 or 2
}
