/*
2014105080 컴퓨터학부 장진화
20192 자료구조프로그래밍
실습문제 [1] cockcroach.c

문제 설명
 - gcc 컴파일러 사용
 - 실행 방법 : {exe file} {row number} {col number} {output filename} 
 - 파일 출력 : 최종 위치, 최종 방문 횟수, 방마다 방문 횟수 평균, 방마다의 방문 횟수
 - 50000번 안에 모두 방문하지 못할 경우 파일에 fail 출력
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define NUM_DIRECTIONS 8
#define MAX_ITERATION 50000
enum _directions {
    LEFT_DOWN = 0,
    DOWN,
    RIGHT_DOWN,
    RIGHT,
    RIGHT_UP,
    UP,
    LEFT_UP,
    LEFT
} DIRECTIONS; //방향에 대한 열거형 선언

//필요한 전역변수 선언
int row, col, current_row, current_col, final_row, final_col;
int iteration_count = 0;
int visit_count = 0;
int all_visited = FALSE;
char filename[100];

//선언한 열거형 변수와 같은 방향으로 움직여주는 배열
const int imove[NUM_DIRECTIONS] = {-1, 0, 1, 1,  1,  0, -1, -1};
const int jmove[NUM_DIRECTIONS] = { 1, 1, 1, 0, -1, -1, -1,  0};

//visit_matrix : 방문 여부 확인, count_matrix : 방문 횟수 계산
int** visit_matrix;
int** count_matrix;

//전달한 매개변수 확인
int checkAll(int argc, char* argv[])
{
    if(argc != 4) {
        printf("Please input {#row #col filename} for parameters\n");
        return FALSE;
    }

    row = atoi(argv[1]);
    col = atoi(argv[2]);
    strcpy(filename, argv[3]);

    if(row < 2 || row > 40) {
        printf("Please input correct row number: 2 <= row <= 40\n");
        return FALSE;
    }
    if(col < 2 || col > 20) {
        printf("Please input correct col number: 2 <= col <= 20\n");
        return FALSE;
    }

    return TRUE;
}

//2차원 배열 동적 할당
void createMatrix()
{
    int i, j;
    visit_matrix = (int**)malloc(row * sizeof(int*));
    count_matrix = (int**)malloc(row * sizeof(int*));
    for(i = 0; i < row; i++) {
        visit_matrix[i] = (int*)malloc(col * sizeof(int));
        count_matrix[i] = (int*)malloc(col * sizeof(int));
        for(j = 0; j < col; j++) {
            visit_matrix[i][j] = 0;
            count_matrix[i][j] = 0;
        }
    }
    //current_row/col : 바퀴벌레의 최초 위치 지정(배열의 한 가운데)
    current_row = row / 2;
    current_col = col / 2;
}

//바퀴벌레의 무작위 이동 함수
void moveCockcroach()
{
    //rand 함수 사용을 위한 선언
    srand(time(NULL));

    //최초 위치 방문 처리
    visit_matrix[current_row][current_col] = TRUE;
    count_matrix[current_row][current_col]++;
    iteration_count++;
    visit_count++;

    while(TRUE) {

        if(iteration_count == MAX_ITERATION) { //50000번 반복 이내에 해결하지 못할 경우 반복문 종료
            all_visited = FALSE;
            break;
        }

        if(visit_count == row * col) { //모든 방을 방문할 경우 반복문 종료
            //최종 위치 출력을 위해 final_row/col 저장
            final_row = current_row + 1;
            final_col = current_col + 1;
            all_visited = TRUE;
            break;
        }

        DIRECTIONS = rand() % 8; //무작위 방향 설정

        switch(DIRECTIONS) { //생성된 방향에 대한 처리(LEFT_DOWN에 모든 설명을 작성함)
        case LEFT_DOWN:

            if(current_row == 0 || current_col == col - 1) { //바퀴벌레가 배열의 가장자리에 위치할 경우
                //방문 횟수만 증가
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
            //생성된 방향으로 바퀴벌레를 움직임
            current_row += imove[LEFT_DOWN];
            current_col += jmove[LEFT_DOWN];

            if(visit_matrix[current_row][current_col] == FALSE) { //해당 방을 처음 방문한 경우
                //방문 여부를 TRUE로 바꾸고 방문 횟수 증가
                visit_matrix[current_row][current_col] = TRUE;
                count_matrix[current_row][current_col]++;
                iteration_count++;
                visit_count++;
                break;
            }
            else { //해당 방을 여러번 방문한 경우 방문 횟수만 증가
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        case DOWN:

            if(current_col == col - 1) {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }

            current_row += imove[DOWN];
            current_col += jmove[DOWN];

            if(visit_matrix[current_row][current_col] == FALSE) {
                count_matrix[current_row][current_col]++;
                visit_matrix[current_row][current_col] = TRUE;
                iteration_count++;
                visit_count++;
                break;
            }
            else {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        case RIGHT_DOWN:

            if(current_row == row - 1 || current_col == col - 1) {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }

            current_row += imove[RIGHT_DOWN];
            current_col += jmove[RIGHT_DOWN];

            if(visit_matrix[current_row][current_col] == FALSE) {
                count_matrix[current_row][current_col]++;
                visit_matrix[current_row][current_col] = TRUE;
                iteration_count++;
                visit_count++;
                break;
            }
            else {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        case RIGHT:

            if(current_row == row - 1) {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }

            current_row += imove[RIGHT];
            current_col += jmove[RIGHT];

            if(visit_matrix[current_row][current_col] == FALSE) {
                count_matrix[current_row][current_col]++;
                visit_matrix[current_row][current_col] = TRUE;
                iteration_count++;
                visit_count++;
                break;
            }
            else {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        case RIGHT_UP:

            if(current_row == row - 1 || current_col == 0) {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }

            current_row += imove[RIGHT_UP];
            current_col += jmove[RIGHT_UP];

            if(visit_matrix[current_row][current_col] == FALSE) {
                count_matrix[current_row][current_col]++;
                visit_matrix[current_row][current_col] = TRUE;
                iteration_count++;
                visit_count++;
                break;
            }
            else {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        case UP:

            if(current_col == 0) {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }

            current_row += imove[UP];
            current_col += jmove[UP];

            if(visit_matrix[current_row][current_col] == FALSE) {
                count_matrix[current_row][current_col]++;
                visit_matrix[current_row][current_col] = TRUE;
                iteration_count++;
                visit_count++;
                break;
            }
            else {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        case LEFT_UP:

            if(current_row == 0 || current_col == 0) {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }

            current_row += imove[LEFT_UP];
            current_col += jmove[LEFT_UP];

            if(visit_matrix[current_row][current_col] == FALSE) {
                count_matrix[current_row][current_col]++;
                visit_matrix[current_row][current_col] = TRUE;
                iteration_count++;
                visit_count++;
                break;
            }
            else {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        case LEFT:

            if(current_row == 0) {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }

            current_row += imove[LEFT];
            current_col += jmove[LEFT];

            if(visit_matrix[current_row][current_col] == FALSE) {
                count_matrix[current_row][current_col]++;
                visit_matrix[current_row][current_col] = TRUE;
                iteration_count++;
                visit_count++;
                break;
            }
            else {
                count_matrix[current_row][current_col]++;
                iteration_count++;
                break;
            }
        }
    }
}
//방마다의 방문 횟수를 파일에 출력
void printMatrix(FILE *outFile)
{
    int i, j;
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            fprintf(outFile, "%3d", count_matrix[i][j]);
        }
        fprintf(outFile, "\n");
    }
}
//파일에 바퀴벌레의 최종 위치, 최종 방문 횟수 총합, 방마다 방문 횟수 평균, 방마다 방문 횟수 출력
void writeFile()
{
    FILE *outFile;
    outFile = fopen(filename, "w");

    if(all_visited) { //모든 방을 방문한 경우
        fprintf(outFile, "Final location : { %d, %d }\n", final_row, final_col);
        fprintf(outFile, "Total Number of Visits : %d\n", iteration_count);
        fprintf(outFile, "Average of Visits per room : %lf\n", (float) iteration_count / (row * col));
        fprintf(outFile, "Number of Visits per room : \n");
        printMatrix(outFile);
    }
    else { //모든 방을 방문하지 못한 경우 fail 출력
        fprintf(outFile, "Final Location : { %d, %d }\n\n", final_row, final_col);
        fprintf(outFile, "Failed to visit all room within 50000 iterations");
    }
    fclose(outFile);
}

//2차원 배열 동적할당 해제
void freeMatrix()
{
    int i;
    for(i = 0; i < row; i++) {
        free(visit_matrix[i]);
        free(count_matrix[i]);
    }
    free(visit_matrix);
    free(count_matrix);
}

int main(int argc, char* argv[])
{
    if (checkAll(argc, argv) == FALSE) {
        return -1;
    }
    createMatrix();
    moveCockcroach();
    writeFile();
    freeMatrix();
    return 0;
}