#include "tabela.h"
#include <iostream>

using namespace std;

// Guarda os símbolos dos jogadores
char tabela[N_linhas][N_colunas];

// Posição permitida da jogada
int index_disponivel[N_colunas];

void inicializa_tabela()
{
    // Inicializa com espaço vazio (*)
    for(int i = 0; i < N_linhas; i++)
        for(int j = 0; j < N_colunas; j++)
            tabela[i][j] = '*';

    // Primeiro espaço disponível é a base
    for(int j = 0; j < N_colunas; j++)
        index_disponivel[j] = N_linhas - 1;
}

/**
 * Desenha a tabela com os símbolos dos jogadores
 * com uma formatação visual.
 * 
      1   2   3   4   5   6   7 
    +---+---+---+---+---+---+---+
    | * | * | * | * | * | * | * |
    +---+---+---+---+---+---+---+
    | * | * | * | * | * | * | * |
    +---+---+---+---+---+---+---+
    | * | * | * | * | * | * | * |
    +---+---+---+---+---+---+---+
    | * | * | * | * | * | * | * |
    +---+---+---+---+---+---+---+
    | * | * | * | * | * | * | * |
    +---+---+---+---+---+---+---+
    | * | * | * | * | * | * | * |
    +---+---+---+---+---+---+---+
 */
void desenha_tabela() {
    cout << "\n  "; 
    for (int j = 0; j < N_colunas; ++j) {
        cout << "  " << j + 1 << " ";
    }
    cout << "\n";

    for (int i = 0; i < N_linhas; ++i) {
        cout << "  +"; 
        for (int j = 0; j < N_colunas; ++j) {
            cout << "---+"; 
        }
        cout << endl;
        
        cout << "  "; 
        for (int j = 0; j < N_colunas; ++j) {
            cout << "| " << tabela[i][j] << " ";
        }
        cout << "|" << endl; 
    }

    cout << "  +";
    for (int j = 0; j < N_colunas; ++j) {
        cout << "---+";
    }
    cout << endl;
}

/**
 * Retorna um booleano se não há mais espaço na coluna
 */
bool coluna_cheia(int coluna)
{
    return index_disponivel[coluna] < 0;
}

/**
 * Retorna um booleano se a tabela está totalmente preenchida
 */
bool tabela_cheia()
{
    for(int i = 0; i < N_colunas; i++)
        if (!coluna_cheia(i)) return false;

    return true;
}

/**
 * Verifica se a posição não está fora das dimensões do tabuleiro
 */
bool posicao_valida(int coluna, int linha)
{
    return (coluna >= 0 && coluna < N_colunas && linha >= 0 && linha < N_linhas); 
}



