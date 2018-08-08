#include<stdio.h>
#include<stdlib.h>
#define listSize 128
#define parentNum 64			//

int whoIsWin(int[], int[]);		//두개의 parentList가 경쟁했을 때 이긴쪽을 리턴
int childScoreList[listSize];	//parentList들끼리 경쟁하여 이긴쪽의 값으로 만들어진 list
int inputScoreList[listSize];	//최초에 직접 넣어줄 원본 리스트
int makedHeapList[listSize][parentNum];	//최초에 만든 리스트들과 그걸로 파생된 모든 리스트 (heap 구조 이용)

void makeParentNode();			//random 방식을 적용하여 1세대의 부모 노드들을 생성


//단판 승부로 이긴 쪽을 자손으로 만듭니다.
int main(void)
{
	int scoreList1[listSize], scoreList2[listSize];

	int winner = whoIsWin(scoreList1, scoreList2);

	if (winner == 1)
		for (int i = 0; i < listSize;i++)
			childScoreList[i] = scoreList1[i];

}


int whoIsWin(int scoreList1[listSize], int scoreList2[listSize]){
	
	
	int p1_score, p2_score;		//플레이어 1과 플레이어 2의 점수를 저장하는 변수.
	

	if (p1_score>p2_score)
		return 1;

	else
		return 2;

}

//random 방식을 적용하여 1세대의 부모 노드들을 생성
void makeParentNode(){
	 
	for (int i = 0; i < parentNum; i++)
		for (int j = 0; j < listSize; j++)
			makedHeapList[i][j] = inputScoreList[i] * srand(10);

}