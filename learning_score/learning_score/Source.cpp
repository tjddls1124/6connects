#include<stdio.h>
#include<stdlib.h>
#define listSize 128
#define parentNum 64			//

int whoIsWin(int[], int[]);		//�ΰ��� parentList�� �������� �� �̱����� ����
int childScoreList[listSize];	//parentList�鳢�� �����Ͽ� �̱����� ������ ������� list
int inputScoreList[listSize];	//���ʿ� ���� �־��� ���� ����Ʈ
int makedHeapList[listSize][parentNum];	//���ʿ� ���� ����Ʈ��� �װɷ� �Ļ��� ��� ����Ʈ (heap ���� �̿�)

void makeParentNode();			//random ����� �����Ͽ� 1������ �θ� ������ ����


//���� �ºη� �̱� ���� �ڼ����� ����ϴ�.
int main(void)
{
	int scoreList1[listSize], scoreList2[listSize];

	int winner = whoIsWin(scoreList1, scoreList2);

	if (winner == 1)
		for (int i = 0; i < listSize;i++)
			childScoreList[i] = scoreList1[i];

}


int whoIsWin(int scoreList1[listSize], int scoreList2[listSize]){
	
	
	int p1_score, p2_score;		//�÷��̾� 1�� �÷��̾� 2�� ������ �����ϴ� ����.
	

	if (p1_score>p2_score)
		return 1;

	else
		return 2;

}

//random ����� �����Ͽ� 1������ �θ� ������ ����
void makeParentNode(){
	 
	for (int i = 0; i < parentNum; i++)
		for (int j = 0; j < listSize; j++)
			makedHeapList[i][j] = inputScoreList[i] * srand(10);

}