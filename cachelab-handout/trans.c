/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

	// 来源 https://yangtau.me/computer-system/csapp-cache.html
	int block_size = 8;
	for (int i = 0; i < N; i += block_size) {
    	for (int j = 0; j < M; j += block_size) {
        for (int k = 0; k < block_size / 2; k++) {
            // A top left
           int a0 = A[k + i][j];
           int a1 = A[k + i][j + 1];
           int a2 = A[k + i][j + 2];
           int a3 = A[k + i][j + 3];

            // copy
            // A top right
            int a4 = A[k + i][j + 4];
            int a5 = A[k + i][j + 5];
            int a6 = A[k + i][j + 6];
            int a7 = A[k + i][j + 7];

            // B top left
            B[j][k + i] = a0;
            B[j + 1][k + i] = a1;
            B[j + 2][k + i] = a2;
            B[j + 3][k + i] = a3;

            // copy
            // B top right
            B[j + 0][k + 4 + i] = a4;
            B[j + 1][k + 4 + i] = a5;
            B[j + 2][k + 4 + i] = a6;
            B[j + 3][k + 4 + i] = a7;
        }
        for (int k = 0; k < block_size / 2; k++) {
            // step 1 2
            int a0 = A[i + 4][j + k]; 
	    int a4 = A[i + 4][j + k + 4];
            int a1 = A[i + 5][j + k]; 
	    int a5 = A[i + 5][j + k + 4];
            int a2 = A[i + 6][j + k]; 
	    int a6 = A[i + 6][j + k + 4];
            int a3 = A[i + 7][j + k];
	    int a7 = A[i + 7][j + k + 4];
            // step 3
            int tmp = B[j + k][i + 4]; 
	    B[j + k][i + 4] = a0;
	    a0 = tmp;
            tmp = B[j + k][i + 5];
	    B[j + k][i + 5] = a1;
	    a1 = tmp;
            tmp = B[j + k][i + 6];
	    B[j + k][i + 6] = a2;
	    a2 = tmp;
            tmp = B[j + k][i + 7];
	    B[j + k][i + 7] = a3;
	    a3 = tmp;
            // step 4
            B[j + k + 4][i + 0] = a0;
	    B[j + k + 4][i + 4 + 0] = a4;
            B[j + k + 4][i + 1] = a1;
	    B[j + k + 4][i + 4 + 1] = a5;
            B[j + k + 4][i + 2] = a2;
	    B[j + k + 4][i + 4 + 2] = a6;
            B[j + k + 4][i + 3] = a3;
	    B[j + k + 4][i + 4 + 3] = a7;
        }
    }
}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

