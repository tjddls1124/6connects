// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.

3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.

4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.



[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)


<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Connect6Algo.h"
#include <algorithm>

//구조체 선언
typedef struct {
	int x;
	int y;
	int score;
}Coord;


//minimax 관련상수
#define firstSearchNum 7
#define childNum 4
#define depth 3


//Board 관련상수
#define width 19
#define height 19
#define BOARD_SIZE 19

//Minimax 관련 변수
int tempX[2];
int tempY[2];
int tempBoard[width][height];
int max_score = -10000;


//SCore 관련변수
int score[BOARD_SIZE][BOARD_SIZE];
int temp_score[BOARD_SIZE][BOARD_SIZE];
void renewScore(int su);
int isRev = 0;


//Block 관련
int blockCount = 0;
void changeBlock();
Coord blocks[10] = {};


//getScore 관련함수
int getScore();
void valueHorizon();
void valueVerticle();
void valueDiagonal1();
void valueDiagonal2();
void reverse();



//getScore2 관련
int getScore2();
bool isEndPosition = false;
int boardSum = 0;



//평가점수 관련 변수
int my_con[8] = { 0, 0, 2, 4, 10, 10, 1000, -1000 };      //연속된 돌
int my_slp[7] = { 0, 0, 0, 4, 6, 7, 0 };   //한쪽이 막힌 돌
int my_ind[7] = { 0, 0, 0, 4, 6, 7, 0 };   //애매한 돌(한쪽이 막히고 띄어진 돌)
int my_jump[7] = { 0, 0, 0, 5, 7, 8, 0 };   //띈 돌

int op_con[8] = { 0, 0, 0, -9, -900, -900, -1000, 1000 };      //연속된 돌
int op_slp[7] = { 0, 0, 0, 0, -900, -900, 0 };   //한쪽이 막힌 돌
int op_ind[7] = { 0, 0, 0 - 6, -900, -900, 0 };   //애매한 돌(한쪽이 막히고 띄어진 돌)
int op_jump[7] = { 0, 0, 0, -8, -900, -900, 0 };   //띈 돌

//각 점수들의 개수를 저장하는 변수 배열 생성
int conNum[3][8] = { 0 };
int indNum[3][7] = { 0 };
int jumNum[3][7] = { 0 };
int slpNum[3][7] = { 0 };
int valueTurn;



Coord scoreList[361] = { 0 };
Coord battleTop[firstSearchNum * firstSearchNum] = { 0 };

using namespace std;



void opScoreChange(int a, int b, int my_score, int su){
	// 돌 4개의 경우

	if (su == 0 && isRev == 0) { // 첫번째 수, 내돌
		score[a][b] += my_score + 500;
	}
	else if (su == 0 && isRev == 1) // 첫번 째 수, 상대방 돌
		score[a][b] += my_score + 200;
	else if (su == 1 && isRev == 0) //두번째 수, 내돌
		score[a][b] += my_score;
	else if (su == 1 && isRev == 1) //두번째 수, 상대방 돌
		score[a][b] += my_score + 200;
}


void opScoreChange2(int a, int b, int my_score, int su){
	// 돌 5개의 경우

	if (su == 0 && isRev == 0) { // 첫번째 수, 내돌
		score[a][b] += my_score + 500;
	}
	else if (su == 0 && isRev == 1) // 첫번 째 수, 상대방 돌
		score[a][b] += my_score + 200;
	else if (su == 1 && isRev == 0) //두번째 수, 내돌
		score[a][b] += my_score + 500;
	else if (su == 1 && isRev == 1) //두번째 수, 상대방 돌
		score[a][b] += my_score + 200;
}




bool compareCoord(const Coord &a, const Coord &b) {
	return a.score > b.score;
}

void sortScore(){
	int count = 0;
	for (int i = 0; i < width; i++){ //score값과 좌표추가
		for (int j = 0; j < height; j++){
			if (score[i][j] != -1){
				scoreList[count].x = i;
				scoreList[count].y = j;
				scoreList[count].score = score[i][j];
				count++;
			}
		}
	}
	sort(scoreList, scoreList + count, compareCoord);
}

/**
상위 점수를 가진 전장을 탐색합니다.
totalScore : 0 ,count : 0 , x :0 , y :0 , d: 0 을 넣어줍니다.
*/
void battleSearch(Coord* scoreList, int totalScore, int count, int x, int y, int d, int turn)
{

	for (int i = 0; i < firstSearchNum; i++){
		battleTop[count + i].x += 100 * x;
		battleTop[count + i].y += 100 * y;
	}

	for (int i = 0; i < firstSearchNum; i++) {
		if (d == 0){
			x = scoreList[i].x;
			y = scoreList[i].y;
			totalScore = scoreList[i].score;
			tempBoard[x][y] = turn; //돌을 놔보고 갱신
			renewScore(d + 1);
			sortScore();
			battleSearch(scoreList, totalScore, i * firstSearchNum, x, y, 1, turn); //첫 번째 돌인경우 재귀호출
			tempBoard[x][y] = 0;
			renewScore(d);
			sortScore();
		}

		if (d == 1){ //2번째 돌인경우
			battleTop[count + i].x += scoreList[i].x;
			battleTop[count + i].y += scoreList[i].y;
			battleTop[count + i].score = scoreList[i].score + totalScore;
		}
	}
	return;
}

//상위 점수를 가진 전장을 찾아내어 배열 battleTop에 Sort하여 저장합니다.
void warSearch(int turn){

	for (int i = 0; i < firstSearchNum * firstSearchNum; i++){
		battleTop[i].x = 0;
		battleTop[i].y = 0;
		battleTop[i].score = 0;
	}
	renewScore(0);
	sortScore();

	battleSearch(scoreList, 0, 0, 0, 0, 0, turn);
	sort(battleTop, battleTop + firstSearchNum * firstSearchNum, compareCoord);
}

void changeBlock(){// 블록을 무조건 1번으로 처리
	for (int i = 0; i < blockCount; i++){
		tempBoard[blocks[i].x][blocks[i].y] = 1;

	}
}

void saveBoard(){ // 현재 board를 tempBoard에 저장
	blockCount = 0;
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 19; j++){
			if (showBoard(i, j) == 3) {
				//블록리스트에 저장
				blocks[blockCount].x = i;
				blocks[blockCount].y = j;
				blockCount++;
			}
			tempBoard[i][j] = showBoard(i, j);
		}
	}
	//블록을 1로 변환
	changeBlock();
}

