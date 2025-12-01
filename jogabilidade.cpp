#include "tabela.h"
#include "jogabilidade.h"
#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <random>

using namespace std;

// Semáforo do jogador. Inicializa permitindo entrar na RC
binary_semaphore semaforo_jogador(1);

// Vetor com os semáforos para os N bots
vector<unique_ptr<binary_semaphore>> semaforos_bots;

// Variável de controle para fim do jogo
bool terminou = false;

int N_bots;
char bot_symbols[5] = {'@',  '#',  '%', '&', '!'};

/**
 * Inicializa os semáforos dos bots.
 * No início do jogo não permite que acesse a RC
 */
void inicializa_jogadores()
{
    for(int k = 0; k < N_bots; k++)
        semaforos_bots.emplace_back(make_unique<binary_semaphore>(0));
}

/**
 * Verifica se a peça em (linha, coluna) conecta com outras 3 peças,
 * formando um Connect Four.
 */
bool venceu(int linha, int coluna, char symbol)
{
    // Verifica se posição está nas dimensões do tabuleiro
    if(!posicao_valida(coluna, linha) || tabela[linha][coluna] != symbol) return false;  
    
    // Loop para cada posição adjacente da peça
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            // Condição para não pegar a peça passada como parâmetro
            if(i || j)
            {
                bool conectou = true;
                // Verifica se há outras 3 peças conectadas
                for(int k = 1; k <= 3; k++)
                {   int nova_linha = linha + i*k;
                    int nova_coluna = coluna + j*k;
                    // Checa se o símbolo é do jogador
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


/**
 * Função para a jogada do jogador. 
 * Pergunta qual coluna quer colocar a peça, verifica se é possível.
 * Adiciona a jogada no tabuleiro.
 */
void jogador()
{

    // Loop infinito até o jogo não terminar
    while(true)
    {
        // Entra na região crítica
        semaforo_jogador.acquire();

        /**
         * Se o jogo terminou, libera o próximo jogador a fazer a jogada,
         * nesse caso é do bot_0. Assim, o bot também encerra o loop de jogo.
         */
        if(terminou)
        {
            semaforos_bots[0]->release();
            return;
        }
        
        /**
         * Sequência para pegar a coluna da peça do jogador.
         * Faz verificação de validade da jogada.
         */
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

        /**
         * Checa se o jogador venceu. Caso venceu,
         * imprime mensagem e libera o bot_0 para encerrar
         * o loop de jogo.
         */
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

        /**
         * Checa se o jogo empatou (tabela cheia). Caso verdade,
         * imprime mensagem e libera o bot_0 para encerrar
         * o loop de jogo.
         */
        if(tabela_cheia())
        {
            cout << "\n     **TABULEIRO CHEIO! EMPATE...\n" << endl;
            desenha_tabela();
            terminou = true;
            semaforos_bots[0]->release();
            return;
        }

        /**
         * Imprime jogada e libera bot_0 para continuar o jogo
         */
        desenha_tabela();
        semaforos_bots[0]->release();
    }
}

/**
 * Função para a jogada dos bots.
 * Recebe um index [0, N-1] para identificação de qual bot é a jogada 
 * Aleatoriamente faz uma jogada.
 * Adiciona a jogada no tabuleiro.
 */
void bot(int index)
{
    // Loop infinito até o jogo não terminar
    while(true)
    {

        // Entra na região crítica
        semaforos_bots[index]->acquire();

        /**
         * Se o jogo terminou, libera o próximo jogador a fazer a jogada,
         * nesse caso é o bot_i+1. Se já for o último bot, 
         * libera para o jogador encerrar o loop de jogo
         */
        if(terminou)
        {
            if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();
            return;
        }


        cout << "\n     **VEZ DO BOT " << index + 1 << "**\n" << endl;

        //Seed para a escolha aleatória da coluna
        unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 gen(seed + index);
        uniform_int_distribution<int> distrib(0, N_colunas - 1);

        int coluna;
        do
        {
            // Escolhe uma coluna
            coluna = distrib(gen);
        }while(coluna_cheia(coluna));

        
        cout << "Bot " << index + 1 << " escolheu a coluna " << coluna + 1 << "\n" << endl;

       
        int linha = index_disponivel[coluna];
        tabela[linha][coluna] = bot_symbols[index];
        index_disponivel[coluna]--;

        /**
         * Checa se o bot venceu. Caso venceu,
         * imprime mensagem e libera próximo bot ou jogador 
         * para encerrar o loop de jogo.
         */
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

        /**
         * Checa se o jogo empatou (tabela cheia). Caso verdade,
         * imprime mensagem e libera o próximo jogador para 
         * encerrar o loop de jogo.
         */
        if(tabela_cheia())
        {
            cout << "\n     **TABULEIRO CHEIO! EMPATE...**\n" << endl;
            desenha_tabela();
            terminou = true;
            if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();
            return;
        }

        /**
         * Imprime jogada e libera proximo jogador para continuar o jogo
         */
        desenha_tabela();
        if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();     
    }
}