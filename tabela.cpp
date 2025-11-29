#include "tabela.h"
#include <iostream>

using namespace std;


char tabela[N_linhas][N_colunas];
int index_disponivel[N_colunas];

void inicializa_tabela()
{
    for(int i = 0; i < N_linhas; i++)
        for(int j = 0; j < N_colunas; j++)
            tabela[i][j] = '*';

    for(int j = 0; j < N_colunas; j++)
        index_disponivel[j] = N_linhas - 1;
}

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


bool coluna_cheia(int coluna)
{
    return index_disponivel[coluna] < 0;
}

bool tabela_cheia()
{
    for(int i = 0; i < N_colunas; i++)
        if (!coluna_cheia(i)) return false;

    return true;
}

bool posicao_valida(int coluna, int linha)
{
    return (coluna >= 0 && coluna < N_colunas && linha >= 0 && linha < N_linhas); 
}