int showScore(int x, int y)
{
	return score[x][y];
}

void score_side(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int su)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 64;
	if (tempBoard[a][b] == 3 || tempBoard[c][d] == 3 || tempBoard[e][f] == 3 || tempBoard[g][h] == 3 || tempBoard[i][j] == 3 || tempBoard[k][l] == 3)
		binary = 0;

	switch (binary)
	{
	case (0) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (1) :
		score[a][b] += 1;
		score[c][d] += 1;
		score[e][f] += 1;
		score[g][h] += 1;
		score[i][j] += 1;
		score[k][l] += 0;
		break;
	case (2) :
		score[a][b] += 1;
		score[c][d] += 1;
		score[e][f] += 1;
		score[g][h] += 1;
		score[i][j] += 0;
		score[k][l] += 1;
		break;
	case (3) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (4) :
		score[a][b] += 1;
		score[c][d] += 1;
		score[e][f] += 1;
		score[g][h] += 0;
		score[i][j] += 1;
		score[k][l] += 1;
		break;
	case (5) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (6) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (7) :
		score[a][b] += 5;
		score[c][d] += 5;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (8) :
		score[a][b] += 1;
		score[c][d] += 1;
		score[e][f] += 0;
		score[g][h] += 1;
		score[i][j] += 1;
		score[k][l] += 1;
		break;
	case (9) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (10) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 3;
		break;

	case (11) :
		score[a][b] += 5;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (12) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (13) :
		score[a][b] += 5;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (14) :
		score[a][b] += 5;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (15) :
		opScoreChange(a, b, 30, su);
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (16) :
		score[a][b] += 1;
		score[c][d] += 0;
		score[e][f] += 1;
		score[g][h] += 1;
		score[i][j] += 1;
		score[k][l] += 1;
		break;
	case (17) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (18) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (19) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (20) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (21) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (22) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (23) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (24) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (25) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (26) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (27) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (28) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 5;
		break;
	case (29) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (30) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (31) :
		opScoreChange2(a, b, 500, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (32) :
		score[a][b] += 0;
		score[c][d] += 1;
		score[e][f] += 1;
		score[g][h] += 1;
		score[i][j] += 1;
		score[k][l] += 1;
		break;
	case (33) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (34) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (35) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (36) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (37) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (38) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (39) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (40) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (41) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (42) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (43) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (44) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 5;
		break;
	case (45) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (46) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (47) :
		score[a][b] += 0;
		opScoreChange2(c, d, 500, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (48) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (49) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (50) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (51) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (52) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 5;
		break;
	case (53) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (54) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (55) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange2(e, f, 500, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (56) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 5;
		break;
	case (57) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (58) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (59) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange2(g, h, 500, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (60) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		opScoreChange(k, l, 30, su);
		break;
	case (61) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange2(i, j, 500, su);
		score[k][l] += 0;
		break;
	case (62) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange2(k, l, 500, su);
		break;
	case (63) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		if (isRev) boardSum -= 10000;
		else boardSum += 10000;
		break;
	case (64) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	default:
		break;
	}
}

void score_left(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int su)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 64;

	switch (binary)
	{
	case (0) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (1) :
		score[a][b] += 1;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 0;
		break;
	case (2) :
		score[a][b] += 1;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 0;
		score[k][l] += 2;
		break;
	case (3) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 4;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (4) :
		score[a][b] += 1;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 0;
		score[i][j] += 2;
		score[k][l] += 2;
		break;
	case (5) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 0;
		break;
	case (6) :
		score[a][b] += 3;
		score[c][d] += 4;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 4;
		break;
	case (7) :
		score[a][b] += 5;
		score[c][d] += 6;
		score[e][f] += 7;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (8) :
		score[a][b] += 1;
		score[c][d] += 2;
		score[e][f] += 0;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 2;
		break;
	case (9) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 4;
		score[k][l] += 0;
		break;
	case (10) :
		score[a][b] += 3;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 4;
		break;
	case (11) :
		score[a][b] += 6;
		score[c][d] += 6;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (12) :
		score[a][b] += 3;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 4;
		break;
	case (13) :
		score[a][b] += 5;
		score[c][d] += 6;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 7;
		score[k][l] += 0;
		break;
	case (14) :
		score[a][b] += 5;
		score[c][d] += 7;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 7;
		break;
	case (15) :
		opScoreChange(a, b, 30, su);
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (16) :
		score[a][b] += 1;
		score[c][d] += 0;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 2;
		break;
	case (17) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 4;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (18) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (19) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 6;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (20) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 4;
		break;
	case (21) :
		score[a][b] += 6;
		score[c][d] += 0;
		score[e][f] += 7;
		score[g][h] += 0;
		score[i][j] += 7;
		score[k][l] += 0;
		break;
	case (22) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 7;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 7;
		break;
	case (23) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (24) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 4;
		score[k][l] += 3;
		break;
	case (25) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 7;
		score[k][l] += 0;
		break;
	case (26) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 7;
		break;
	case (27) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (28) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 7;
		score[k][l] += 7;
		break;
	case (29) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (30) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (31) :
		opScoreChange2(a, b, 500, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (32) :
		score[a][b] += 0;
		score[c][d] += 1;
		score[e][f] += 1;
		score[g][h] += 1;
		score[i][j] += 1;
		score[k][l] += 1;
		break;

	case (33) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (34) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (35) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 6;
		score[g][h] += 6;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (36) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (37) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 6;
		score[k][l] += 0;
		break;
	case (38) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 6;
		break;
	case (39) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (40) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (41) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 6;
		score[i][j] += 6;
		score[k][l] += 0;
		break;
	case (42) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 6;
		score[i][j] += 0;
		score[k][l] += 6;
		break;
	case (43) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (44) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 6;
		score[k][l] += 6;
		break;
	case (45) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (46) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (47) :
		score[a][b] += 0;
		opScoreChange2(c, d, 500, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (48) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (49) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (50) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (51) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (52) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 5;
		break;
	case (53) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (54) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (55) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange2(e, f, 500, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (56) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 5;
		break;
	case (57) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (58) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (59) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange2(g, h, 500, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (60) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		opScoreChange(k, l, 30, su);
		break;
	case (61) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange2(i, j, 500, su);
		score[k][l] += 0;
		break;
	case (62) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange2(k, l, 500, su);
		break;
	case (63) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		if (isRev) boardSum -= 10000;
		else boardSum += 10000;
		break;
	case (64) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	default:
		break;
	}
}

void score_right(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int su)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 64;

	switch (binary)
	{
	case (0) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (1) :
		score[a][b] += 1;
		score[c][d] += 1;
		score[e][f] += 1;
		score[g][h] += 1;
		score[i][j] += 1;
		score[k][l] += 0;
		break;
	case (2) :
		score[a][b] += 2;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 0;
		score[k][l] += 1;
		break;
	case (3) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (4) :
		score[a][b] += 2;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 0;
		score[i][j] += 2;
		score[k][l] += 1;
		break;
	case (5) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (6) :
		score[a][b] += 3;
		score[c][d] += 4;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (7) :
		score[a][b] += 5;
		score[c][d] += 5;
		score[e][f] += 5;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (8) :
		score[a][b] += 2;
		score[c][d] += 2;
		score[e][f] += 0;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 1;
		break;
	case (9) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (10) :
		score[a][b] += 4;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (11) :
		score[a][b] += 6;
		score[c][d] += 6;
		score[e][f] += 0;
		score[g][h] += 6;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (12) :
		score[a][b] += 4;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 3;
		break;
	case (13) :
		score[a][b] += 6;
		score[c][d] += 6;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 6;
		score[k][l] += 0;
		break;
	case (14) :
		score[a][b] += 7;
		score[c][d] += 7;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 6;
		break;
	case (15) :
		opScoreChange(a, b, 30, su);
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (16) :
		score[a][b] += 2;
		score[c][d] += 0;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 1;
		break;
	case (17) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (18) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (19) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (20) :
		score[a][b] += 4;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 3;
		break;
	case (21) :
		score[a][b] += 6;
		score[c][d] += 0;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (22) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 7;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (23) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (24) :
		score[a][b] += 4;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 4;
		score[k][l] += 3;
		break;
	case (25) :
		score[a][b] += 6;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (26) :
		score[a][b] += 7;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (27) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (28) :
		score[a][b] += 7;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 7;
		score[k][l] += 6;
		break;
	case (29) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (30) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (31) :
		opScoreChange2(a, b, 500, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (32) :
		score[a][b] += 0;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 1;
		break;
	case (33) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (34) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 4;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (35) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 5;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (36) :
		score[a][b] += 0;
		score[c][d] += 4;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (37) :
		score[a][b] += 0;
		score[c][d] += 6;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (38) :
		score[a][b] += 0;
		score[c][d] += 6;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (39) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (40) :
		score[a][b] += 0;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 4;
		score[k][l] += 3;
		break;
	case (41) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 5;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (42) :
		score[a][b] += 0;
		score[c][d] += 7;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (43) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (44) :
		score[a][b] += 0;
		score[c][d] += 7;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 7;
		score[k][l] += 5;
		break;
	case (45) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (46) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (47) :
		score[a][b] += 0;
		opScoreChange2(c, d, 500, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (48) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 4;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (49) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 6;
		score[g][h] += 6;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (50) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 7;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (51) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (52) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 7;
		score[g][h] += 0;
		score[i][j] += 6;
		score[k][l] += 5;
		break;
	case (53) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (54) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (55) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange2(e, f, 500, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (56) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 7;
		score[k][l] += 5;
		break;
	case (57) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (58) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (59) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange2(g, h, 500, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (60) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		opScoreChange(k, l, 30, su);
		break;
	case (61) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange2(i, j, 500, su);
		score[k][l] += 0;
		break;
	case (62) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange2(k, l, 500, su);
		break;
	case (63) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		if (isRev) boardSum -= 10000;
		else boardSum += 10000;

		break;
	case (64) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	default:
		break;
	}
}

void score_free(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int su)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 0;

	switch (binary)
	{
	case (0) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (1) :
		score[a][b] += 2;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 0;
		break;
	case (2) :
		score[a][b] += 2;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 0;
		score[k][l] += 2;
		break;
	case (3) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 4;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (4) :
		score[a][b] += 2;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 0;
		score[i][j] += 2;
		score[k][l] += 2;
		break;
	case (5) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 0;
		break;
	case (6) :
		score[a][b] += 3;
		score[c][d] += 4;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 4;
		break;
	case (7) :
		score[a][b] += 5;
		score[c][d] += 6;
		score[e][f] += 7;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (8) :
		score[a][b] += 2;
		score[c][d] += 2;
		score[e][f] += 0;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 2;
		break;
	case (9) :
		score[a][b] += 3;
		score[c][d] += 3;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 4;
		score[k][l] += 0;
		break;
	case (10) :
		score[a][b] += 3;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 4;
		break;
	case (11) :
		score[a][b] += 5;
		score[c][d] += 5;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (12) :
		score[a][b] += 4;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 4;
		break;
	case (13) :
		score[a][b] += 5;
		score[c][d] += 6;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 7;
		score[k][l] += 0;
		break;
	case (14) :
		score[a][b] += 6;
		score[c][d] += 7;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 7;
		break;
	case (15) :
		opScoreChange(a, b, 30, su);
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (16) :
		score[a][b] += 2;
		score[c][d] += 0;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 2;
		break;
	case (17) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 3;
		score[g][h] += 4;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (18) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 4;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (19) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 5;
		score[g][h] += 6;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (20) :
		score[a][b] += 3;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 4;
		score[k][l] += 3;
		break;
	case (21) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 6;
		score[k][l] += 0;
		break;
	case (22) :
		score[a][b] += 5;
		score[c][d] += 6;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 6;
		break;
	case (23) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (24) :
		score[a][b] += 4;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 4;
		score[k][l] += 3;
		break;
	case (25) :
		score[a][b] += 5;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 6;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (26) :
		score[a][b] += 6;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (27) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (28) :
		score[a][b] += 7;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 7;
		score[k][l] += 6;
		break;
	case (29) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (30) :
		opScoreChange(a, b, 30, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (31) :
		opScoreChange2(a, b, 500, su);
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (32) :
		score[a][b] += 0;
		score[c][d] += 2;
		score[e][f] += 2;
		score[g][h] += 2;
		score[i][j] += 2;
		score[k][l] += 2;
		break;
	case (33) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 3;
		score[g][h] += 3;
		score[i][j] += 3;
		score[k][l] += 0;
		break;
	case (34) :
		score[a][b] += 0;
		score[c][d] += 3;
		score[e][f] += 4;
		score[g][h] += 3;
		score[i][j] += 0;
		score[k][l] += 3;
		break;
	case (35) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 6;
		score[g][h] += 6;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (36) :
		score[a][b] += 0;
		score[c][d] += 4;
		score[e][f] += 4;
		score[g][h] += 0;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (37) :
		score[a][b] += 0;
		score[c][d] += 6;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 6;
		score[k][l] += 0;
		break;
	case (38) :
		score[a][b] += 0;
		score[c][d] += 5;
		score[e][f] += 6;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 6;
		break;
	case (39) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (40) :
		score[a][b] += 0;
		score[c][d] += 4;
		score[e][f] += 0;
		score[g][h] += 4;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (41) :
		score[a][b] += 0;
		score[c][d] += 6;
		score[e][f] += 0;
		score[g][h] += 6;
		score[i][j] += 6;
		score[k][l] += 0;
		break;
	case (42) :
		score[a][b] += 0;
		score[c][d] += 7;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 0;
		score[k][l] += 6;
		break;
	case (43) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (44) :
		score[a][b] += 0;
		score[c][d] += 7;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 6;
		score[k][l] += 5;
		break;
	case (45) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (46) :
		score[a][b] += 0;
		opScoreChange(c, d, 30, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (47) :
		score[a][b] += 0;
		opScoreChange2(c, d, 500, su);
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (48) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 4;
		score[g][h] += 4;
		score[i][j] += 3;
		score[k][l] += 3;
		break;
	case (49) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 6;
		score[g][h] += 6;
		score[i][j] += 5;
		score[k][l] += 0;
		break;
	case (50) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 7;
		score[g][h] += 5;
		score[i][j] += 0;
		score[k][l] += 5;
		break;
	case (51) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (52) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 7;
		score[g][h] += 0;
		score[i][j] += 5;
		score[k][l] += 6;
		break;
	case (53) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (54) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange(e, f, 30, su);
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (55) :
		score[a][b] += 0;
		score[c][d] += 0;
		opScoreChange2(e, f, 500, su);
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (56) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 7;
		score[i][j] += 6;
		score[k][l] += 5;
		break;
	case (57) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		opScoreChange(i, j, 30, su);
		score[k][l] += 0;
		break;
	case (58) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange(g, h, 30, su);
		score[i][j] += 0;
		opScoreChange(k, l, 30, su);
		break;
	case (59) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		opScoreChange2(g, h, 500, su);
		score[i][j] += 0;
		score[k][l] += 0;
		break;
	case (60) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange(i, j, 30, su);
		opScoreChange(k, l, 30, su);
		break;
	case (61) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		opScoreChange2(i, j, 500, su);
		score[k][l] += 0;
		break;
	case (62) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		opScoreChange2(k, l, 500, su);
		break;
	case (63) :
		score[a][b] += 0;
		score[c][d] += 0;
		score[e][f] += 0;
		score[g][h] += 0;
		score[i][j] += 0;
		score[k][l] += 0;
		if (isRev) boardSum -= 10000;
		else boardSum += 10000;
		break;
	default:
		break;
	}
} // 수정완료 // 수정완료

void horizon(int su)
{
	for (int n = 0; n < height; ++n)
	{
		for (int m = 0; m < width - 5; ++m)
		{
			if (m == 0 && tempBoard[n][m + 6] == 0) {
				score_left(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
			else if (m == 0 && tempBoard[n][m + 6] == 2) {
				score_side(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 0 && tempBoard[n][m + 6] == 0){
				score_free(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 2 && tempBoard[n][m + 6] == 0){
				score_left(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 0 && tempBoard[n][m + 6] == 2){
				score_right(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 2 && tempBoard[n][m + 6] == 2){
				score_side(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
			else if (m == 13 && tempBoard[n][m - 1] == 2){
				score_side(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
			else if (m == 13 && tempBoard[n][m - 1] == 0){
				score_right(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5, su);
			}
		}
	}
}

void verticle(int su)
{
	for (int n = 0; n < height - 5; ++n)
	{
		for (int m = 0; m < width; ++m)
		{
			if (n == 0 && tempBoard[n + 6][m] == 0) {
				score_left(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
			else if (n == 0 && tempBoard[n + 6][m] == 2){
				score_side(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 0 && tempBoard[n + 6][m] == 0){
				score_free(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 2 && tempBoard[n + 6][m] == 0){
				score_left(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 0 && tempBoard[n + 6][m] == 2){
				score_right(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 2 && tempBoard[n + 6][m] == 2){
				score_side(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
			else if (n == 13 && tempBoard[n][m - 1] == 2){
				score_side(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
			else if (n == 13 && tempBoard[n][m - 1] == 0){
				score_right(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m, su);
			}
		}
	}
}

void diagonal1(int su)  // ↘
{
	for (int n = 0; n < height - 5; ++n)
	{
		for (int m = 0; m < width - 5; ++m)
		{
			if ((n == 0 && m == 13) || (n == 13 && m == 0)){
				score_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if ((n == 0 || m == 0) && tempBoard[n + 6][m + 6] == 0) {
				score_left(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if ((n == 0 || m == 0) && tempBoard[n + 6][m + 6] == 2) {
				score_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 0 && tempBoard[n + 6][m + 6] == 0){
				score_free(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 2 && tempBoard[n + 6][m + 6] == 0){
				score_left(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 0 && tempBoard[n + 6][m + 6] == 2){
				score_right(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 2 && tempBoard[n + 6][m + 6] == 2){
				score_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if ((n == 13 || m == 13) && tempBoard[n - 1][m - 1] == 2){
				score_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
			else if ((n == 13 || m == 13) && tempBoard[n - 1][m - 1] == 0){
				score_right(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5, su);
			}
		}
	}
}

void diagonal2(int su) // ↗
{
	for (int n = 5; n < height; ++n)
	{
		for (int m = 0; m < width - 5; ++m)
		{
			if ((n == 5 && m == 0) || (n == 18 && m == 13)){
				score_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (n == 5 && tempBoard[n - 6][m + 6] == 0) {
				score_right(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (n == 5 && tempBoard[n - 6][m + 6] == 2) {
				score_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m == 0 && tempBoard[n - 6][m + 6] == 0) {
				score_left(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m == 0 && tempBoard[n - 6][m + 6] == 2) {
				score_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 0 && tempBoard[n - 6][m + 6] == 0){
				score_free(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 2 && tempBoard[n - 6][m + 6] == 0){
				score_left(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 0 && tempBoard[n - 6][m + 6] == 2){
				score_right(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 2 && tempBoard[n - 6][m + 6] == 2){
				score_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (n == 18 && tempBoard[n - 6][m + 6] == 0){
				score_left(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (n == 18 && tempBoard[n - 6][m + 6] == 2){
				score_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m == 13 && tempBoard[n + 1][m - 1] == 0){
				score_right(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
			else if (m == 13 && tempBoard[n + 1][m - 1] == 2){
				score_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5, su);
			}
		}
	}
}

int tempIsFree(int x, int y) {
	return x >= 0 && y >= 0 && x < width && y < height && tempBoard[x][y] == 0;
}

/**
*임시보드에 돌을 놔보기
turn 1 : my turn
turn 2 : op turn
*/
void tempMyMove(int x, int y, int turn) {
	if (tempIsFree(x, y))
		tempBoard[x][y] = turn;
}

void deleteTempMove(int x, int y)
{
	tempBoard[x][y] = 0;
}

void Minimax(int current_depth, int pos_x1, int pos_x2, int pos_y1, int pos_y2, int cnt)
{
	//블럭이 없는 첫수이면 중앙에 두기
	if (blockCount == 0 && cnt == 1)
	{
		tempX[0] = 9;
		tempY[0] = 9;
		tempX[1] = 0;
		tempY[1] = 0;
		return;
	}

	//초기화 - 두는 위치 임시 저장할 변수
	int tempPos_x1 = 0;
	int tempPos_y1 = 0;
	int tempPos_x2 = 0;
	int tempPos_y2 = 0;

	if (current_depth == depth)		//depth까지 내려갔으면 종료
	{
		int totalScore = getScore2();				//renewScore에서 totalScore변경하게 확인.

		if (max_score < totalScore)				//더 점수가 높으면
		{
			max_score = totalScore;
			tempX[0] = pos_x1;
			tempY[0] = pos_y1;

			if (cnt > 1)
			{
				tempX[1] = pos_x2;
				tempY[1] = pos_y2;
			}
		}
		return;
	}

	else
	{
		//현재 돌을 놓고 Minimax로 들어온 상태		
		if (getScore2() > 5000)		//내 돌이 6개가 있으면
		{							//처음 둔 돌 저장 , Minimax 완전히 종료
			tempX[0] = pos_x1;
			tempY[0] = pos_y1;
			tempX[1] = pos_x2;
			tempY[1] = pos_y2;
			max_score = 100000;
			return;
		}

		//6목을 내가 만들었으면 더 탐색하지 않고 종료
		if (max_score == 100000)
			return;

		if (getScore2() < -5000)		//상대 돌이 6개가 있으면
		{
			return;						//이번 Minimax만 종료
		}

		int turn = (current_depth + 1) % 2;		//depth에 따라 turn이 달라짐.

		if (turn == 0)
			turn = 2;

		if (turn == 2)			//상대 턴이면 반전시켜서 전장탐색 실행
			reverse();			//반전

		warSearch(turn);		//전장탐색

		if (turn == 2)
			reverse();			//반전 돌려놓기

		//돌두고 Minimax 그리고 돌 지우기
		for (int i = 0; i < childNum; i++)		//돌을 두어 childnum만큼 tree의 노드를 생성합니다.
		{
			if (turn == 2)			//상대 턴이면 반전시켜서 전장탐색 실행
				reverse();			//반전

			warSearch(turn);		//전장탐색

			if (turn == 2)
				reverse();			//반전 돌려놓기

			//6목을 내가 만들었으면 더 탐색하지 않고 종료
			if (max_score == 100000)
				return;

			// 처음 두는 돌의 위치를 저장.
			if (current_depth == 0)
			{
				pos_x1 = battleTop[i].x / 100;
				pos_y1 = battleTop[i].y / 100;

				if (cnt > 1)
				{
					pos_x2 = battleTop[i].x % 100;
					pos_y2 = battleTop[i].y % 100;
				}
			}

			tempPos_x1 = battleTop[i].x / 100;
			tempPos_y1 = battleTop[i].y / 100;
			tempPos_x2 = battleTop[i].x % 100;
			tempPos_y2 = battleTop[i].y % 100;


			for (int C = 1; C <= cnt; C++)		//cnt가 1이면 한번 두고, 2면 두 번 둡니다.
			{
				if (C == 1)
					tempMyMove(tempPos_x1, tempPos_y1, turn);		//첫 번째 돌 두기

				if (C == 2)
					tempMyMove(tempPos_x2, tempPos_y2, turn);		//두 번째 돌 두기
			}

			//	print_bas();

			Minimax(current_depth + 1, pos_x1, pos_x2, pos_y1, pos_y2, cnt);

			for (int C = 1; C <= cnt; C++)		//cnt가 1이면 한번 두고, 2면 두 번 둡니다.
			{
				if (C == 1)
					deleteTempMove(tempPos_x1, tempPos_y1);		//첫 번째 돌 지우기

				if (C == 2)
					deleteTempMove(tempPos_x2, tempPos_y2);		//두 번째 돌 지우기
			}

			//	print_bas();
		}
	}
}

//모양에 따른 모양을 가져옵니다.
void value_side(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 64;
	if (tempBoard[a][b] == 3 || tempBoard[c][d] == 3 || tempBoard[e][f] == 3 || tempBoard[g][h] == 3 || tempBoard[i][j] == 3 || tempBoard[k][l] == 3)
		binary = 0;

	switch (binary)
	{

	case 6: conNum[valueTurn][2]++;
		break;
	case 7: indNum[valueTurn][3]++;
		break;
	case 12: conNum[valueTurn][2]++;
		break;
	case 13: conNum[valueTurn][3]++;
		break;
	case 14: indNum[valueTurn][3]++;
		break;
	case 15: slpNum[valueTurn][4]++;
		break;
	case 19: indNum[valueTurn][3]++;
		break;
	case 21: indNum[valueTurn][3]++;
		break;
	case 22: indNum[valueTurn][3]++;
		break;
	case 23: indNum[valueTurn][4]++;
		break;
	case 24: conNum[valueTurn][2]++;
		break;
	case 25: indNum[valueTurn][3]++;
		break;
	case 26: indNum[valueTurn][3]++;
		break;
	case 27: indNum[valueTurn][4]++;
		break;
	case 28: indNum[valueTurn][3]++;
		break;
	case 29: indNum[valueTurn][4]++;
		break;
	case 30: indNum[valueTurn][4]++;
		break;
	case 31: slpNum[valueTurn][5]++;
		break;
	case 35: indNum[valueTurn][3]++;
		break;
	case 37: indNum[valueTurn][3]++;
		break;
	case 38: indNum[valueTurn][3]++;
		break;
	case 39: indNum[valueTurn][4]++;
		break;
	case 41: indNum[valueTurn][3]++;
		break;
	case 42: indNum[valueTurn][3]++;
		break;
	case 43: indNum[valueTurn][4]++;
		break;
	case 44: indNum[valueTurn][3]++;
		break;
	case 45: indNum[valueTurn][4]++;
		break;
	case 46: indNum[valueTurn][4]++;
		break;
	case 47: indNum[valueTurn][5]++;
		break;
	case 49: indNum[valueTurn][3]++;
		break;
	case 50: indNum[valueTurn][3]++;
		break;
	case 51: indNum[valueTurn][4]++;
		break;
	case 52: indNum[valueTurn][3]++;
		break;
	case 53: indNum[valueTurn][4]++;
		break;
	case 54: indNum[valueTurn][4]++;
		break;
	case 55: indNum[valueTurn][5]++;
		break;
	case 56: indNum[valueTurn][3]++;
		break;
	case 57: indNum[valueTurn][4]++;
		break;
	case 58: indNum[valueTurn][4]++;
		break;
	case 59: indNum[valueTurn][5]++;
		break;
	case 60: indNum[valueTurn][4]++;
		break;
	case 61: indNum[valueTurn][5]++;
		break;
	case 62: slpNum[valueTurn][5]++;
		break;
	case 63: conNum[valueTurn][6]++;
		break;
	default:
		break;
	}
}

void value_left(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 64;

	switch (binary)
	{

	case 3: conNum[valueTurn][2]++;
		break;
	case 7: indNum[valueTurn][3]++;
		break;
	case 11: indNum[valueTurn][3]++;
		break;
	case 12: conNum[valueTurn][2]++;
		break;
	case 13: indNum[valueTurn][3]++;
		break;
	case 14: indNum[valueTurn][3]++;
		break;
	case 15: indNum[valueTurn][4]++;
		break;
	case 19: indNum[valueTurn][3]++;
		break;
	case 21: indNum[valueTurn][3]++;
		break;
	case 22: indNum[valueTurn][3]++;
		break;
	case 23: indNum[valueTurn][4]++;
		break;
	case 24: conNum[valueTurn][2]++;
		break;
	case 25: indNum[valueTurn][3]++;
		break;
	case 26: indNum[valueTurn][3]++;
		break;
	case 27: indNum[valueTurn][4]++;
		break;
	case 28: indNum[valueTurn][3]++;
		break;
	case 29: indNum[valueTurn][4]++;
		break;
	case 30: indNum[valueTurn][4]++;
		break;
	case 31: indNum[valueTurn][5]++;
		break;
	case 35: indNum[valueTurn][3]++;
		break;
	case 37: indNum[valueTurn][3]++;
		break;
	case 38: indNum[valueTurn][3]++;
		break;
	case 39: indNum[valueTurn][4]++;
		break;
	case 41: indNum[valueTurn][3]++;
		break;
	case 42: indNum[valueTurn][3]++;
		break;
	case 43: indNum[valueTurn][4]++;
		break;
	case 44: indNum[valueTurn][3]++;
		break;
	case 45: indNum[valueTurn][4]++;
		break;
	case 46: indNum[valueTurn][4]++;
		break;
	case 47: indNum[valueTurn][5]++;
		break;
	case 49: indNum[valueTurn][3]++;
		break;
	case 50: indNum[valueTurn][3]++;
		break;
	case 51: indNum[valueTurn][4]++;
		break;
	case 52: indNum[valueTurn][3]++;
		break;
	case 53: indNum[valueTurn][3]++;
		break;
	case 54: indNum[valueTurn][4]++;
		break;
	case 55: indNum[valueTurn][5]++;
		break;
	case 56: slpNum[valueTurn][3]++;
		break;
	case 57: indNum[valueTurn][4]++;
		break;
	case 58: indNum[valueTurn][4]++;
		break;
	case 59: indNum[valueTurn][5]++;
		break;
	case 60: slpNum[valueTurn][4]++;
		break;
	case 61: indNum[valueTurn][5]++;
		break;
	case 62: slpNum[valueTurn][5]++;
		break;
	case 63: conNum[valueTurn][6]++;
		break;
	default:
		break;
	}
}

void value_right(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 64;

	switch (binary)
	{
	case 6: conNum[valueTurn][2]++;
		break;
	case 7: slpNum[valueTurn][3]++;
		break;
	case 11: indNum[valueTurn][3]++;
		break;
	case 12: conNum[valueTurn][2]++;
		break;
	case 13: indNum[valueTurn][3]++;
		break;
	case 14: indNum[valueTurn][3]++;
		break;
	case 15: slpNum[valueTurn][4]++;
		break;
	case 19: indNum[valueTurn][3]++;
		break;
	case 21: indNum[valueTurn][3]++;
		break;
	case 22: indNum[valueTurn][3]++;
		break;
	case 23: indNum[valueTurn][4]++;
		break;
	case 24: conNum[valueTurn][2]++;
		break;
	case 25: indNum[valueTurn][3]++;
		break;
	case 26: indNum[valueTurn][3]++;
		break;
	case 27: indNum[valueTurn][4]++;
		break;
	case 28: indNum[valueTurn][3]++;
		break;
	case 29: indNum[valueTurn][4]++;
		break;
	case 30: indNum[valueTurn][4]++;
		break;
	case 31: slpNum[valueTurn][5]++;
		break;
	case 35: indNum[valueTurn][4]++;
		break;
	case 37: indNum[valueTurn][3]++;
		break;
	case 38: indNum[valueTurn][3]++;
		break;
	case 39: indNum[valueTurn][4]++;
		break;
	case 41: indNum[valueTurn][3]++;
		break;
	case 42: indNum[valueTurn][3]++;
		break;
	case 43: indNum[valueTurn][4]++;
		break;
	case 44: indNum[valueTurn][3]++;
		break;
	case 45: indNum[valueTurn][4]++;
		break;
	case 46: indNum[valueTurn][4]++;
		break;
	case 47: indNum[valueTurn][5]++;
		break;
	case 48: conNum[valueTurn][2]++;
		break;
	case 49: indNum[valueTurn][3]++;
		break;
	case 50: indNum[valueTurn][3]++;
		break;
	case 51: indNum[valueTurn][4]++;
		break;
	case 52: indNum[valueTurn][3]++;
		break;
	case 53: indNum[valueTurn][4]++;
		break;
	case 54: indNum[valueTurn][4]++;
		break;
	case 55: indNum[valueTurn][5]++;
		break;
	case 56: indNum[valueTurn][3]++;
		break;
	case 57: indNum[valueTurn][4]++;
		break;
	case 58: indNum[valueTurn][4]++;
		break;
	case 59: indNum[valueTurn][5]++;
		break;
	case 60: indNum[valueTurn][4]++;
		break;
	case 61: indNum[valueTurn][5]++;
		break;
	case 62: indNum[valueTurn][5]++;
		break;
	case 63: conNum[valueTurn][6]++;
		break;
	default:
		break;
	}
}

void value_free(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l)
{
	int binary =
		2 * 2 * 2 * 2 * 2 * tempBoard[a][b] +
		2 * 2 * 2 * 2 * tempBoard[c][d] +
		2 * 2 * 2 * tempBoard[e][f] +
		2 * 2 * tempBoard[g][h] +
		2 * tempBoard[i][j] +
		tempBoard[k][l];

	if (tempBoard[a][b] == 2 || tempBoard[c][d] == 2 || tempBoard[e][f] == 2 || tempBoard[g][h] == 2 || tempBoard[i][j] == 2 || tempBoard[k][l] == 2)
		binary = 0;

	switch (binary)
	{
	case 3: conNum[valueTurn][2]++;
		break;

	case 6: conNum[valueTurn][2]++;
		break;

	case 7: conNum[valueTurn][3]++;
		break;

	case 11: jumNum[valueTurn][3]++;
		break;

	case 12: conNum[valueTurn][2]++;
		break;

	case 13: jumNum[valueTurn][3]++;
		break;

	case 14: conNum[valueTurn][3]++;
		break;

	case 15: conNum[valueTurn][4]++;
		break;

	case 19: jumNum[valueTurn][3]++;
		break;

	case 21: jumNum[valueTurn][3]++;
		break;

	case 22: jumNum[valueTurn][3]++;
		break;

	case 23: jumNum[valueTurn][4]++;
		break;

	case 24: conNum[valueTurn][2]++;
		break;

	case 25: jumNum[valueTurn][3]++;
		break;

	case 26: jumNum[valueTurn][3]++;
		break;

	case 27: jumNum[valueTurn][4]++;
		break;

	case 28: conNum[valueTurn][3]++;
		break;

	case 29: jumNum[valueTurn][4]++;
		break;

	case 30: conNum[valueTurn][4]++;
		break;

	case 31: conNum[valueTurn][5]++;
		break;

	case 35: jumNum[valueTurn][3]++;
		break;

	case 37: jumNum[valueTurn][3]++;
		break;

	case 38: jumNum[valueTurn][3]++;
		break;

	case 39: jumNum[valueTurn][3]++;
		break;

	case 41: jumNum[valueTurn][3]++;
		break;

	case 42: jumNum[valueTurn][3]++;
		break;

	case 43: jumNum[valueTurn][4]++;
		break;
	case 44: jumNum[valueTurn][4]++;
		break;
	case 45: jumNum[valueTurn][4]++;
		break;
	case 46: jumNum[valueTurn][4]++;
		break;
	case 47: jumNum[valueTurn][5]++;
		break;
	case 48: conNum[valueTurn][2]++;
		break;
	case 49: jumNum[valueTurn][3]++;
		break;
	case 50: jumNum[valueTurn][3]++;
		break;
	case 51: jumNum[valueTurn][4]++;
		break;
	case 52: jumNum[valueTurn][3]++;
		break;
	case 53: jumNum[valueTurn][4]++;
		break;
	case 54: jumNum[valueTurn][4]++;
		break;
	case 55: jumNum[valueTurn][5]++;
		break;
	case 56: conNum[valueTurn][3]++;
		break;
	case 57: jumNum[valueTurn][4]++;
		break;
	case 58: jumNum[valueTurn][4]++;
		break;
	case 59: jumNum[valueTurn][5]++;

	case 60: conNum[valueTurn][4]++;
		break;
	case 61: jumNum[valueTurn][5]++;
		break;
	case 62: conNum[valueTurn][5]++;
		break;
	case 63: conNum[valueTurn][6]++;
		break;
	default:
		break;
	}
} // 수정완료 // 수정완료

void valueHorizon()
{
	for (int n = 0; n < height; ++n)
	{
		for (int m = 0; m < width - 5; ++m)
		{
			if (m == 0 && tempBoard[n][m + 6] == 0) {
				value_left(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
			else if (m == 0 && tempBoard[n][m + 6] == 2) {
				value_side(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 0 && tempBoard[n][m + 6] == 0){
				value_free(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 2 && tempBoard[n][m + 6] == 0){
				value_left(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 0 && tempBoard[n][m + 6] == 2){
				value_right(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
			else if (m > 0 && m < 13 && tempBoard[n][m - 1] == 2 && tempBoard[n][m + 6] == 2){
				value_side(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
			else if (m == 13 && tempBoard[n][m - 1] == 2){
				value_side(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
			else if (m == 13 && tempBoard[n][m - 1] == 0){
				value_right(n, m, n, m + 1, n, m + 2, n, m + 3, n, m + 4, n, m + 5);
			}
		}
	}
}

void valueVerticle()
{
	for (int n = 0; n < height - 5; ++n)
	{
		for (int m = 0; m < width; ++m)
		{
			if (n == 0 && tempBoard[n + 6][m] == 0) {
				value_left(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
			else if (n == 0 && tempBoard[n + 6][m] == 2){
				value_side(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 0 && tempBoard[n + 6][m] == 0){
				value_free(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 2 && tempBoard[n + 6][m] == 0){
				value_left(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 0 && tempBoard[n + 6][m] == 2){
				value_right(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
			else if (n > 0 && n < 13 && tempBoard[n - 1][m] == 2 && tempBoard[n + 6][m] == 2){
				value_side(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
			else if (n == 13 && tempBoard[n][m - 1] == 2){
				value_side(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
			else if (n == 13 && tempBoard[n][m - 1] == 0){
				value_right(n, m, n + 1, m, n + 2, m, n + 3, m, n + 4, m, n + 5, m);
			}
		}
	}
}

void valueDiagonal1()  // ↘
{
	for (int n = 0; n < height - 5; ++n)
	{
		for (int m = 0; m < width - 5; ++m)
		{
			if ((n == 0 && m == 13) || (n == 13 && m == 0)){
				value_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if ((n == 0 || m == 0) && tempBoard[n + 6][m + 6] == 0) {
				value_left(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if ((n == 0 || m == 0) && tempBoard[n + 6][m + 6] == 2) {
				value_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 0 && tempBoard[n + 6][m + 6] == 0){
				value_free(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 2 && tempBoard[n + 6][m + 6] == 0){
				value_left(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 0 && tempBoard[n + 6][m + 6] == 2){
				value_right(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 13 && tempBoard[n - 1][m - 1] == 2 && tempBoard[n + 6][m + 6] == 2){
				value_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if ((n == 13 || m == 13) && tempBoard[n - 1][m - 1] == 2){
				value_side(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
			else if ((n == 13 || m == 13) && tempBoard[n - 1][m - 1] == 0){
				value_right(n, m, n + 1, m + 1, n + 2, m + 2, n + 3, m + 3, n + 4, m + 4, n + 5, m + 5);
			}
		}
	}
}

void valueDiagonal2() // ↗
{
	for (int n = 5; n < height; ++n)
	{
		for (int m = 0; m < width - 5; ++m)
		{
			if ((n == 5 && m == 0) || (n == 18 && m == 13)){
				value_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (n == 5 && tempBoard[n - 6][m + 6] == 0) {
				value_right(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (n == 5 && tempBoard[n - 6][m + 6] == 2) {
				value_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m == 0 && tempBoard[n - 6][m + 6] == 0) {
				value_left(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m == 0 && tempBoard[n - 6][m + 6] == 2) {
				value_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 0 && tempBoard[n - 6][m + 6] == 0){
				value_free(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 2 && tempBoard[n - 6][m + 6] == 0){
				value_left(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 0 && tempBoard[n - 6][m + 6] == 2){
				value_right(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m > 0 && n > 0 && m < 13 && n < 19 && tempBoard[n + 1][m - 1] == 2 && tempBoard[n - 6][m + 6] == 2){
				value_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (n == 18 && tempBoard[n - 6][m + 6] == 0){
				value_left(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (n == 18 && tempBoard[n - 6][m + 6] == 2){
				value_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m == 13 && tempBoard[n + 1][m - 1] == 0){
				value_right(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
			else if (m == 13 && tempBoard[n + 1][m - 1] == 2){
				value_side(n, m, n - 1, m + 1, n - 2, m + 2, n - 3, m + 3, n - 4, m + 4, n - 5, m + 5);
			}
		}
	}
}

void init_Score(){
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			score[i][j] = 0;
		}
	}
}

void init_tmpScore(){
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			temp_score[i][j] = 0;
		}
	}
}

void cal(int su){
	verticle(su);
	horizon(su);
	diagonal1(su);
	diagonal2(su);
}


void tmp_score(){
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			temp_score[i][j] = score[i][j];
		}
	}
}

int getScore2(){
	int su = 0;
	boardSum = 0;
	init_Score();
	init_tmpScore();
	cal(su);
	tmp_score();
	reverse();
	init_Score();
	cal(su);

	for (int i = 0; i < width; i++){ //반전시킨 점수를 뺀 뒤,
		for (int j = 0; j < height; j++){
			score[i][j] -= temp_score[i][j];
		}
	}


	for (int i = 0; i < width; i++){ // 보드판의 모든 스코어를 총합
		for (int j = 0; j < height; j++){
			boardSum += score[i][j];
		}
	}
	reverse();

	//초기화
	init_Score();
	init_tmpScore();

	return boardSum;
}

int getScore() //현재판의 평가점수를 리턴합니다.
{
	int value = 0;
	int my_value = 0;
	int op_value = 0;


	//1번은 my, 2번은 op의 점수

	//돌의 모양에 따른 평가 점수를 생성합니다.

	//점수 개수 초기화
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			conNum[i][j] = 0;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 7; j++)
			indNum[i][j] = 0;


	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 7; j++)
			slpNum[i][j] = 0;


	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 7; j++)
			jumNum[i][j] = 0;



	//반전 이전이므로, 1번, 즉, 자신의 옵션으로 점수 생성
	valueTurn = 1;

	valueHorizon();

	valueVerticle();

	valueDiagonal1();

	valueDiagonal2();

	for (int i = 0; i < 8; i++)
		my_value += my_con[i] * conNum[1][i];		//점수 * 개수
	for (int i = 0; i < 7; i++)
		my_value += my_ind[i] * indNum[1][i];		//점수 * 개수
	for (int i = 0; i < 7; i++)
		my_value += my_jump[i] * jumNum[1][i];		//점수 * 개수
	for (int i = 0; i < 7; i++)
		my_value += my_slp[i] * slpNum[1][i];		//점수 * 개수

	reverse();		//최초 반전

	//반전 이후이므로 2번, 즉, 상대방 옵션으로 점수 생성	
	valueTurn = 2;

	valueHorizon();

	valueVerticle();

	valueDiagonal1();

	valueDiagonal2();

	reverse();	//반전 원상복귀


	for (int i = 0; i < 8; i++)
		op_value += op_con[i] * conNum[2][i];		//점수 * 개수
	for (int i = 0; i < 7; i++)
		op_value += op_ind[i] * indNum[2][i];		//점수 * 개수
	for (int i = 0; i < 7; i++)
		op_value += op_jump[i] * jumNum[2][i];		//점수 * 개수
	for (int i = 0; i < 7; i++)
		op_value += op_slp[i] * slpNum[2][i];		//점수 * 개수

	value = my_value + op_value;

	return value;

}


void reverse(){
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			if (tempBoard[i][j] == 1){
				tempBoard[i][j] = 2;
			}
			else if (tempBoard[i][j] == 2){
				tempBoard[i][j] = 1;
			}
		}
	}
	changeBlock();// block처리하도록 변경
	if (isRev == 0) isRev = 1;
	else isRev = 0;
}
void renewScore(int su){
	init_Score();
	init_tmpScore();
	cal(su);
	tmp_score();
	reverse();
	init_Score();
	cal(su);
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			score[i][j] += temp_score[i][j];
		}
	}
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			if (tempBoard[i][j] != 0){
				score[i][j] = -1;
			}
		}
	}
	reverse();
}

void minmin(int cnt)
{
	saveBoard();
	int x[2], y[2];

	max_score = -10000;
	Minimax(0, 0, 0, 0, 0, cnt);
	for (int i = 0; i < 2; i++){
		x[i] = tempX[i];
		y[i] = tempY[i];
	}

	domymove(x, y, cnt);
}

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:종웅성 level4,Department:AI부서[C]" };

void myturn(int cnt) {
	int x[2], y[2];



	minmin(cnt);


}
