# Connect-Four-Multithread

```
  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-. 
 / C \/ O \/ N \/ N \/ E \/ C \/ T \/   \/ F \/ O \/ U \/ R \ 
 |   ||   ||   ||   ||   ||   ||   |\   /|   ||   ||   ||   |
 \---/\---/\---/\---/\---/\---/\---/  \-/ \---/\---/\---/\---/
```

Esse repositório faz parte do Trabalho Prático da disciplina SSC0140 - Sistemas Operacionais I. O trabalho consiste de um jogo com aplicações de conceitos aprendidos na matéria: semáforos e threads. 

Nessa direção, o nosso grupo implementou o jogo clássico Connect Four utilizando esses requisitos.

## Como jogar

Para jogar **Connect-Four-Multithread** é preciso compilar o código, logo, necessita-se que C++20 esteja instalado em sua máquina. Para compilar, execute o comando `make` no diretório raíz desse repositório.

Após compilado, rode `make run` para inicializar o jogo. 

Em execução, o jogo lhe perguntará a quantidade de bots que você gostaria de jogar contra. Digite um número entre `1` e `5`. Escolhido a quantidade de adversários, o Connect Four iniciará em seu turno de jogada. 

```
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
```

Para jogar sua peça, escolha a coluna (`1` a `7`) e será posicionado a sua peça. Após jogado, cada bot fará sua jogada e o jogo continuará até você (ou um bot) ganhar.

#### Como ganhar

O objetivo do Connect Four é ter quatro peças juntas em uma mesma direção. Ou seja, 4 peças conectadas em uma linha, coluna ou diagonal, sem ter nenhum espaço livre ou peça adversária nessa conexão.

Segue um exemplo de tabuleiro vitorioso para o jogador `$`:

```
    1   2   3   4   5   6   7 
  +---+---+---+---+---+---+---+
  | * | * | # | * | * | * | * |
  +---+---+---+---+---+---+---+
  | * | * | @ | % | * | * | * |
  +---+---+---+---+---+---+---+
  | * | * | % | # | * | * | * |
  +---+---+---+---+---+---+---+
  | * | * | # | @ | * | * | * |
  +---+---+---+---+---+---+---+
  | * | * | @ | % | * | * | * |
  +---+---+---+---+---+---+---+
  | * | * | $ | $ | $ | $ | * |
  +---+---+---+---+---+---+---+
```

## Implementação

O jogo utiliza de thread e semáforos para seu funcionamento correto. As threads estão relacionadas aos jogadores (você e os bots), em que cada jogador possui uma thread. Para garantir que não haja acesso inadequado em uma região crítica (nesse caso o tabuleiro) é utilizado semáforos para garantir acesso único por vez. 

### Threads

O uso das threads é feito pela biblioteca padrão `thread`. Nesse jogo, há uma thread para cada jogador. A thread do usuário está relacionado à função `jogador`, enquanto cada bot está relacionado a função `bot`. Nas threads dos bots, é passado o index `i` para diferenciar os `n` bots. 

```c++
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
```

### Semáforos

O uso dos semáforos é por meio da biblioteca padrão `semaphore`. Como há duas partes por rodada (a sua e a dos bots), o jogo é implementado com um semáforo para cada tipo de jogador.

```c++
binary_semaphore semaforo_jogador(1);
vector<unique_ptr<binary_semaphore>> semaforos_bots;
```


#### Semáforo jogador

A inicialização acima permite, inicialmente, que o jogador acesse a região crítica. Quando a thread chamar a função `jogador`, o jogador entra na região crítica e modifica o tabuleiro com a jogada do usuário.

Após a jogada, é verificado se algum caso de fim de jogo foi atingido. Se chegou ao fim, libera os semáforos dos bots para eles também encerrarem o loop do jogo.

Por fim, caso ainda tenha rodadas a serem jogadas, libera os semáforos dos bots para eles realizarem as suas devidas ações.

Para permitir que os bots joguem em sequência, apenas o semáforo do `bot_0` é liberado. Os outros bots terão seu respectivo semáforo liberado na função `bot`.

```c++
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
        
        // ...modifica a tabela
        
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
```
#### Semáforos bots

Os semáforos para os bots funcionam de maneira similar ao do jogador. Para cada um dos `n` bots e quando for sua vez, entra na região crítica, modifica o tabuleiro com a sua jogada, e verifica se chegou ao fim, caso verdadeiro, libera o semáforo do próximo jogador para encerrar o Connect Four. 

Nessa verificação, checa-se se algum bot ainda não jogou, ou seja, índice do bot atual é `i < n - 1`. Se nem todos jogaram, libera o semáforo do bot seguinte, até que todos tenham jogado. Quando isso acontecer, libera o semáforo do jogador para passar o turno para o usuário.

Se o jogo ainda continuar, libera o semáforo do jogador para ele fazer a sua jogada.

```c++
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


        // ...modifica a tabela

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
```


## Integrantes

...