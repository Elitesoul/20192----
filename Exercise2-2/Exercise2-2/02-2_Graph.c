/*
2014105080 ��ǻ���к� ����ȭ
20192 �ڷᱸ�����α׷���
�ǽ����� [2]-2 knight_tour_graph.c

���� ����
 - ���� ȯ�� : Visual Studio
 - ���� ��� : {exe file} {row number} {col number} {output filename}
 - �׷����� �̿��� ü���� �� knight�� ��� ������ ��� �̵� ���� ���
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

//�׷��� node �迭 ����ü
typedef struct tourhead {
	int dir[DIRECTIONS];
	int nodeSize;
}TOURHEAD;

//warnsdorff's rule�� ���� ���� �� �ⱸ ���� ����ü
typedef struct warnstour {
	int dir;
	int exit;
}WARNSTOUR;

//warnsdorff's rule�� ���� tour �迭 ����ü
typedef struct warnslist {
	WARNSTOUR tour[DIRECTIONS];
	int listSize;
}WARNSLIST;

int board[SIZE][SIZE];
TOURHEAD **graph;

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
	WARNSTOUR *list1 = (WARNSTOUR *)size1;
	WARNSTOUR *list2 = (WARNSTOUR *)size2;
	return list1->exit - list2->exit;
}

//warnsdorff's rule ���� : �ⱸ�� ���� ���� ������� ����
WARNSLIST warnsSort(TOURHEAD **graph, int row, int col)
{
	WARNSLIST temp_w;
	int i, temp_row, temp_col;
	temp_w.listSize = 0;

	for (i = 0; i < graph[row][col].nodeSize; i++) {
		temp_row = row + row_move[graph[row][col].dir[i]];
		temp_col = col + col_move[graph[row][col].dir[i]];
		if (tourable(temp_row, temp_col) == TRUE) {
			temp_w.tour[temp_w.listSize].dir = graph[row][col].dir[i];
			temp_w.tour[temp_w.listSize].exit = count_exit(temp_row, temp_col);
			temp_w.listSize++;
		}
	}
	qsort(temp_w.tour, temp_w.listSize, sizeof(WARNSTOUR), compare);
	return temp_w;
}

//knight �̵� �Լ� : Recursive Backtracking ���
int knightTour(TOURHEAD **graph, int row, int col)
{
	int i;
	int temp_row, temp_col;
	WARNSLIST warns;

	if (visit_count >= MAX_COUNT) {
		return TRUE;
	}

	warns = warnsSort(graph, row, col);

	for (i = 0; i < warns.listSize; i++) {
		visit_count++;
		temp_row = row + row_move[warns.tour[i].dir];
		temp_col = col + col_move[warns.tour[i].dir];
		board[temp_row][temp_col] = visit_count;
		if (knightTour(graph, temp_row, temp_col) == TRUE) {
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

//�׷��� ���� : ������ row, col�� ���� node �迭 ����
void makeGraph()
{
	int i, j, dir;
	int temp_row, temp_col;

	graph = (TOURHEAD **)malloc(SIZE * sizeof(TOURHEAD *));
	for (i = 0; i < SIZE; i++) {
		graph[i] = (TOURHEAD *)malloc(SIZE * sizeof(TOURHEAD));
		for (j = 0; j < SIZE; j++) {
			graph[i][j].nodeSize = 0;
			for (dir = 0; dir < DIRECTIONS; dir++) {
				temp_row = i + row_move[dir];
				temp_col = j + col_move[dir];
				if (tourable(temp_row, temp_col) == TRUE) {
					graph[i][j].dir[graph[i][j].nodeSize] = dir;
					graph[i][j].nodeSize++;
				}
			}
		}
	}
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

//�׷��� �����Ҵ� ����
void freeGraph()
{
	int i;
	for (i = 0; i < SIZE; i++) {
		free(graph[i]);
	}
	free(graph);
}

int main(int argc, char* argv[])
{
	if (checkAll(argc, argv) == FALSE) {
		return -1;
	}

	initBoard();
	makeGraph();
	knightTour(graph, current_row, current_col);
	writeFile();
	freeGraph();

	return 0;
}