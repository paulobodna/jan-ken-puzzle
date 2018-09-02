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

int recursive_play(int **matrix, int row, int col, int actual_row, int actual_col);

void move_to(int **matrix, int row, int col, int actual_row, int actual_col, int new_row, int new_col) {
    int peace = matrix[actual_row][actual_col];

    // fazendo o movimento
    matrix[new_row][new_col] = peace;
    matrix[actual_row][actual_col] = 0;

    int qtd_peaces = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            qtd_peaces += recursive_play(matrix, row, col, i, j);
        }
    }

    // desfazendo o movimento
    matrix[actual_row][actual_col] = peace;
    matrix[new_row][new_col] = (peace%3)+1;

    // imprimir caso a resposta tenha sido encontrada
    if(qtd_peaces == 1) {
        printf("%d %d %d\n", new_row+1, new_col+1, peace);
    }

    return;
}

int recursive_play(int **matrix, int row, int col, int actual_row, int actual_col) {
    if(matrix[actual_row][actual_col] == 0) {
        return 0;
    }

    int peace = matrix[actual_row][actual_col];

    // -> ir para direita
    if(actual_col < col-1 && matrix[actual_row][actual_col+1] == (peace%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row, actual_col+1);
    }

    // -> ir para baixo
    if(actual_row < row-1 && matrix[actual_row+1][actual_col] == (peace%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row+1, actual_col);
    }
    
    // -> ir para esquerda
    if(actual_col > 0 && matrix[actual_row][actual_col-1] == (peace%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row, actual_col-1);
    }

    // -> ir para cima
    if(actual_row > 0 && matrix[actual_row-1][actual_col] == (peace%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row-1, actual_col);
    }

    return 1;
}

void find_solutions(int **matrix, int row, int col) {
    
    int qtd_peaces = 0;
    for(int i = 0; i < row; i ++) {
        for(int j = 0; j < col; j++) {
            if(matrix[i][j] != 0) {
                qtd_peaces += recursive_play(matrix, row, col, i, j);
            }
        }
    }

    if(qtd_peaces == 1) {
        printf("Nao ha movimentos possiveis.\n");
    }

    return;
}

int main(int argc, char* argv[]) {

    int row, col;
    scanf("%d %d", &row, &col);

    int **matrix;

    create_matrix(&matrix, row, col);
    read_matrix(matrix, row, col);
    print_matrix(matrix, row, col);

    find_solutions(matrix, row, col);

    delete_matrix(&matrix, row);


    return 0;
}
