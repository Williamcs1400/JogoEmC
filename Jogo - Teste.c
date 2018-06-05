#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#ifndef _WIN32

	#include <termios.h>
	#include <fcntl.h>

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
#ifdef _WIN32
	#define CLEAR "cls"
	#define ENTER 13
#else
	#define CLEAR "clear"
	#define ENTER 10
#endif
#define Linhas 10
#define Colunas 135

/*Funções declaradas apenas para tirar os warnings*/
void TelasBoasVindas();
void PrintarMatriz();
int usleep();

/*Função para printat game over quando o jogador dejesar sair ou perder*/
void TelaGameOver(){
    printf("#######################################################################################################################################################\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                              _______      ___      .___  ___.  _______      ______   ____    ____  _______ .______                                  #\n");
    printf("#                             /  _____|    /   \\     |   \\/   | |   ____|    /  __  \\  \\   \\  /   / |   ____||   _  \\                           #\n");
    printf("#                            |  |  __     /  ^  \\    |  \\  /  | |  |__      |  |  |  |  \\   \\/   /  |  |__   |  |_)  |                            #\n");
    printf("#                            |  | |_ |   /  /_\\  \\   |  |\\/|  | |   __|     |  |  |  |   \\      /   |   __|  |      /                             #\n");
    printf("#                            |  |__| |  /  _____  \\  |  |  |  | |  |____    |  `--'  |    \\    /    |  |____ |  |\\  \\----.                        #\n");
    printf("#                             \\______| /__/     \\__\\ |__|  |__| |_______|    \\______/      \\__/     |_______|| _| `._____|                       #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#                                                                                                                                                     #\n");
    printf("#######################################################################################################################################################\n");

}

/*Se o usuário escolher, será mostrado as instruções do jogo */
void MostraInstrucoes(char Jogo[Linhas][Colunas]){
    system(CLEAR);
    int Tecla, i;
    printf("                             Os comandos sao:                              \n");
    printf("    No jogo os controles sao basicamente ""W"" para se movimentar para cima \n");
    printf("                    e ""s"" para se movimentar para baixo.                    \n");
    printf("    Dentro do jogo o objetivo eh conseguir o maior numero de pontos antes que\n");
    printf("                            seu combustivel acabe.                            \n");
    printf("                   Para sair do jogo basta apertar backspace.                   \n");
    printf("                             RUN, PIXEL, RUN!                                  \n");
    printf("                    Pressione ENTER para voltar a tela inicial\n               ");
    /*Esse ciclo é para que o usuário consiga entrar e sair das instruções várias vezes*/
    for(i=0; i<100; i++){
        Tecla=getch();
        if(Tecla==ENTER){
            TelasBoasVindas(Jogo);
        }
    }
}

/*Gera números aleatórios para gerar inimigos e combustivel*/
void GerarValoresAletorios(){
    int i, NumerosAleatorios[5];
    srand(time(NULL));
    for(i=0; i<5; i++){
        NumerosAleatorios[i]=rand()%10+3;
    }
}

/*Recebe o comando da função Movimento e faz mover o tiro*/
void MovimentarTiro(char Jogo[Linhas][Colunas]){
    int i, j;
    for(i=0; i<Linhas; i++){
        for(j=Colunas-1; j>0; j--){
            if(Jogo[i][j]=='>'){
                Jogo[i][j]=' ';
                Jogo[i][j+1]='>';
            }
        }
    }
    for(i=0; i<Linhas; i++){
        for(j=Colunas; j==Colunas; j++){
            if(Jogo[i][j]=='>'){
                Jogo[i][j]=' ';
            }
        }
    }
}

