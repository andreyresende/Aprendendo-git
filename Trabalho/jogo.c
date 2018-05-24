/* Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programacao de Computadores – 1/2018
Aluno(a): Andrey Calaca Resende
Matricula: 180062433
Turma: A
Versao do compilador: gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.9)
Descricao: O jogo acontece na ordem definida pela funcao Jogo, que e chamada pela Main, como explicado no comentario abaixo, as funcoes nao estao na melhor ordem para o entendimento, e sim para melhor compilacao, recomendo a leitura/correcao seguindo a ordem em que aparecem sendo chamadas pela funcao Jogo, mantendo sempre as variaveis globais em mente. */
/*O codigo comeca de fato na linha 70, ate la sao linhas definindo especificacoes do trabalho.*/
/* Detalhe na organizacao das funcoes: A ordem delas pode parecer confusa mas o objetivo dessa ordem e tirar os Warnings de 'Implicit Declaration' do terminal ao compilar */
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
void welcome(){//Da as boas-vindas ao player.
    int n;
    printf("Bem-Vindo ao River Raid, C Version =D.\nDigite 1 para continuar\n");
    scanf("%d",&n);
    while(n != 1){
        printf("Digite 1 e enter para continuar:\n");
        scanf("%d",&n);
    }
}
void Opcao(int escolheu){//Funcao utilizada pela funcao Menu, no momento so funciona para instrucoes. Colocada antes no codigo para evitar Warnings no terminal, ja que se ela for chamada por uma funcao antes de ser declarada ele reclama com "Implicit Declaration".
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
void Menu(){//Basicamente da as opcoes, chamando a devida funcao, recusando comandos invalidos e limpando a tela
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
void colidiu(){//Procura o personagem e verifica se houve uma colisao
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
void movertiro(){//Move o tiro ne...e chamada a cada frame pela funcao Arraste
    int i, j;
    for(i=0;i<9;i++){
        for(j=134;j>0;j--){
            if(tabuleiro[i][j] == '>'){
                tabuleiro[i][j+2] = '>';//+2 pra poder vencer o arraste, uma vez que a funcao tambem esta afetando o tiro
                tabuleiro[i][j+1] = ' ';//Como o tiro esta pulando de 2 em 2 isso evita que ele pule o alvo e nao acerte
                tabuleiro[i][j] = ' ';
            }
        }
    }
}
void Arraste(){//Arrasta a matriz para a esquerda e monitora o funcionamento dos tiros.
    int i, j;
    movertiro();
    for(i=0;i<10;i++){
        for(j=0;j<134;j++){
            if(tabuleiro[i][j] != '+'){
                tabuleiro[i][j]=tabuleiro[i][j+1];
                tabuleiro[i][j+1]=' ';
            }
        }
    }
}
void show(){//printa o mapa na tela, e chamada a cada loop, depois da Arraste.
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
void QueTiroFoiEsse(){//Cria os tiros =D, e chamada no final da funcao Mover.
    int i=0;
    for(i=0;tabuleiro[i][0] != '+';i++){
        if(tabuleiro[i+1][0] == '+'){
            tabuleiro[i+1][1] = '>';
            combustivel -= 2;//Note que o tiro retira 3 unidades de combustivel, pois a funcao jogo ja tira 1 a cada 'frame'.
        }
    }
}
void Mover(){//Move o personagem. Identifica quando uma tecla e pressionada e qual, depois procura o + e move.
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
            combustivel--;//Note que como a cada 'frame' um combustivel ja e retirado pela funcao Jogo, a acao de mover so precisa retirar mais 1 para que se tire 2 naquele frame.
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
        else if(apertou == 'd' || apertou == 'D'){
            QueTiroFoiEsse();
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
    srand(time(0));//especificado pelo trabalho
    CLEAR;
    welcome();
    CLEAR;
    Menu();
    Jogo();
    return 0;
}