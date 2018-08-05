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

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <algorithm>
#include "Connect6Algo.h"

using namespace std;

#define width 19
#define height 19
#define depth 4			//minimax tree에서 진행하는 단계의 수
#define childnum 3		//minimax tree에서 만들 자식노드의 수(전장탐색에서 고려할 돌의 수)
#define firstSearchNum 7 // 2개의 최적 조합을 찾기위한 탐색개수


// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:종웅성,Department:한양대학교" };

int max_score;	//Minimax에서 사용하는 최고점 저장하는 전역변수
int showBoard(int x, int y);


typedef struct {
	int x;
	int y;
	int score;
}Coord;


int tempBoard[width][height];
Coord blocking[10];
Coord battleTop[10] = { 0 };		//전장탐색을 하여 가장 좋은 점수가 나온 2개의 돌을 높은 순서대로 저장하는 배열.(cnt가 1이면 한 개의 돌)


void myturn(int cnt);
void saveBoard(); // 현재 board를 tempBoard에 저장
int tempIsFree(int x, int y);
void tempMyMove(int x, int y, int turn);
void checkBlocking(); // 블로킹들의 위치저장 , 구조체 배열에 저장
void changeBlocking(int n, int turn); //n번째 블로킹을 해당 turn의 돌로 변경
void Minimax(int a, int b, int c); //Minimax 알고리즘을 통해 둘 돌의 위치를 정하는 함수. dfs 방식을 따를 예정. 
//domymove() 함수를 통해 둘 돌들의 위치까지 최종적으로 출력함.
void battleSearch(int tempboard[19][19]);//Minimax 안에서의 전장탐색 알고리즘 - battleTop[] 에 점수 높은 순서대로 저장.
int getScore(int bd[]);//board 와 tempboard 에서의 총 점수값을 구합니다.
void shapeScore();//판의 점수를 관리하는 함수
bool ifFree(int x, int y);
void renewScore();
void domymove();
void warSearch(int turn);


int board[width][height];
int score[width][height];
Coord scoreList[400] = { 0 };

void myturn(int cnt) {

	int x[2], y[2];

	saveBoard();						//tempboard에 현재 판을 저장.
	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

	max_score = -10000;
	Minimax(0, 0, 0);



	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	domymove(x, y, cnt);
}

// 현재 board를 tempBoard에 저장
void saveBoard() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++)
			tempBoard[i][j] = board[i][j];
	}
}

/**
Score 정렬을 위한 좌표비교함수
*/
bool compareCoord(const Coord &a, const Coord &b) {
	return a.score > b.score;
}

/**
Score값이 높은순으로 정렬(내림차순)
*/
void sortScore() {
	int count = 0;
	for (int i = 0; i < width; i++) { //score값과 좌표추가
		for (int j = 0; j < height; j++) {
			if (score[i][j] != 0) {
				scoreList[count].x = i;
				scoreList[count].y = j;
				scoreList[count].score = score[i][j];
				count++;
			}
		}
	}
	sort(scoreList, scoreList + count, compareCoord);


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


// 블로킹들의 위치저장 , 구조체 배열에 저장
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

//n번째 블로킹을 해당 turn의 돌로 변경
void changeBlocking(int n, int turn) {

	int x = blocking[n].x;
	int y = blocking[n].y;

	tempBoard[x][y] = turn; //turn : 1 or 2
}


//current_depth는 현재 어떤 깊이까지 트리가 진행되었는지를 의미
//pos_x와 pos_y는 처음 둘 돌의 위치를 저장
void Minimax(int current_depth, int pos_x1, int pos_x2, int pos_y1, int pos_y2, int cnt)
{
	int tempPos_x1;
	int tempPos_y1;
	int tempPos_x2;
	int tempPos_y2;

	if (current_depth == depth)		//depth까지 내려갔으면 종료
	{
		int score = getScore(*tempBoard); //tempBoard -> *tempBoard로 수정 , 오류수정부탁
		if (max_score < score)
		{
			max_score = score;
			pos_x1 = tempPos_x1;
			pos_y1 = tempPos_y1;

			if (cnt > 1)
			{
				pos_x2 = tempPos_x2;
				pos_y2 = tempPos_y2;
			}
		}
	}

	else
	{
		warSearch(1);			//전장탐색 알고리즘을 통해 battleTop에 우선순위순서로 둘 돌의 위치를 저장합니다.

		//돌두고 Minimax 그리고 돌 지우기
		for (int i = 0; i < childnum; i++)		//돌을 두어 childnum만큼 tree의 노드를 생성합니다.
		{
			// 처음 두는 돌의 위치를 저장.
			if (current_depth == 0)
			{
				tempPos_x1 = battleTop[i].x / 100;
				tempPos_y1 = battleTop[i].y / 100;

				if (cnt > 1)
				{
					tempPos_x1 = battleTop[i].x % 100;
					tempPos_y1 = battleTop[i].y % 100;
				}
			}

			else
			{
				tempPos_x1 = pos_x1;
				tempPos_x2 = pos_x2;
				tempPos_y1 = pos_y1;
				tempPos_y2 = pos_y2;
			}


			// ? cnt가 1이면 for 작동안함

			for (int C = 0; C < cnt; C++)		//cnt가 1이면 한번 두고, 2면 두 번 둡니다.
			{
				int turn;
				turn = (current_depth + 1) % 2;		//depth에 따라 turn이 달라짐.
				if (turn == 0)
					turn = 2;

				tempMyMove(battleTop[i][C].x, battleTop[i][C].y, turn);		//임시 판에 돌 두기
			}

			Minimax(current_depth + 1, tempPos_x1, tempPos_x2, tempPos_y1, tempPos_y2, cnt);


			for (int C = 0; C < cnt; C++)		//cnt가 1이면 한번 두고, 2면 두 번 둡니다.
			{
				deleteTempMove(battleTop[i][C].x, battleTop[i][C].y);	//둔 돌 삭제
			}
		}
	}
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
			board[x][y] = turn; //돌을 놔보고 갱신
			renewScore();
			sortScore();
			battleSearch(scoreList, totalScore, i * firstSearchNum, x, y, 1, turn); //첫 번째 돌인경우 재귀호출
			board[x][y] = 0;
			renewScore();
			sortScore();
		}

		if (d == 1){ //2번째 돌인경우
			battleTop[count + i].x += scoreList[i].x;
			battleTop[count + i].y += scoreList[i].y;
			battleTop[count + i].score = totalScore + scoreList[i].score;
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

	battleSearch(scoreList, 0, 0, 0, 0, 0, turn);
	sort(battleTop, battleTop + firstSearchNum * firstSearchNum, compareCoord);
}


//보드판의 총 점수를 받아오는 함수  - tempBoard와 board중 선택
int getScore(int bd[width][height]) //수정부탁
{
	if (bd == tempBoard)
	{

	}

	else if (bd == board)
	{

	}
}

//판의 점수를 관리하는 함수
void shapeScore(void) {

}