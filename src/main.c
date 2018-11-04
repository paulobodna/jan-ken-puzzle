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
 *        Authors:  Paulo Bodnarchuki (9790944)
 *                  Eduardo Zaboto Mirolli (9778501)
 *                  Rodrigo Noventa Junior (9791243)
 *
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <map>

typedef struct solution {
    int qtd_sol;
    int qtd_cases;
} SOLUTION;

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

//void create_sol(SOLUTION **sol) {
//
//    (*sol) = (SOLUTION *) malloc(sizeof(SOLUTION));
//
//    (*sol)->qtd_sol = 0;
//    (*sol)->qtd_cases = 0;
//
//    return;
//}

//void delete_sol(SOLUTION **sol) {
//    free((*sol)->cases);
//    free((*sol));
//
//    (*sol) = NULL;
//
//    return;
//}

long unsigned int to_int(int **matrix, int row, int col) {

    long unsigned int b = 0;
    
    int counter = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            b |= matrix[i][j] << (counter*2);
            counter++;
        }
    }

    return b;
}

int how_many_pieces(int **matrix, int row, int col) {
    int qtd_pieces = 0;

    printf("peças\n");
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(matrix[i][j] > 0) qtd_pieces++;
        }
    }
    return qtd_pieces;
}

int recursive_play(int **matrix, int row, int col, int actual_row, int actual_col, int qtd_pieces, std::map<long unsigned int, int> &table);

// realiza o movimento requisitado recursivamente,
// desfazendo-o ao retornar
int move_to(int **matrix, int row, int col, int actual_row, int actual_col, int new_row, int new_col, int qtd_pieces, std::map<long unsigned int, int> &table) {

    // fazendo o movimento
    int piece = matrix[actual_row][actual_col];
    matrix[new_row][new_col] = piece;
    matrix[actual_row][actual_col] = 0;

    // verifica se a configuração nova do tabuleiro já está gravada no map
    long unsigned int number = to_int(matrix, row, col);
    if(table.find(number) != table.end()) {
        return table[number];
    }

    int solutions = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            solutions += recursive_play(matrix, row, col, i, j, qtd_pieces, table);
        }
    }
    
    if(qtd_pieces <= 1) {
        solutions = 1;
    }
    // salvando a configuracao no map
    table[number] = solutions;

    // desfazendo o movimento
    matrix[actual_row][actual_col] = piece;
    matrix[new_row][new_col] = (piece%3)+1;

    return solutions;
}

// realiza as jogadas recursivamente
int recursive_play(int **matrix, int row, int col, int actual_row, int actual_col, int qtd_pieces, std::map<long unsigned int, int> &table) {
    if(matrix[actual_row][actual_col] == 0) {
        return 0;
    }

    int solutions = 0;

    int piece = matrix[actual_row][actual_col];

    // -> ir para direita
    if(actual_col < col-1 && matrix[actual_row][actual_col+1] == (piece%3)+1) {
        solutions += move_to(matrix, row, col, actual_row, actual_col, actual_row, actual_col+1, qtd_pieces-1, table);
    }

    // -> ir para baixo
    if(actual_row < row-1 && matrix[actual_row+1][actual_col] == (piece%3)+1) {
        solutions += move_to(matrix, row, col, actual_row, actual_col, actual_row+1, actual_col, qtd_pieces-1, table);
    }
    
    // -> ir para esquerda
    if(actual_col > 0 && matrix[actual_row][actual_col-1] == (piece%3)+1) {
        solutions += move_to(matrix, row, col, actual_row, actual_col, actual_row, actual_col-1, qtd_pieces-1, table);
    }

    // -> ir para cima
    if(actual_row > 0 && matrix[actual_row-1][actual_col] == (piece%3)+1) {
        solutions += move_to(matrix, row, col, actual_row, actual_col, actual_row-1, actual_col, qtd_pieces-1, table);
    }

    return solutions;
}

int find_solutions(int **matrix, int row, int col, std::map<long unsigned int, int> &table) {
    printf("Comecou a encontrar solucoes.\n");
    int qtd_pieces = how_many_pieces(matrix, row, col);
    printf("Pecas no jogo: %d\n", qtd_pieces);

    int solutions = 0;
    for(int i = 0; i < row; i ++) {
        for(int j = 0; j < col; j++) {
            if(matrix[i][j] != 0) {
                printf("Posicao [%d][%d]:\n", i, j);
                solutions += recursive_play(matrix, row, col, i, j, qtd_pieces, table);
            }
        }
    }

    return solutions;
}

// incluir map
void print_sol (int row, int col, int sol, std::map<long unsigned int, int> &table) {
    printf("%d\n", sol);

    int **matrix;
    create_matrix(&matrix, row, col);

    int qtd_cases = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            for(int k = 1; k < 4; k++) {
                matrix[i][j] = k;
                long unsigned int number = to_int(matrix, row, col);

                if(table.find(number) != table.end()) {
                    qtd_cases++;
                }
            }
            matrix[i][j] = 0;
        }
    }

    printf("%d\n", qtd_cases);

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            for(int k = 1; k < 4; k++) {
                matrix[i][j] = k;
                long unsigned int number = to_int(matrix, row, col);

                if(table.find(number) != table.end()) {
                    printf("%d %d %d\n", i+1, j+1, k);
                }
            }
            matrix[i][j] = 0;
        }
    }
    
    delete_matrix(&matrix, row);

    return;
}

int main(int argc, char* argv[]) {

    int row, col;
    scanf("%d %d", &row, &col);

    int **matrix;
//    SOLUTION *sol;

    create_matrix(&matrix, row, col);
//    create_sol(&sol);
    std::map<long unsigned int, int> table;

    read_matrix(matrix, row, col);

    int sol = find_solutions(matrix, row, col, table);
    printf("Percorreu o mapa.\n");
    print_sol(row, col, sol, table);
    printf("Terminou de imprimir a resposta.\n");

//    delete_sol(&sol);
    delete_matrix(&matrix, row);

    return 0;
}
