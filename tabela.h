#ifndef TABELA_H
#define TABELA_H

#define N_colunas 7
#define N_linhas 6


extern char tabela[N_linhas][N_colunas];
extern int index_disponivel[N_colunas];

void inicializa_tabela();
void desenha_tabela();
bool coluna_cheia(int coluna);
bool tabela_cheia();
bool posicao_valida(int coluna, int linha);



#endif
