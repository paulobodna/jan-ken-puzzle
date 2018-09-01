/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  jan-ken-puzzle solver
 *
 *        Version:  1.0
 *        Created:  01/09/2018 18:18:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paulo Bodnarchuki, paulo.henrique.cruz@usp.br
 *         Nº USP:  9790944
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void create_matrix(int ***matrix, int row, int col) {

    (*matrix) = (int **) malloc(row * sizeof(int *));
    for(int i = 0; i < row; i++) {
        (*matrix)[i] = (int *) malloc(col * sizeof(int));
    }

    return;
}

void read_matrix(int **matrix, int row, int col) {

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    return;
}

void print_matrix(int **matrix, int row, int col) {

    printf("\n");
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return;
}

void delete_matrix(int ***matrix, int row) {

    for(int i = 0; i < row; i++) {
        free((*matrix)[i]);
    }
    free(*matrix);

    *matrix = NULL;

    return;
}



int main(int argc, char* argv[]) {

    int row, col;
    scanf("%d %d", &row, &col);

    int **matrix;

    create_matrix(&matrix, row, col);
    read_matrix(matrix, row, col);
    print_matrix(matrix, row, col);


    delete_matrix(&matrix, row);


    return 0;
}
