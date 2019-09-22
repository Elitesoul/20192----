/*
2014105080 ��ǻ���к� ����ȭ
20192 �ڷᱸ�����α׷���
�ǽ����� [2]-1 knight_tour_array.c

���� ����
 - ���� ȯ�� : Visual Studio
 - ���� ��� : {exe file} {row number} {col number} {output filename}
 - �迭�� �̿��� ü���� �� knight�� ��� ������ ��� �̵� ���� ���
 - ���� ��� : ü�� ���� �湮 ����
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

//���� �ε��� �� �ⱸ ���� ����ü
typedef struct tour {
	int dir;
	int exit;
}TOUR;

//warnsdorff's rule�� ���� ����ü
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

//������ �Ű����� Ȯ��
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

//������ ��ġ���� �� �� �ִ� ���� Ȯ��
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

//������ ��ġ���� �� �� �ִ� �� ���� Ƚ�� ���
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

//qsort�� �� �Լ�
int compare(const void *size1, const void *size2)
{
	TOUR *list1 = (TOUR *)size1;
	TOUR *list2 = (TOUR *)size2;
	return list1->exit - list2->exit;
}

//warnsdorff's rule ���� : �ⱸ�� ���� ���� ������� ����
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

//knight �̵� �Լ� : Recursive Backtracking ���
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

//board �迭 �ʱ�ȭ �� ���� ��ġ �湮 ������ 0���� ����
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

//���Ͽ� ���
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