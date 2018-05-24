/*O código começa de fato na linha 62, até lá são linhas definindo especificações do trabalho.*/
/* Detalhe na organização das funções: A ordem delas pode parecer confusa mas o objetivo dessa ordem é tirar os Warnings de 'Implicit Declaration' do terminal ao compilar */
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
int pontuacao = 1;
int colisao = 0;

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
void welcome(){//Dá as boas-vindas ao player.
    int n;
    printf("Bem-Vindo ao River Raid, C Version =D.\nDigite 1 para continuar\n");
    scanf("%d",&n);
    while(n != 1){
        printf("Digite 1 e enter para continuar:\n");
        scanf("%d",&n);
    }
}
void Opcao(int escolheu){//Funcao utilizada pela funcao Menu, no momento só funciona para instruções. Colocada antes no código para evitar Warnings no terminal, já que se ela for chamada por uma função antes de ser declarada ele reclama com "Implicit Declaration".
    int jogar=0;
    switch (escolheu){
        case 2 ://Implementar depois
            break;
        case 3 ://Implementar depois
            break;
        case 4 :
            CLEAR;
            printf("Ola, Bem vindo ao River C.\n");
            printf("Comandos: seta para cima ou w(move o personagem para cima), seta para baixo ou s(move o personagem para baixo)\n");
            printf("ATENCAO: Apertar alguma tecla que nao seja um comando subtrai 2 de combustivel.\n");
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
void map(){//define o mapa e as bordas
    int i;
    for(i=0;i<10;i++){
        for(int j=0;j<135;j++){
            tabuleiro[i][j] = ' ';
            if(i==5 && j==0 ){
                tabuleiro[i][j] = '+';
            }
            if(i==0){
                borda[i][j] = '#';
            }
            if(i==5 && j==134){
                tabuleiro[i][j]= 'F';
            }
        }
    }
}
void colidiu(){//Procura o personagem e verifica se houve uma colisão
    int i=0;
    while(tabuleiro[i][0] != '+'){
        if(tabuleiro[i+1][0] == '+'){
            if(tabuleiro[i+1][1] == 'X'){
                colisao++;
            }
            else if(tabuleiro[i+1][1] == 'F'){
                combustivel += 40;
            }
        }
        i++;
    }
}
void Arraste(){//Arrasta a matriz para a esquerda e monitora o funcionamento dos tiros.
    int i, j;
    for(i=0;i<9;i++){
        for(j=0;j<134;j++){
            if(tabuleiro[i][j] != '+'){
                tabuleiro[i][j]=tabuleiro[i][j+1];
                tabuleiro[i][j+1]=' ';
            }
        }
    }
}
void show(){//printa o mapa na tela, é chamada a cada loop, depois da Arraste.
    int i, j;
    printf("Combustivel: %d               Pontos: %d\n",combustivel,pontuacao);
    for(i=0;i<135;i++){
        printf("%c",borda[0][i]);
    }
    printf("\n");
    for(i=0;i<10;i++){
        for(j=0;j<135;j++){
            printf("%c",tabuleiro[i][j]);
        }
        printf("\n");
    }
    for(i=0;i<135;i++){
        printf("%c",borda[0][i]);
    }
    printf("\n");
}
void Mover(){//Move o personagem. Identifica quando uma tecla é pressionada e qual, depois procura o + e move.
    int i=0;
    char apertou;
    if(kbhit() == 1){
        apertou = getch();
        if((apertou == 'w'|| apertou == 'W') && tabuleiro[0][0] != '+'){//Se apertou W ele move pra cima
            while(tabuleiro[i][0] != '+'){
                if(tabuleiro[i+1][0] == '+'){
                    tabuleiro[i][0] = '+';
                }
                i++;
            }
            tabuleiro[i][0] = ' ';
            combustivel--;//Note que como a cada 'frame' um combustível já é retirado pela função Jogo, a ação de mover só precisa retirar mais 1 para que se tire 2 naquele frame.
        }
        else if((apertou == 's' || apertou == 'S') && tabuleiro[0][0] == '+'){
            tabuleiro[0][0] = ' ';
            tabuleiro[1][0] = '+';
        }
        else if((apertou == 's' || apertou == 'S') && tabuleiro[9][0] != '+'){//Se apertou S ele move pra baixo
            while(tabuleiro[i][0] != '+'){
                if(tabuleiro[i+1][0] == '+'){
                    tabuleiro[i+2][0] = '+';
                }
                i++;
            }
            tabuleiro[i][0] = ' ';
            combustivel--;
        }
        else{
            combustivel -= 2;//Punição, tira 2 de combustível extra por apertar uma tecla que não é um comando.
        }
    }
}
void QueTiroFoiEsse(){//Cria os tiros =D
    int i=0, j=0;
    char apertou;
    if(kbhit() == 1){
        apertou == getch();
        if(apertou == 'd' || apertou == 'D'){
            if(tabuleiro[0][0] == '+'){
                tabuleiro[0][1] == '>';
                combustivel -= 2;//Note que a cada "frame" um combustível já é retirado, portanto quando se atira, para perder 3, aqui só é necessário colocar 2.
            }
            else{
                while(tabuleiro[i][0] != '+'){
                    if(tabuleiro[i][0] == '+'){
                        tabuleiro[i][1] = '>';
                        combustivel -= 2;
                    }
                }
            }
        }
    }
}
void Jogo(){//Realiza o jogo de fato
    map();//Monta o mapa
    while(colisao == 0 && combustivel > 0){
        CLEAR;
        colidiu();
        Arraste();
        Mover();
        show();
        usleep(50000);
        pontuacao++;
        combustivel--;
    }
    CLEAR;
    printf("GAME OVER\n");
    printf("Pontuacao: %d\n",pontuacao );
}
int main(){
    srand(time(0));
    CLEAR;
    welcome();
    CLEAR;
    Menu();
    Jogo();
    return 0;
}