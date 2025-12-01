
#ifndef JOGABILIDADE_H
#define JOGABILIDADE_H

#define symbol_jogador '$'

extern int N_bots;

void inicializa_jogadores();
bool venceu(int linha, int coluna, char symbol);
void jogador();
void bot(int index);


#endif

