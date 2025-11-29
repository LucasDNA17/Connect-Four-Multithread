#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <cstdlib> 
#include <ctime>
using namespace std;

#define N_bots 3
#define N_colunas 7
#define N_linhas 6
#define symbol_jogador '$'


char bot_symbols[5] = {'@',  '#',  '%', '&', '!'};


char tabuleiro[N_linhas][N_colunas];
int index_disponivel[N_colunas];
bool terminou = false;

binary_semaphore semaforo_jogador(1);
vector<unique_ptr<binary_semaphore>> semaforos_bots;


void inicializa_tabuleiro()
{
    for(int i = 0; i < N_linhas; i++)
        for(int j = 0; j < N_colunas; j++)
            tabuleiro[i][j] = '*';

    for(int j = 0; j < N_colunas; j++)
        index_disponivel[j] = N_linhas - 1;

    for(int k = 0; k < N_bots; k++)
        semaforos_bots.emplace_back(make_unique<binary_semaphore>(0));
}


void desenha_tabuleiro(void)
{
    for(int i = 0; i < N_linhas; i++)
    {
        cout << "|";
        for(int j = 0; j < N_colunas; j++)
        {
            cout << " " << tabuleiro[i][j];
        }

        cout << " |" << endl;
    }
}


bool coluna_cheia(int coluna)
{
    return index_disponivel[coluna] < 0;
}

bool tabuleiro_cheio()
{
    for(int i = 0; i < N_colunas; i++)
        if (!coluna_cheia(i)) return false;

    return true;
}

bool posicao_valida(int coluna, int linha)
{
    return (coluna >= 0 && coluna < N_colunas && linha >= 0 && linha < N_linhas); 
}


bool venceu(int linha, int coluna)
{    
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i || j)
            {
                bool conectou = true;
                for(int k = 1; k <= 3; k++)
                {
                    if(tabuleiro[linha][coluna] != tabuleiro[linha + i*k][coluna + j*k])
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
        
        
        cout << "VEZ DO JOGADOR" << endl;
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
        }
        
            
        int linha = index_disponivel[coluna];
        tabuleiro[linha][coluna] = symbol_jogador;
        index_disponivel[coluna]--;
                
        if(venceu(linha, coluna))
        {
            cout << "JOGADOR VENCEU!" << endl;
            terminou = true;
            semaforos_bots[0]->release();
            return;
        }

        else if(tabuleiro_cheio())
        {
            cout << "TABULEIRO CHEIO! EMPATE!" << endl;
            terminou = true;
            semaforos_bots[0]->release();
            return;
        }

        desenha_tabuleiro();
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

        cout << "VEZ DO BOT " << index << endl;

        srand(time(0) + index);
        int coluna;
        do
        {
            coluna = rand() % N_colunas;
        }while(coluna_cheia(coluna));


        int linha = index_disponivel[coluna];
        tabuleiro[linha][coluna] = bot_symbols[index];
        index_disponivel[coluna]--;
                
        if(venceu(linha, coluna))
        {
            cout << "BOT VENCEU!" << index << endl;
            terminou = true;
            if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();
            return;
        }
        else if(tabuleiro_cheio())
        {
            cout << "TABULEIRO CHEIO! EMPATE!" << endl;
            terminou = true;
            if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();
            return;
        }

        desenha_tabuleiro();
        if (index < N_bots - 1) semaforos_bots[index + 1]->release(); else semaforo_jogador.release();     
    }
}





int main(void)
{
    inicializa_tabuleiro();
    desenha_tabuleiro();

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