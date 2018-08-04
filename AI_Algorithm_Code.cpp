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
#include "Connect6Algo.h"
#define width 19
#define height 19
// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:웅성,Department:한양대학교" };


typedef struct {
	int x;
	int y;
}Coord;


int tempBoard[width][height];
Coord blocking[10];
Coord battleTop[10];



void myturn(int cnt);
void saveBoard(); // 현재 board를 tempBoard에 저장
int tempIsFree(int x, int y);
void tempMyMove(int x, int y, int turn);
void checkBlocking(); // 블로킹들의 위치저장 , 구조체 배열에 저장
void changeBlocking(int n, int turn); //n번째 블로킹을 해당 turn의 돌로 변경
void Minimax(); //Minimax 알고리즘을 통해 둘 돌의 위치를 정하는 함수. dfs 방식을 따를 예정. 
				//domymove() 함수를 통해 둘 돌들의 위치까지 최종적으로 출력함.
void battleSearch(int tempboard[]);//Minimax 안에서의 전장탐색 알고리즘 - battleTop[] 에 점수 높은 순서대로 저장.
int getScore(int bd[]);//board 와 tempboard 에서의 총 점수값을 구합니다.
void shapeScore();//판의 점수를 관리하는 함수


void myturn(int cnt) {

	int x[2], y[2];
	saveBoard();
	
	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

	


	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	domymove(x, y, cnt);
}

void saveBoard(){ // 현재 board를 tempBoard에 저장
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 19; j++)
			tempBoard[i][j] = board[i][j];
	}
}

int tempIsFree(int x, int y){
	return x >= 0 && y >= 0 && x < width && y < height && tempBoard[x][y] == 0;
}

/**
*임시보드에 돌을 놔보기
turn 1 : my turn
turn 2 : op turn
*/
void tempMyMove(int x, int y, int turn){
	if(tempIsFree)
		tempBoard[x][y] = turn;
}


void checkBlocking(){ // 블로킹들의 위치저장 , 구조체 배열에 저장
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


void changeBlocking(int n, int turn){ //n번째 블로킹을 해당 turn의 돌로 변경

	int x = blocking[n].x;
	int y = blocking[n].y;

	tempBoard[x][y] = turn; //turn : 1 or 2
}


void Minimax()
{
	battleSearch(tempBoard);			//전장탐색 알고리즘을 통해 battleTop에 우선순위순서로 둘 돌의 위치를 저장합니다.


	getScore(tempBoard);

}

//qsort함수를 이용하여 상위 3개의 점수를 가진 전장을 찾아내어 배열 battleTop에 저장합니다.
void battleSearch(int tempboard[][])
{




}

int getScore(int bd[width][height])	//보드판의 총 점수를 받아오는 함수  - temp와 board
{
	if (bd == tempBoard)
	{

	}

	else if (bd == board)
	{

	}
}

void shapeScore(void) {

}