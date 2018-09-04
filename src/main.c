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

typedef struct endcase {
    int row, col, piece;
} ENDCASE;

typedef struct solution {
    int qtd_sol;
    int qtd_cases;
    ENDCASE *cases;
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

int recursive_play(int **matrix, int row, int col, int actual_row, int actual_col, SOLUTION *sol);

// verifica se ha uma solucao identica ja encontrada
int new_case(SOLUTION *sol, int new_row, int new_col, int piece) {
    for(int i = 0; i < sol->qtd_cases; i++) {
        if(sol->cases[i].row == new_row &&
            sol->cases[i].col == new_col &&
            sol->cases[i].piece == piece) {
            return 0;
        }
    }

    return 1;
}

// realiza o movimento requisitado recursivamente,
// desfazendo-o ao retornar
void move_to(int **matrix, int row, int col, int actual_row, int actual_col, int new_row, int new_col, SOLUTION *sol) {
    int piece = matrix[actual_row][actual_col];

    // fazendo o movimento
    matrix[new_row][new_col] = piece;
    matrix[actual_row][actual_col] = 0;

    int qtd_pieces = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            qtd_pieces += recursive_play(matrix, row, col, i, j, sol);
        }
    }

    // desfazendo o movimento
    matrix[actual_row][actual_col] = piece;
    matrix[new_row][new_col] = (piece%3)+1;

    // salvar caso a resposta tenha sido encontrada
    if(qtd_pieces == 1) {
        if(new_case(sol, new_row, new_col, piece)) {
            sol->cases = realloc(sol->cases, sizeof(ENDCASE) * (sol->qtd_cases+1));
            sol->cases[sol->qtd_cases].row = new_row;
            sol->cases[sol->qtd_cases].col = new_col;
            sol->cases[sol->qtd_cases].piece = piece;


            sol->qtd_cases++;
        }

        sol->qtd_sol++;
    }

    return;
}

// realiza as jogadas recursivamente
int recursive_play(int **matrix, int row, int col, int actual_row, int actual_col, SOLUTION *sol) {
    if(matrix[actual_row][actual_col] == 0) {
        return 0;
    }

    int piece = matrix[actual_row][actual_col];

    // -> ir para direita
    if(actual_col < col-1 && matrix[actual_row][actual_col+1] == (piece%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row, actual_col+1, sol);
    }

    // -> ir para baixo
    if(actual_row < row-1 && matrix[actual_row+1][actual_col] == (piece%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row+1, actual_col, sol);
    }
    
    // -> ir para esquerda
    if(actual_col > 0 && matrix[actual_row][actual_col-1] == (piece%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row, actual_col-1, sol);
    }

    // -> ir para cima
    if(actual_row > 0 && matrix[actual_row-1][actual_col] == (piece%3)+1) {
        move_to(matrix, row, col, actual_row, actual_col, actual_row-1, actual_col, sol);
    }

    return 1;
}

void find_solutions(int **matrix, int row, int col, SOLUTION *sol) {
    
    int qtd_pieces = 0;
    for(int i = 0; i < row; i ++) {
        for(int j = 0; j < col; j++) {
            if(matrix[i][j] != 0) {
                qtd_pieces += recursive_play(matrix, row, col, i, j, sol);
            }
        }
    }

    if(qtd_pieces == 1) {
        printf("Nao ha movimentos possiveis.\n");
    }

    return;
}

void create_sol(SOLUTION **sol) {

    (*sol) = (SOLUTION *) malloc(sizeof(SOLUTION));

    (*sol)->qtd_sol = 0;
    (*sol)->qtd_cases = 0;
    (*sol)->cases = NULL;

    return;
}

void delete_sol(SOLUTION **sol) {
    free((*sol)->cases);
    free((*sol));

    (*sol) = NULL;

    return;
}

void print_sol (SOLUTION *sol) {
    printf("%d\n%d\n", sol->qtd_sol, sol->qtd_cases);
    for(int i = 0; i < sol->qtd_cases; i++) {
        printf("%d %d %d\n", sol->cases[i].row,
                            sol->cases[i].col,
                            sol->cases[i].piece);
    }

    return;
}

// funcao de comparacao para ordenar casos
int comp(const void *elem1, const void *elem2) {

    ENDCASE f = *((ENDCASE *)elem1);
    ENDCASE s = *((ENDCASE *)elem2);

    if(f.row <= s.row && f.col <= s.col && f.piece < s.piece) return -1;
    else return 1;

    return 0;
}

int main(int argc, char* argv[]) {

    int row, col;
    scanf("%d %d", &row, &col);

    int **matrix;
    SOLUTION *sol;

    create_matrix(&matrix, row, col);
    create_sol(&sol);

    read_matrix(matrix, row, col);

    find_solutions(matrix, row, col, sol);
    // ordenar casos
    qsort(sol->cases, sol->qtd_cases, sizeof(ENDCASE), comp);
    print_sol(sol);

    delete_sol(&sol);
    delete_matrix(&matrix, row);

    return 0;
}
