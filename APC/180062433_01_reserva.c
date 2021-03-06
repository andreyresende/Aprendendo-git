/* Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programacao de Computadores – 1/2018
Aluno(a): Andrey Calaca Resende
Matricula: 180062433
Turma: A
Versao do compilador: gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.9)
Descricao: O jogo acontece na ordem definida pela funcao jogo, que e chamada pela Main, como explicado no comentario abaixo, as funcoes nao estao na melhor ordem para o entendimento, e sim para melhor compilacao, recomendo a leitura/correcao seguindo a ordem em que aparecem sendo chamadas pela funcao jogo, mantendo sempre as variaveis globais em mente. */
/*O codigo comeca de fato na linha 71, ate la sao linhas definindo especificacoes do trabalho.*/
/* Detalhe na organizacao das funcoes: A ordem delas pode parecer confusa mas o objetivo dessa ordem e tirar os Warnings de 'Implicit Declaration' do terminal ao compilar */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define RAND () ((rand()%100 + 1))

char tabuleiro[8][135];
char borda[1][135];
int altura, largura;
int probX, probF;
int velocidade;
int combustivel = 400;
int pontuacao = 1;
int colisao = 0;
int vicio = 1;//Serve para, ao final de cada jogo, verificar se o usuario deseja jogar novamente

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
    char n;
    printf("Bem-Vindo ao River Raid, C Version =D.\nDigite 1 para continuar\n");
    scanf("%c",&n);
    while(n != 1){
        printf("Digite 1 e enter para continuar:\n");
        scanf("%c",&n);
    }
}
void opcao(int escolheu){//Funcao utilizada pela funcao menu, no momento so funciona para instrucoes. Colocada antes no codigo para evitar Warnings no terminal, ja que se ela for chamada por uma funcao antes de ser declarada ele reclama com "Implicit Declaration".
    int jogar=0;
    switch (escolheu){
        case 2 ://Implementar depois
            break;
        case 3 ://Implementar depois
            break;
        case 4 :
            CLEAR;
            printf("Comandos:\nw(move o personagem para cima e gasta 2 de combustivel).\ns(move o personagem para baixo e gasta 2 de combustivel).\nd(atira e gasta 3 de combustivel).\n");
            printf("Voce eh uma nave (+), inimigos(X) virao pela direita, bem como combustivel(F).\nEncostar em um inimigo finaliza o jogo, encostar em um (F) aumenta o seu combustivel em 40 unidades.\n");
            printf("O seu tiro destroi tanto inimigos quanto combustivel, sendo que ao destruir um inimigo 50 ponto sao ganhos, entretanto, destruir um combustivel nao lhe proporciona nenhum beneficio.\n");
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
void menu(){//Basicamente da as opcoes, chamando a devida funcao, recusando comandos invalidos e limpando a tela
    int escolha;
    printf("1 - Jogar\n");
    printf("2 - Configuracoes\n");
    printf("3 - Ranking\n");
    printf("4 - Instrucoes\n");
    printf("5 - Sair\n\n");
    printf("Escolha uma opcao: ");
    scanf("%d",&escolha);
    while(escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4 && escolha != 5){
        printf("opcao invalida, digite uma opcao valida : ");
        scanf("%d",&escolha);
        printf("\n");
    }
    switch (escolha){
            case 1 :
                CLEAR;
                break;
            case 2 :
                opcao(2);//Implementar depois
                break;
            case 3 :
                opcao(3);//Implementar depois
                break;
            case 4 :
                opcao(4);
                break;
            case 5 :
                exit(0);
        }
}
void map(){//define o mapa e as bordas
    int i;
    for(i=0;i<8;i++){
        for(int j=0;j<135;j++){
            tabuleiro[i][j] = ' ';
            if(i==5 && j==0 ){
                tabuleiro[i][j] = '+';
            }
            if(i==0){
                borda[i][j] = '#';
            }
        }
    }
}
void spawn(){
    if((rand()%10+1)<6){//50% de chance de spawn
        if((rand()%10+1)<6){//50% de chance de inimigo
            tabuleiro[rand()%8][134] = 'X';
        }
        else if((rand()%10+1)<6){//50% de chance de inimigo
            tabuleiro[rand()%8][134] = 'F';
        }
    }
}
void colidiu(){//Procura o personagem e verifica se houve uma colisao
    int i=0;
    if(tabuleiro[0][0] == '+' && (tabuleiro[0][1]=='X' || tabuleiro[0][1]=='F')){
        if(tabuleiro[0][1]=='F'){
            combustivel += 40;
            tabuleiro[0][1] = ' ';
        }
        else{
            colisao++;
        }
    }
    while(tabuleiro[i][0] != '+'){
        if(tabuleiro[i+1][0] == '+'){
            if(tabuleiro[i+1][1] == 'X'){
                colisao++;
            }
            else if(tabuleiro[i+1][1] == 'F'){
                combustivel += 40;
                tabuleiro[i+1][1]=' ';
            }
        }
        i++;
    }
}
void moverTiro(){//Move o tiro, eh chamada a cada frame pela funcao jogo
    int i, j;
    for(i=0;i<8;i++){
        for(j=134;j>0;j--){//Procura o tiro
            if(tabuleiro[i][j] == '>'){//Quando acha
                if((tabuleiro[i][j+1]=='F') || (tabuleiro[i][j+1]=='X')){//se tiver alguma coisa na frente zera ambos
                    if(tabuleiro[i][j+1]=='X'){
                        pontuacao+=50;
                    }
                    tabuleiro[i][j+1]=' ';
                    tabuleiro[i][j]=' ';
                }
                else{//senao, o tiro continua
                    tabuleiro[i][j+1]='>';
                    tabuleiro[i][j]=' ';
                }
            }
        }
    }
}
void arraste(){//Arrasta a matriz para a esquerda
    int i, j;
    for(i=0;i<8;i++){
        for(j=0;j<135;j++){
            if((tabuleiro[i][j]=='X' || tabuleiro[i][j]=='F') && tabuleiro[i][j-1] != '+'){
                if(tabuleiro[i][j-1]=='>'){
                    tabuleiro[i][j]=' ';
                    tabuleiro[i][j-1]=' ';
                }
                else{
                    tabuleiro[i][j-1]=tabuleiro[i][j];
                    tabuleiro[i][j]=' ';
                }
            }
        }
    }
}
void show(){//printa o mapa na tela, e chamada a cada loop, depois da arraste.
    int i, j;
    printf("Combustivel: %d               Pontos: %d\n",combustivel,pontuacao);
    for(i=0;i<135;i++){
        printf("%c",borda[0][i]);
    }
    printf("\n");
    for(i=0;i<8;i++){
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
void queTiroFoiEsse(){//Cria os tiros =D, e chamada no final da funcao moverPersonagem.
    int i=0;
    if(tabuleiro[0][0] == '+'){//Caso especial para a primeira linha, pois se deixasse apenas o loop ele nao seria executado 
        if(tabuleiro[0][1]=='X' || tabuleiro[0][1]=='F'){//Caso especifico quando o inimigo esta na casa imediatamente a direita do personagem
            tabuleiro[0][1]=' ';
        }
        else{
            tabuleiro[0][1]='>';
        }
    }
    for(i=0;tabuleiro[i][0] != '+';i++){//Procura o personagem
        if(tabuleiro[i+1][0] == '+'){//Quando acha
            if(tabuleiro[i+1][1]=='X' || tabuleiro[i+1][1]=='F'){
            tabuleiro[i+1][1]=' ';
            }
            else{
            tabuleiro[i+1][1]='>';
            }
        }
    }
    combustivel -= 2;//Note que o tiro retira 3 unidades de combustivel, pois a funcao jogo ja tira 1 a cada 'frame'.
}
void moverPersonagem(){//Move o personagem. Identifica quando uma tecla e pressionada e qual, depois procura o + e move.
    int i=0;
    char apertou;
    if(kbhit() == 1){
        apertou = getch();
        if((apertou == 'w'|| apertou == 'W') && tabuleiro[0][0] != '+'){//Se apertou W ele move pra cima
            while(tabuleiro[i][0] != '+'){//Procura o personagem na coluna
                if(tabuleiro[i+1][0] == '+'){//Quando acha 
                    tabuleiro[i][0] = '+';//Move pra cima
                }
                i++;
            }
            tabuleiro[i][0] = ' ';//Fica fora do while se nao ele troca o personagem de lugar e nunca acha cade ele, ai da problema
            combustivel--;//Note que como a cada 'frame' um combustivel ja e retirado pela funcao jogo, a acao de mover so precisa retirar mais 1 para que se tire 2 naquele frame.
        }
        else if((apertou == 's' || apertou == 'S') && tabuleiro[0][0] == '+'){
            tabuleiro[0][0] = ' ';
            tabuleiro[1][0] = '+';
        }
        else if((apertou == 's' || apertou == 'S') && tabuleiro[7][0] != '+'){//Se apertou S ele move pra baixo
            while(tabuleiro[i][0] != '+'){//mesma coisa do while do W
                if(tabuleiro[i+1][0] == '+'){
                    tabuleiro[i+2][0] = '+';
                }
                i++;
            }
            tabuleiro[i][0] = ' ';
            combustivel--;
        }
        else if(apertou == 'd' || apertou == 'D'){
            queTiroFoiEsse();
        }
    }
}
void jogo(){//Realiza o jogo de fato
    map();//Monta o mapa
    while(colisao == 0 && combustivel > 0){
        CLEAR;
        colidiu();//Eh chamada antes das outras porque a cada frame ele verifica se o proximo char eh um inimigo/F e depois move, assim ele ve se colidiu.
        moverTiro();
        arraste();
        moverPersonagem();
        spawn();
        show();
        usleep(50000);
        pontuacao++;
        combustivel--;
    }
    CLEAR;
    printf("GAME OVER\n");
    printf("Pontuacao: %d\n",pontuacao );
    if(colisao>0){
        printf("Morte por Colisao\n");
    }
    if(combustivel <= 0){
        printf("Sem combustivel\n");
    }
    printf("Deseja jogar novamente?\nDigite 0 para sair, ou 1 para jogar: ");
    vicio = 2;
    while(vicio != 0 && vicio != 1){
        scanf("%d",&vicio);
    }
    CLEAR;
}
int main(){
    while(vicio == 1){
        srand(time(0));//especificado pelo trabalho
        CLEAR;
        welcome();
        CLEAR;
        menu();
        jogo();
        colisao = 0;
        combustivel = 400;
        pontuacao = 0;
    }
    return 0;
}