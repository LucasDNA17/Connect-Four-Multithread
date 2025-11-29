#include "jogabilidade.h"
#include <iostream>
#include <string>
#include <limits> 

using namespace std;


void desenha_menu() 
{
    cout << "\n";
    cout << "  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-.  .-. \n";
    cout << " / C \\/ O \\/ N \\/ N \\/ E \\/ C \\/ T \\/   \\/ F \\/ O \\/ U \\/ R \\ \n";
    cout << " |   ||   ||   ||   ||   ||   ||   |\\   /|   ||   ||   ||   |\n";
    cout << " \\---/\\---/\\---/\\---/\\---/\\---/\\---/  \\-/ \\---/\\---/\\---/\\---/\n";
    cout << "\n";

    cout << "     **Pressione **ENTER** para começar...**" << endl;    
    cin.get(); 
    cout << "\n";
}


int quantidade_bots()
{
    int n_bots;
    cout << "Contra quantos bots você quer jogar? (1 - 5)" << endl;
    
    do
    {
        cin >> n_bots;
        if(n_bots < 0) cout << "Deve haver pelo menos um bot! Tente de novo!" << endl;
        if(n_bots > 5) cout << "Deve haver no máximo cinco bots! Tente de novo!" << endl;
    } while(n_bots < 0 || n_bots > 5);
    
    return n_bots;
}
