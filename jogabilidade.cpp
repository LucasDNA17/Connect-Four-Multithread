#include "tabela.h"
#include "jogabilidade.h"
#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <cstdlib> 
#include <ctime>

using namespace std;


binary_semaphore semaforo_jogador(1);
vector<unique_ptr<binary_semaphore>> semaforos_bots;
bool terminou = false;

int N_bots;
char bot_symbols[5] = {'@',  '#',  '%', '&', '!'};


void inicializa_jogadores()
{
    for(int k = 0; k < N_bots; k++)
        semaforos_bots.emplace_back(make_unique<binary_semaphore>(0));
}

bool venceu(int linha, int coluna, char symbol)
{
    if(!posicao_valida(coluna, linha) || tabela[linha][coluna] != symbol) return false;    
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i || j)
            {
                bool conectou = true;
                for(int k = 1; k <= 3; k++)
                {   int nova_linha = linha + i*k;
                    int nova_coluna = coluna + j*k;
                    if(!posicao_valida(nova_coluna, nova_linha) || symbol != tabela[nova_linha][nova_coluna])
                    {
                        conectou = false;
                        break;
                    }       
                }

                if(conectou) return true;
            }
            
        }
    }

    return false;
}



void jogador()
{

    while(true)
    {
        semaforo_jogador.acquire();
        if(terminou)
        {
            semaforos_bots[0]->release();
            return;
        }
        
        
        cout << "\n     **SUA VEZ!**\n" << endl;
        cout << "Em qual coluna você vai colocar a peça?" << endl;

        int coluna;
        cin >> coluna;
        coluna -= 1;

        while(coluna < 0 || coluna > N_colunas - 1 || coluna_cheia(coluna))
        {
            if(coluna_cheia(coluna))
            {
                cout << "Coluna " << coluna + 1 << " está cheia!" << endl;
            }
            else
            {
                cout << "Coluna Inválida. Colunas possíveis: ";
                for (int i = 1; i <= N_colunas; i++) cout << i << " ";
                cout << endl;
            }

            cin >> coluna;
            coluna -= 1;
        }
        
            
        int linha = index_disponivel[coluna];
        tabela[linha][coluna] = symbol_jogador;
        index_disponivel[coluna]--;

        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1;j++)
            {
                if(venceu(linha + i, coluna +j, symbol_jogador))
                {
                    cout << "\n     **VOCÊ VENCEU!**\n" << endl;
                    desenha_tabela();
                    terminou = true;
                    semaforos_bots[0]->release();
                    return;
                }
            }
        }

        if(tabela_cheia())
        {
            cout << "\n     **TABULEIRO CHEIO! EMPATE...\n" << endl;
            desenha_tabela();
            terminou = true;
            semaforos_bots[0]->release();
            return;
        }

        desenha_tabela();
        semaforos_bots[0]->release();
    }
}


void bot(int index)
{
    while(true)
    {
        semaforos_bots[index]->acquire();

        if(terminou)
        {
            if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();
            return;
        }


        cout << "\n     **VEZ DO BOT " << index + 1 << "**\n" << endl;

        srand(time(0) + index);
        int coluna;
        do
        {
            coluna = rand() % N_colunas;
        }while(coluna_cheia(coluna));
        cout << "Bot " << index + 1 << " escolheu a coluna " << coluna + 1 << "\n" << endl;

       
        int linha = index_disponivel[coluna];
        tabela[linha][coluna] = bot_symbols[index];
        index_disponivel[coluna]--;

        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1;j++)
            {
                if(venceu(linha + i, coluna +j, bot_symbols[index]))
                {
                    cout << "     **BOT " << index + 1 << " VENCEU**\n" << endl;
                    desenha_tabela();
                    terminou = true;
                    if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();
                    return;
                }
            }
        }

        if(tabela_cheia())
        {
            cout << "\n     **TABULEIRO CHEIO! EMPATE...**\n" << endl;
            desenha_tabela();
            terminou = true;
            if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();
            return;
        }

        desenha_tabela();
        if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();     
    }
}