/*Essa função printa e limpa a tela constantemente, dando sensação de movimento */
void Movimento(char Jogo[Linhas][Colunas]){
    int comando, i, j, flag_baixo, flag_cima, mover;
    /*Enquanto a entrada for diferende de backspace(8) ele ficará no loop*/
    while(comando!=8){
        usleep(50000);
        mover=kbhit();
        if(mover==1){
            comando=getch();
            /* */
            flag_baixo=0;
            flag_cima=0;
            if(comando==8){
                TelaGameOver(Jogo);
            }
            /*Movimento para cima*/
            if(comando==119){
                for(i=0; i<Linhas; i++){
                        if((Jogo[i][0]=='+')&&(i>2)&&(flag_cima!=1)){
                            Jogo[i][0]=' ';
                            flag_cima=1;
                            Jogo[i-1][0]='+';
                    }
                }
            }
            /*Movimento para baixo*/
            if(comando==115){
                for(i=0; i<Linhas; i++){
                        if((Jogo[i][0]=='+')&&(i<8)&&(flag_baixo!=1)){
                            Jogo[i][0]=' ';
                            flag_baixo=1;
                            Jogo[i+1][0]='+';
                    }
                }
            }
            /*Disparar tiro*/
            if(comando==32){
                for(i=0; i<Linhas; i++){
                    for(j=0; j<Colunas; j++){
                        if(Jogo[i][j]=='+'){
                            Jogo[i][j+1]='>';
                            MovimentarTiro(Jogo);
                        }
                    }
                }
            }
            PrintarMatriz(Jogo);
        }
        else{
            MovimentarTiro(Jogo);
            PrintarMatriz(Jogo);
        }
    }
}

void PrintarMatriz(char Jogo[Linhas][Colunas]){
    int i, j;
    system(CLEAR);
    for(i=0; i<Linhas; i++){
        for(j=0; j<Colunas; j++){
            printf("%c", Jogo[i][j]);
        }
        printf("\n");
    }
    Movimento(Jogo);

}

/*Essa função desenha a matriz, armazena e a printa*/
void EscreverMatriz(char Jogo[Linhas][Colunas]){
    int i, j;
    /*Printa a posição inicial da matriz*/
    for(i=0; i<Linhas; i++){
        for(j=0; j<Colunas; j++){
            Jogo[i][j]=' ';
            if((i==1)||(i==9)){
                Jogo[i][j]='#';
            }
            if((i==5)&&(j==0)){
                Jogo[i][j]='+';
            }
        }
    }
    PrintarMatriz(Jogo);
}

/*Escolher entre as opcoes da tela de boas vindas*/
void EscolherOpcoes(char Jogo[Linhas][Colunas]){
    int opcao;
    scanf("%d", &opcao);
    if(opcao==1){
       EscreverMatriz(Jogo);
    }
    if(opcao==2){
        MostraInstrucoes(Jogo);
    }
}

/*Mostra a tela de Boas Vindas e apresenta as opções para o usuário escolher*/
void TelasBoasVindas(char Jogo[Linhas][Colunas]){
        system(CLEAR);
        int opcao;
        printf("#######################################################################################################################################################\n");
        printf("#                                                                                                                                                     #\n");
        printf("#                                                                                                                                                     #\n");
        printf("#                                                                                                                                                     #\n");
        printf("#                              ____                            ____                                  ____                    _                        #\n");
        printf("#                             |  _ \\   _   _   _ __           |  _ \\  (_) __  __   ___  | |         |  _ \\   _   _   _ __   | |                       #\n");
        printf("#                             | |_) | | | | | | '_ \\          | |_) | | | \\ \\/ /  / _ \\ | |         | |_) | | | | | | '_ \\  | |                       #\n");
        printf("#                             |  _ <  | |_| | | | | |  _      |  __/  | |  >  <  |  __/ | |  _      |  _ <  | |_| | | | | | |_|                       #\n");
        printf("#                             |_| \\_\\  \\__,_| |_| |_| ( )     |_|     |_| /_/\\_\\  \\___| |_| ( )     |_| \\_\\  \\__,_| |_| |_| (_)                       #\n");
        printf("#                                                     |/                                    |/                                                        #\n");
        printf("#                                                                      1 - Jogar                                                                      #\n");
        printf("#                                                                      2 - Instrucoes                                                                 #\n");
        printf("#                                                                   Digite a opcao desejada                                                           #\n");
        printf("#                                                                                                                                                     #\n");
        printf("#                                                                                                                                                     #\n");
        printf("#                                                                                                                                                     #\n");
        printf("#######################################################################################################################################################\n");
        EscolherOpcoes(Jogo);
}


int main (){
    char Jogo[Linhas][Colunas];
    TelasBoasVindas(Jogo);
    return 0;
}
