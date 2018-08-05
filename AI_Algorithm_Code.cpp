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
#include "Connect6Algo.h"
#define width 19
#define height 19
#define depth 4			//minimax tree���� �����ϴ� �ܰ��� ��
#define childnum 3		//minimax tree���� ���� �ڽĳ���� ��(����Ž������ ����� ���� ��)

// "�����ڵ�[C]"  -> �ڽ��� ���� (����)
// "AI�μ�[C]"  -> �ڽ��� �Ҽ� (����)
// ����� ���������� �ݵ�� �������� ����!
char info[] = { "TeamName:����,Department:�Ѿ���б�" };

int max_score;	//Minimax���� ����ϴ� �ְ��� �����ϴ� ��������


typedef struct {
	int x;
	int y;
}Coord;


int tempBoard[width][height];
Coord blocking[10];
Coord battleTop[10][2];		//����Ž���� �Ͽ� ���� ���� ������ ���� 2���� ���� ���� ������� �����ϴ� �迭.(cnt�� 1�̸� �� ���� ��)


void myturn(int cnt);
void saveBoard(); // ���� board�� tempBoard�� ����
int tempIsFree(int x, int y);
void tempMyMove(int x, int y, int turn);
void checkBlocking(); // ���ŷ���� ��ġ���� , ����ü �迭�� ����
void changeBlocking(int n, int turn); //n��° ���ŷ�� �ش� turn�� ���� ����
void Minimax(); //Minimax �˰����� ���� �� ���� ��ġ�� ���ϴ� �Լ�. dfs ����� ���� ����. 
				//domymove() �Լ��� ���� �� ������ ��ġ���� ���������� �����.
void battleSearch(int tempboard[]);//Minimax �ȿ����� ����Ž�� �˰��� - battleTop[] �� ���� ���� ������� ����.
int getScore(int bd[]);//board �� tempboard ������ �� �������� ���մϴ�.
void shapeScore();//���� ������ �����ϴ� �Լ�


void myturn(int cnt) {

	int x[2], y[2];

	saveBoard();						//tempboard�� ���� ���� ����.
	// �� �κп��� �˰��� ���α׷�(AI)�� �ۼ��Ͻʽÿ�. �⺻ ������ �ڵ带 ���� �Ǵ� �����ϰ� ������ �ڵ带 ����Ͻø� �˴ϴ�.
	// ���� Sample code�� AI�� Random���� ���� ���� Algorithm�� �ۼ��Ǿ� �ֽ��ϴ�.

	max_score = -10000;
	Minimax(0, 0, 0);



	// �� �κп��� �ڽ��� ���� ���� ����Ͻʽÿ�.
	// �ʼ� �Լ� : domymove(x�迭,y�迭,�迭ũ��)
	// ���⼭ �迭ũ��(cnt)�� myturn()�� �Ķ���� cnt�� �״�� �־���մϴ�.
	domymove(x, y, cnt);
}

// ���� board�� tempBoard�� ����
void saveBoard() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++)
			tempBoard[i][j] = board[i][j];
	}
}

int tempIsFree(int x, int y) {
	return x >= 0 && y >= 0 && x < width && y < height && tempBoard[x][y] == 0;
}

/**
*�ӽú��忡 ���� ������
turn 1 : my turn
turn 2 : op turn
*/
void tempMyMove(int x, int y, int turn) {
	if (tempIsFree)
		tempBoard[x][y] = turn;
}

void deleteTempMove(int x, int y)
{
	tempBoard[x][y] = 0;
}


// ���ŷ���� ��ġ���� , ����ü �迭�� ����
void checkBlocking() {
	int count = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (tempBoard[i][j] == 3) {
				blocking[count].x = i;
				blocking[count].y = j;
				count++;
			}
		}
	}
}

//n��° ���ŷ�� �ش� turn�� ���� ����
void changeBlocking(int n, int turn) {

	int x = blocking[n].x;
	int y = blocking[n].y;

	tempBoard[x][y] = turn; //turn : 1 or 2
}


//current_depth�� ���� � ���̱��� Ʈ���� ����Ǿ������� �ǹ�
//pos_x�� pos_y�� ó�� �� ���� ��ġ�� ����
void Minimax(int current_depth, int pos_x, int pos_x1, int pos_y, int pos_y1)
{
	int tempPos_x;
	int tempPos_y;
	int tempPos_x1;
	int tempPos_y1;

	if (current_depth == depth)		//depth���� ���������� ����
	{
		int score = getScore(tempBoard);
		if (max_score < score)
		{
			max_score = score;
			x[0] = tempPos_x;
			y[0] = tempPos_y;

			if (cnt > 1)
			{
				x[1] = tempPos_x1;
				y[1] = tempPos_y1;
			}
		}
	}

	else
	{
		battleSearch(tempBoard);			//����Ž�� �˰����� ���� battleTop�� �켱���������� �� ���� ��ġ�� �����մϴ�.


		// ó�� �δ� ���� ��ġ�� ����.
		if (current_depth == 0)
		{
			tempPos_x = battleTop[i][0].x;
			tempPos_y = battleTop[i][0].y;

			if (cnt > 1)
			{
				tempPos_x1 = battleTop[i][1].x;
				tempPos_y1 = battleTop[i][1].y;
			}
		}

		else
		{
			tempPos_x = pos_x;
			tempPos_x1 = pos_x1;
			tempPos_y = pos_y;
			tempPos_y1 = pos_y1;
		}

		//���ΰ� Minimax �׸��� �� �����
		for (int i = 0; i < childnum; i++)		//���� �ξ� childnum��ŭ tree�� ��带 �����մϴ�.
			for (int C = 0; C < cnt; C++)		//cnt�� 1�̸� �ѹ� �ΰ�, 2�� �� �� �Ӵϴ�.
			{
				int turn;
				turn = (current_depth + 1) % 2;
				if (turn == 0)
					turn = 2;

				tempMyMove(battleTop[i][C].x, battleTop[i][C].y, turn);		//�ӽ� �ǿ� �� �α�
			}

		Minimax(current_depth + 1, tempPos);


		for (int C = 0; C < cnt; C++)		//cnt�� 1�̸� �ѹ� �ΰ�, 2�� �� �� �Ӵϴ�.
		{
			deleteTempMove(battleTop[i][C].x, battleTop[i][C].y, turn)	//�� �� ����
		}
	}
}


//���� 3���� ������ ���� ������ ã�Ƴ��� �迭 battleTop�� �����մϴ�.
void battleSearch(int tempboard[][])
{




}

//�������� �� ������ �޾ƿ��� �Լ�  - tempBoard�� board�� ����
int getScore(int bd[width][height])
{
	if (bd == tempBoard)
	{

	}

	else if (bd == board)
	{

	}
}

//���� ������ �����ϴ� �Լ�
void shapeScore(void) {

}