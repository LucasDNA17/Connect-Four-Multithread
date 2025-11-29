#include "jogabilidade.h"
#include "tabela.h"
#include "menu.h"
#include <thread>
#include <vector>

using namespace std;

int main()
{

    desenha_menu();
    N_bots = quantidade_bots();

    inicializa_tabela();
    desenha_tabela();
    inicializa_jogadores();

    thread thread_jogador(jogador);
    vector<thread> threads_bots;

    for(int i = 0; i < N_bots; i++)
    {
        threads_bots.emplace_back(bot, i);
    }

    for(thread& t : threads_bots)
    {
        t.join();
    }
        
    thread_jogador.join();
}