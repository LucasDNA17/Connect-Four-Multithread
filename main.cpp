#include "jogabilidade.h"
#include "tabela.h"
#include "menu.h"
#include <thread>
#include <vector>

using namespace std;

/**
 * Ponto de entrada do programa. Chama as funções visuais do jogo
 * (menu, tabela), inicializa os jogadores e cria as threads.
 */
int main()
{

    // Menu inicial
    desenha_menu();

    // Quantidade que o jogador escolheu
    N_bots = quantidade_bots();

    // Imprime a tabela
    inicializa_tabela();
    desenha_tabela();
    inicializa_jogadores();

    // Define as threads
    // Thread JOGADOR: relacionado com a função jogador()
    thread thread_jogador(jogador);
    vector<thread> threads_bots;

    for(int i = 0; i < N_bots; i++)
    {
        // Thread BOT: relacionado com a função bot(). Passa índice do bot
        threads_bots.emplace_back(bot, i);
    }

    for(thread& t : threads_bots)
    {
        t.join();
    }
        
    thread_jogador.join();
}