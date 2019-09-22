/*
2014105080 컴퓨터학부 장진화
20192 자료구조프로그래밍
실습문제 [2]-1 knight_tour_array.c

문제 설명
 - 개발 환경 : Visual Studio
 - 실행 방법 : {exe file} {row number} {col number} {output filename}
 - 배열을 이용한 체스판 위 knight가 모든 발판을 밟는 이동 순서 출력
 - 파일 출력 : 체스 말판 방문 순서
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define EMPTY -1
#define SIZE 8
#define DIRECTIONS 8
#define MAX_COUNT 63 //0 ~ 63

//방향 인덱스 및 출구 개수 구조체
typedef struct tour {
	int dir;
	int exit;
}TOUR;

//warnsdorff's rule을 위한 구조체
typedef struct warnslist {
	TOUR tour[DIRECTIONS];
	int listSize;
}WARNSLIST;

int board[SIZE][SIZE];

const int row_move[DIRECTIONS] = { -2, -1,  1,  2,  2,  1, -1, -2 };
const int col_move[DIRECTIONS] = {  1,  2,  2,  1, -1, -2, -2, -1 };

int current_row, current_col;
int visit_count = 0;
char *filename;

//전달한 매개변수 확인
int checkAll(int argc, char* argv[])
{
	if (argc != 4) {
		printf("Please input {#row #col filename} for parameters\n");
		return FALSE;
	}

	current_row = atoi(argv[1]);
	current_col = atoi(argv[2]);
	filename = argv[3];

	if (current_row < 0 || current_row > 7 || current_col < 0 || current_col > 7) {
		printf("Please input correct row/col number: 0 <= row/col <= 7\n");
		return FALSE;
	}

	return TRUE;
}

//지정된 위치에서 갈 수 있는 발판 확인
int tourable(int row, int col)
{
	if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
		return FALSE;
	}
	if (board[row][col] != EMPTY) {
		return FALSE;
	}
	return TRUE;
}

//지정된 위치에서 갈 수 있는 총 발판 횟수 계산
int count_exit(int row, int col)
{
	int i, temp_row, temp_col;
	int temp_count = 0;

	for (i = 0; i < DIRECTIONS; i++) {
		temp_row = row + row_move[i];
		temp_col = col + col_move[i];
		if (tourable(temp_row, temp_col) == TRUE) {
			temp_count++;
		}
	}

	return temp_count;
}

//qsort용 비교 함수
int compare(const void *size1, const void *size2)
{
	TOUR *list1 = (TOUR *)size1;
	TOUR *list2 = (TOUR *)size2;
	return list1->exit - list2->exit;
}

//warnsdorff's rule 실행 : 출구가 가장 적은 순서대로 정렬
WARNSLIST warnsSort(int row, int col)
{
	WARNSLIST temp_w;
	int i, temp_row, temp_col;
	temp_w.listSize = 0;

	for (i = 0; i < DIRECTIONS; i++) {
		temp_row = row + row_move[i];
		temp_col = col + col_move[i];
		if (tourable(temp_row, temp_col) == TRUE) {
			temp_w.tour[temp_w.listSize].dir = i;
			temp_w.tour[temp_w.listSize].exit = count_exit(temp_row, temp_col);
			temp_w.listSize++;
		}
	}
	qsort(temp_w.tour, temp_w.listSize, sizeof(TOUR), compare);
	return temp_w;
}

//knight 이동 함수 : Recursive Backtracking 사용
int knightTour(int row, int col)
{
	int i;
	int temp_row, temp_col;
	WARNSLIST warns;

	if (visit_count >= MAX_COUNT) {
		return TRUE;
	}

	warns = warnsSort(row, col);

	for (i = 0; i < warns.listSize; i++) {
		visit_count++;
		temp_row = row + row_move[warns.tour[i].dir];
		temp_col = col + col_move[warns.tour[i].dir];
		board[temp_row][temp_col] = visit_count;
		if (knightTour(temp_row, temp_col) == TRUE) {
			return TRUE;
		}
		board[temp_row][temp_col] = EMPTY;
		visit_count--;
	}

	return FALSE;
}

//board 배열 초기화 및 최초 위치 방문 순서를 0으로 지정
void initBoard()
{
	int i, j;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			board[i][j] = EMPTY;
		}
	}
	board[current_row][current_col] = 0;
}

//파일에 출력
void writeFile()
{
	int i, j;
	FILE *outfile;
	outfile = fopen(filename, "w");

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			fprintf(outfile, "%3d", board[i][j]);
		}
		fprintf(outfile, "\n");
	}
	fclose(outfile);
}

int main(int argc, char* argv[])
{
	if (checkAll(argc, argv) == FALSE) {
		return -1;
	}

	initBoard();
	knightTour(current_row, current_col);
	writeFile();

	return 0;
}