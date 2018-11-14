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

unsigned long checks_evited;

void create_char_matrix(char ***matrix, int row, int col) {

    (*matrix) = (char **) malloc(row * sizeof(char *));
    for(int i = 0; i < row; i++) {
        (*matrix)[i] = (char *) malloc(col * sizeof(char));
    }

    return;
}

void print_mat(char** matrix, int rows, int cols){

	for (int i = 0; i < rows; i++)	{
		for (int j = 0; j < cols; j++){
			printf("%c ",matrix[i][j] );
		}
		printf("\n");
	}	

}

void visit(int** matrix, char** colors_mat, int rows, int cols, int i , int j, int* pieces_curr_num){

    //letras:
    //g-> semi-processado
    //w-> nao-processado
    //b-> totalmente processado

	colors_mat[i][j] = 'g';
	(*pieces_curr_num)++;

	//cima
	if( (i-1 >= 0) &&  matrix[i-1][j] != 0 && colors_mat[i-1][j] == 'w') {
		visit(matrix, colors_mat, rows, cols, i-1 , j, pieces_curr_num);

	}
	
	//direita	
	if( (j+1 <= cols-1) &&  matrix[i][j+1] != 0 && colors_mat[i][j+1] == 'w') {
		visit(matrix, colors_mat, rows, cols, i , j+1, pieces_curr_num);

	}
	
	//baixo
	if( (i+1 <= rows-1) && matrix[i+1][j] != 0 && colors_mat[i+1][j] == 'w') {
		visit(matrix, colors_mat, rows, cols, i+1 , j, pieces_curr_num);

	}
	
	//esquerda	
	if( (j-1 >= 0) && matrix[i][j-1] !=0  && colors_mat[i][j-1] == 'w') {
		visit(matrix, colors_mat, rows, cols, i , j-1, pieces_curr_num);
		
	}

	colors_mat[i][j] = 'b';
	
}

void delete_matrix(char ***matrix, int row) {

    for(int i = 0; i < row; i++) {
        free((*matrix)[i]);
    }
    free(*matrix);

    *matrix = NULL;

    return;
}

int isle_checker(int** matrix, int rows, int cols ){

	char** colors_mat = NULL;
	int pieces_num  = 0; 
	int pieces_curr_num = 0;
	int start_i_pos;
	int start_j_pos;
	
    create_char_matrix(&colors_mat, rows, cols);

	//inicializa a matriz que será usada para marcar caminhos já processados
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			if(matrix[i][j] == 0){ //as casas que estão com o valor zero estão sendo marcadas como processadas
				colors_mat[i][j] = 'b';
			
			}else{
				
				pieces_num++; //contando a quantidade de peças no tabuleiro atual
				if(pieces_num == 1){	
					start_i_pos = i; //salvando a primeira posição diferente de zero do tabuleiro
					start_j_pos = j;
				}
				colors_mat[i][j] = 'w'; 
			}

		}
	}

	//percorre a matriz a partir da primeira casa nao vazia encontrada na inicialização acima. 
	//obs.: essa posição certamente não foi processada ainda 	
	visit(matrix, colors_mat, rows, cols, start_i_pos , start_j_pos, &pieces_curr_num); 
			
//    delete_matrix(&colors_mat, rows);

	if (pieces_curr_num == pieces_num){ 
		return 0;
	
	}else{
		return 1;
	}

}

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

long unsigned int to_int(int **matrix, int row, int col) {

    long unsigned int b = 0;
    
    int counter = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            b |= (long unsigned)matrix[i][j] << (counter*2);
            counter++;
        }
    }

    return b;
}

int how_many_pieces(int **matrix, int row, int col) {
    int qtd_pieces = 0;

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(matrix[i][j] > 0) qtd_pieces++;
        }
    }
    return qtd_pieces;
}

long unsigned recursive_play(int **matrix, int row, int col, int actual_row, int actual_col, int qtd_pieces, std::map<long unsigned int, long unsigned> &table);

// Move recursively, returning how many solutions were found
unsigned long move_to(int **matrix, int row, int col, int actual_row, int actual_col, int new_row, int new_col, int qtd_pieces, std::map<long unsigned int, long unsigned> &table) {

    // fazendo o movimento
    int piece = matrix[actual_row][actual_col];
    matrix[new_row][new_col] = piece;
    matrix[actual_row][actual_col] = 0;

    // verifica se a configuração nova do tabuleiro já está gravada no map
    long unsigned number = to_int(matrix, row, col);
    if(table.find(number) != table.end()) {
        checks_evited += table[number];

        // desfazendo o movimento
        matrix[actual_row][actual_col] = piece;
        matrix[new_row][new_col] = (piece%3)+1;

        return table[number];
    }

    // checando se as pecas formam "ilhas" diversas
    if(isle_checker(matrix, row, col)) {
        checks_evited += table[number];

        // desfazendo o movimento
        matrix[actual_row][actual_col] = piece;
        matrix[new_row][new_col] = (piece%3)+1;

        return 0;
    }

    unsigned long solutions = 0;
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

// play recursively, returning how many solutions were found
// long unsigned recursive_play(int **matrix, int row, int col, int actual_row, int actual_col, int qtd_pieces, std::map<long unsigned int, long unsigned> &table) {
    if(matrix[actual_row][actual_col] == 0) {
        return 0;
    }

    long unsigned solutions = 0;

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

unsigned long find_solutions(int **matrix, int row, int col, std::map<long unsigned int, long unsigned> &table) {
    int qtd_pieces = how_many_pieces(matrix, row, col);

    unsigned long solutions = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(matrix[i][j] != 0) {
                solutions += recursive_play(matrix, row, col, i, j, qtd_pieces, table);
            }
        }
    }

    return solutions;
}

void print_sol (int row, int col, unsigned long sol, std::map<long unsigned int, long unsigned> &table) {
    printf("%lu\n", sol);

    int **matrix;
    create_matrix(&matrix, row, col);

    int qtd_cases = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            for(int k = 1; k < 4; k++) {
                matrix[i][j] = k;
                long unsigned number = to_int(matrix, row, col);

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
//    printf("Map size: %lu\nEvited checks: %lu\n", table.size(), checks_evited);
    
    delete_matrix(&matrix, row);

    return;
}

int main(int argc, char* argv[]) {

    int row, col;
    scanf("%d %d", &row, &col);

    int **matrix;

    create_matrix(&matrix, row, col);
    std::map<long unsigned int, long unsigned> table;

    read_matrix(matrix, row, col);

    checks_evited = 0;
    long unsigned sol = find_solutions(matrix, row, col, table);
    print_sol(row, col, sol, table);

    delete_matrix(&matrix, row);

    return 0;
}
