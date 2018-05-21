/*O código começa de fato na linha 59, até lá são linhas definindo especificações do trabalho.*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define RAND () ((rand()%100 + 1))

char tabuleiro[10][135];
char borda[1][135];
int altura, largura;
int probX, probF;
int velocidade;
int combustivel = 400;

#ifdef _WIN32
    #define CLEAR system("cls");
#else
    #define CLEAR system("clear");
#endif

#ifndef _WIN32
    int kbhit(){
        struct termios oldt, newt;
        int ch, oldf;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if(ch != EOF){
            ungetc(ch,stdin);
            return 1;
        }
        return 0;
    }
    int getch(void) {
        int ch;
        struct termios oldt;
        struct termios newt;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    #else
        #include <conio.h>
#endif

void Menu(){//Basicamente da as opcoes, chamando a devida função, recusando comandos invalidos e limpando a tela
    int escolha;
    printf("1 - Jogar\n");
    printf("2 - Configuracoes\n");
    printf("3 - Ranking\n");
    printf("4 - Instrucoes\n");
    printf("5 - Sair\n\n");
    printf("Escolha uma opcao: ");
    scanf("%d",&escolha);
    while(escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4 && escolha != 5){
        printf("Opcao invalida, digite uma opcao valida : ");
        scanf("%d",&escolha);
        printf("\n");
    }
    switch (escolha){
            case 1 :
                CLEAR;
                break;
            case 2 :
                Opcao(2);//Implementar depois
                break;
            case 3 :
                Opcao(3);//Implementar depois
                break;
            case 4 :
                Opcao(4);
                break;
            case 5 :
                exit(0);
        }
}
void Opcao(int escolheu){//Funcao utilizada pela funcao Menu, no momento só funciona para instruções.
    int jogar=0;
    switch (escolheu){
        case 2 ://Implementar depois
            break;
        case 3 ://Implementar depois
            break;
        case 4 :
            CLEAR;
            printf("Ola, Bem vindo ao River C.\n");
            printf("Comandos: seta para cima ou w(move o personagem para cima), seta para baixo ou d(move o personagem para baixo\n");
            printf("Digite 1 para jogar, ou 2 para sair: ");
            scanf("%d",&jogar);
            printf("\n");
            while(jogar != 1 && jogar != 2){
                printf("Valor invalido, digite 1 para jogar, ou 2 para sair: ");
                scanf("%d",&jogar);
                printf("\n");
            }
            if(jogar==2){
                exit(0);
            }
            else{
                CLEAR;
                break;
            }
    }
}
void map(char tabuleiro[10][135]){//define o mapa e as bordas
    int i;
    for(i=0;i<9;i++){
        for(int j=0;j<134;j++){
            tabuleiro[i][j] = '.';
            if(i==5 && j==0 ){
                tabuleiro[i][j] = '+';
            }
            if(i==0){
                borda[i][j] = '#';
            }
        }
    }
}
void Arraste(char tabuleiro[10][135],int* colisao){//Arrasta a matriz para a esquerda
    int i, j;
    char aux;
    for(i=0;i<9;i++){
        for(j=0;j<134;j++){
            if(tabuleiro[i][0]=='+'){//Quando encontrar o personagem a função verifica se ele encontrou um X ou um F.
                if(tabuleiro[i][1]=='X'){
                    colisao++;
                }
                else if(tabuleiro[i][1]=='F'){//Caso encontre um F ele aumenta o combustivel e some com o F
                    combustivel += 40;
                    tabuleiro[i][1]='.';
                }
            }
            else{//Não tendo encontrado nada a função simplesmente arrasta a matriz para a esquerda
                tabuleiro[i][j]=tabuleiro[i][j+1];
                tabuleiro[i][j+1]='.';
            }
        }
    }
}
void Jogo(char tabuleiro[10][135]){//Realiza o jogo de fato
    int colisao=0, pontuacao=0;
    map(tabuleiro[10][135]);
    while(colisao==0 && combustivel>0){
        Arraste(tabuleiro[10][135],&colisao);
    }
    printf("GAME OVER\n");
    printf("%d\n",pontuacao );
}
int main(){
    srand(time(0));
    CLEAR;
    Menu();
    Jogo(tabuleiro);
    return 0;
}