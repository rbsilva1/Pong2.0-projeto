#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ALTURA_TELA 720
#define LARGURA_TELA 1280
#define ALTURA_JOGADOR 90
#define LARGURA_JOGADOR 10

typedef struct Jogador1{
    Vector2 posicao;
} Jogador1;

typedef struct Jogador2{
    Vector2 posicao;
} Jogador2;

typedef struct Bola{
    Vector2 posicao;
    Vector2 velocidade;
    int raio;
    bool atividade;
} Bola;

typedef struct 
{
    char nickVencedor[5];
    int gol1, gol2;
} Historico;

typedef struct nickVencedor
{
    char nickVencedor;
    struct nickVencedor *proximo;
} nickVencedor;

typedef struct 
{
    int gol1;
    int gol2;
} Gol;

void menuInicial(int, int*, int, int*, int*, int*, int*);
void posicaoMenu(int*, int*, int*, int*);
void comecarJogo();
bool atualizacaoDeQuadros(bool*, int*, int*, int*, Sound*, Sound*, int*, int*, int);
void fimDeJogo(int*, int*, int*, int*, int*, int*);
void desenharJogo(int, int, Texture2D);
void lerHistorico();
void salvarHistorico();
char* imprimirNickJogador(nickVencedor*);
int addInPos(nickVencedor**, nickVencedor, int);
int delFinal(nickVencedor**);
nickVencedor montaStruct(char);
int randomizer(int x);
Gol reiniciarJogoAnterior(int);

Historico historico[5] = { 0 };
static Jogador1 jogador1 = { 0 };
static Jogador2 jogador2 = { 0 };
static Bola bola = { 0 };
nickVencedor *lista = NULL;

// Salva infos do jogador e o placar no arquivo
void salvarHistorico() {
    FILE* historicoArquivo = fopen("resources/historico.csv", "w+");
    if (historicoArquivo == NULL) return;

    for (int i = 0; i < 5; i++) {
        fprintf(historicoArquivo, "%s;%i;%i\n", historico[i].nickVencedor, historico[i].gol1, historico[i].gol2);
    }
    
    fclose(historicoArquivo);
}

// Pega a cabeça da lista dinamica de char do nome do jogador e imprime na tela
char* imprimirNickJogador(nickVencedor *inicio){
    nickVencedor *noAtual = inicio; 
    char *nome = calloc(5, sizeof(char));

    // Caminha ate o final
    while (noAtual != NULL){
        size_t tamanho = strlen(nome);
        nome[tamanho] = noAtual->nickVencedor;
        nome[tamanho + 1] = '\0';
        noAtual = noAtual->proximo;
    }  

    return nome;
}

// Cria uma struct com o placar da ultima partida jogada
Gol reiniciarJogoAnterior(int continuar) {
    Gol gols = (Gol) { 0, 0 };
    
    if (continuar) {
        gols = (Gol){ historico[0].gol1, historico[0].gol2 };
        return gols;  
    } else {
        return gols;
    }
}

void menuInicial(int estaNosRecordes, int* estaNoJogo, int selecaoMenu, int* continuar, int* golJogador1, int* golJogador2, int* maximoGols){
    if (!estaNosRecordes && !*continuar) { // Verifica se o jogador está na aba de HISTORICO ou CONTINUAR
        DrawText("Pong", LARGURA_TELA / 2 - MeasureText("Pong", 70) / 2, 80, 70, DARKBLUE);

        if (selecaoMenu == 0) {
            DrawText("- Jogar -", LARGURA_TELA / 2 - MeasureText("- Jogar -", 55) / 2, 300, 55, WHITE);
            DrawText("Recordes", LARGURA_TELA / 2 - MeasureText("Recordes", 50) / 2, 400, 50, WHITE);
            DrawText("Continuar", LARGURA_TELA / 2 - MeasureText("Continuar", 50) / 2, 500, 50, WHITE);
        } else if (selecaoMenu == 1){
            DrawText("Jogar", LARGURA_TELA / 2 - MeasureText("Jogar", 50) / 2, 300, 50, WHITE);
            DrawText("- Recordes -", LARGURA_TELA / 2 - MeasureText("- Recordes -", 55) / 2, 400, 55, WHITE);
            DrawText("Continuar", LARGURA_TELA / 2 - MeasureText("Continuar", 50) / 2, 500, 50, WHITE);
        } else {
            DrawText("Jogar", LARGURA_TELA / 2 - MeasureText("Jogar", 50) / 2, 300, 50, WHITE);
            DrawText("Recordes", LARGURA_TELA / 2 - MeasureText("Recordes", 50) / 2, 400, 50, WHITE);
            DrawText("- Continuar -", LARGURA_TELA / 2 - MeasureText("- Continuar -", 55) / 2, 500, 55, WHITE);
        }
    } else if (estaNosRecordes) { // Se estiver no HISTORICO, vai printa-lo na tela
        for (int i = 0; i < 5; ++i) {
            char temp[16];
            strcpy(temp, TextFormat("%s        %i X %i", historico[i].nickVencedor, historico[i].gol1, historico[i].gol2));
            DrawText(temp, LARGURA_TELA / 2 - MeasureText(temp, 50) / 2, i * 100 + 50, 50, WHITE);
        }

        DrawText("- Voltar -", LARGURA_TELA / 2 - MeasureText("- Voltar -", 50) / 2, 520, 50, WHITE);
    } else { // Se estiver na aba CONTINUAR, retorna ao último jogo.
        Gol retorno = reiniciarJogoAnterior(*continuar);
        if (retorno.gol1 != 0 || retorno.gol2 != 0) {
            *golJogador1 = retorno.gol1;
            *golJogador2 = retorno.gol2;
            *maximoGols *= 2; 
            *continuar = 0;
            *estaNoJogo = 1;
        } 
    }
}

// Dependendo da escolha do usuário ele define se ele está no jogo ou nos recordes e inicia algum deles
void posicaoMenu(int* selecaoMenu, int* estaNosRecordes, int* estaNoJogo, int* continuar) {
    if (!(*estaNosRecordes) && !(*continuar)) {
        // Funcionalidade de escolha no menu alternando entre seta para baixo (KEY_DOWN) e para cima (KEY_UP)
        if (IsKeyPressed(KEY_DOWN) && *selecaoMenu == 0) *selecaoMenu = 1;
        else if (IsKeyPressed(KEY_DOWN) && *selecaoMenu == 1) *selecaoMenu = 2;
        else if (IsKeyPressed(KEY_DOWN) && *selecaoMenu == 2) *selecaoMenu = 0;
        else if (IsKeyPressed(KEY_UP) && *selecaoMenu == 0) *selecaoMenu = 2;
        else if (IsKeyPressed(KEY_UP) && *selecaoMenu == 1) *selecaoMenu = 0;
        else if (IsKeyPressed(KEY_UP) && *selecaoMenu == 2) *selecaoMenu = 1;
        
        if (IsKeyPressed(KEY_ENTER)) {
            if (*selecaoMenu == 0) *estaNoJogo = 1;
            else if (*selecaoMenu == 1) *estaNosRecordes = 1;
            else *continuar = 1;
        }
    } else if (*estaNosRecordes){
        if (IsKeyPressed(KEY_ENTER)) *estaNosRecordes = 0;
    } else {
        if (IsKeyPressed(KEY_ENTER)) *continuar = 0;
    }
}

// Monta uma struct do tipo nickVencedor com a uma nova letra;
nickVencedor montaStruct(char novaLetra) {
    nickVencedor novaLetraStruct;

    novaLetraStruct.nickVencedor = novaLetra;
    return novaLetraStruct;
}

// Adiciona a letra numa posição indicada da lista
int addInPos(nickVencedor **inicio, nickVencedor item, int posicao){
    nickVencedor *novoNo = calloc(5, sizeof(nickVencedor));

    if (novoNo != NULL){ //testa se memoria foi alocada
        *novoNo = item;
        novoNo->proximo = NULL;

        //caso 1: lista vazia
        if (*inicio == NULL) {
            *inicio = novoNo;
        } else { //caso 2: lista jah contem pelo menos 1 elemento
            nickVencedor *noAtual = NULL, *noAnterior = NULL; 
            int cont = 0;

            //caminha ate a posicao desejada 
            //(nao estah validando se posicao é valida)
            noAtual = *inicio;
            noAnterior = *inicio;

            while (cont < posicao){
                noAnterior = noAtual;
                noAtual = noAtual->proximo;
                cont++;
            }

            if (noAnterior != noAtual){ //testa se é inserção no início
                //insere elemento entre no anterior e no atual
                noAnterior->proximo = novoNo;
                novoNo->proximo = noAtual;
            } else { //insercao no inicio (do contrario, criaria um loop)
                novoNo->proximo = *inicio;
                *inicio = novoNo;          
            }
        }

        return 1; //sucesso
    } else {
        return 0; //sem memoria
    }    
}

// Deleta a ultima letra da lista a cada chamada
int delFinal(nickVencedor **inicio){
    if (*inicio != NULL) {
        if ((*inicio)->proximo == NULL) { //lista com apenas 1 elemento
            free(*inicio); //libera memoria

            *inicio = NULL; //atualiza o ponteiro de inicio da lista
        } else {
            nickVencedor *noAtual = *inicio; 
            nickVencedor *noAnterior = *inicio; 

            //caminha ate o final
            while (noAtual->proximo != NULL){
                noAnterior = noAtual;
                noAtual = noAtual->proximo;
            } 

            free(noAtual); //libera memoria do ultimo
            noAnterior->proximo = NULL; //atualiza ponteiro do nó que agora é o último
        }

        return 1;
    } else {
        return 0;
    }
}

void fimDeJogo(int* estaNoJogo, int* estaNosRecordes, int* continuar, int* nickCount, int* golJogador1, int* golJogador2) {
    // Desenha o fim do jogo e a partir da lista do Nick do vencedor vai mostrando o Nick a medida em que o usuario digita.
    DrawText("Fim de Jogo", LARGURA_TELA / 2 - MeasureText("Fim de Jogo", 50) / 2, ALTURA_TELA / 2 - 50, 50, DARKBLUE);    
    DrawText(imprimirNickJogador(lista), LARGURA_TELA / 2 - MeasureText(imprimirNickJogador(lista), 50) / 2, ALTURA_TELA / 2 + 50, 50, DARKBLUE);
    
    int key = GetCharPressed();

    // Analisa se a letra está dentro de um intervalo na ASCII table e, também, verifica se o nick já não está completo
    if (key >= 65 && key <= 125 && *nickCount < 5) {
        addInPos(&lista, montaStruct((char) key), *nickCount); // Adiciona na ultima posição do nick
        *nickCount += 1; // Atualiza a ultima posição do nick
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        *nickCount -= 1; // Diminui o nickCount
        if (*nickCount < 0) *nickCount = 0;
        delFinal(&lista); // Deleta a última letra desta lista
    }

    if (IsKeyPressed(KEY_ENTER) && *nickCount == 5) {
        int pos = -1;
        // Verifica se já tem algum jogador na posição, e passa para a próxima colocando o ultimo para a frente no HISTORICO
        for (int i = 0; i < 5; ++i) {
            if (pos == -1) {
                pos = i;
                break;  
            } else if (historico[i].gol1 != 0 || historico[i].gol2 != 0) {
                pos = i;
                break;  
            }  
        }

        // Cópia da lista e termina de passar o jogador para a última partida no histórico
        if (pos != -1) {
            Historico temp[5] = { historico[0], historico[1], historico[2], historico[3], historico[4] };
            temp[pos].gol1 = *golJogador1;
            temp[pos].gol2 = *golJogador2;
            strcpy(temp[pos].nickVencedor, imprimirNickJogador(lista));

            for (int i = pos + 1; i < 5; ++i) {
                temp[i] = historico[i - 1];
            }

            for (int i = 0; i < 5; ++i) {
                historico[i] = temp[i];
            }
        }


        salvarHistorico();
        
        // Reseta as configs iniciais
        *nickCount = 0;
        *golJogador1 = 0;
        *golJogador2 = 0;
        *estaNoJogo = 0;
        *estaNosRecordes = 0;
        *continuar = 0;

        // Zera a lista para usa-la novamente depois
        for (int i = 0; i < 5; i++) {
            delFinal(&lista);
        }

        comecarJogo();
    }
}

int randomizer(int x) {
    return rand() % x;
}

// Função que inicia as posições dos jogadores e da bola, a velocidade da bola, o tamanho da bola e se ela está ativa
void comecarJogo(){
    jogador1.posicao = (Vector2){50, (ALTURA_TELA - 180) / 2 + (ALTURA_JOGADOR / 2)};
    jogador2.posicao = (Vector2){LARGURA_TELA - 50, (ALTURA_TELA - 180) / 2 + (ALTURA_JOGADOR / 2)};
    bola.posicao.x = LARGURA_TELA / 2;
    bola.posicao.y = ALTURA_TELA / 2 - 10;
    bola.velocidade = (Vector2){0,0};
    bola.raio = 8;
    bola.atividade = false;
}

// função do tipo booleana que retorna true ou false quando atinge o número máximo de gol
bool atualizacaoDeQuadros(bool* pausa, int* mostrarPause, int* con, int* velocidadeJogadores, Sound* GOL, Sound* CONTATO, int* golJogador1, int* golJogador2, int maximoGols){
    // se a tecla "P" do teclado for acionada o jogo pausa e vice e versa
    if(IsKeyPressed(KEY_P) && *mostrarPause == 1) *pausa = !*pausa;

    if (!*pausa) {
        // se a bola não estiver ativa e a tecla "space" for acionada ele inicia os valores de velocidade
        if(!bola.atividade){
            if(IsKeyPressed(KEY_SPACE) && *mostrarPause == 1){
                // muda a variável de atividade para true 
                bola.atividade = true;
                // reinicia o contador de vezes que a bola tocou alguma extremidade
                *con = 0;
                // define a velocidade da bola de forma aleatória
                if(randomizer(10) > 5){
                    bola.velocidade = (Vector2){-5, randomizer(6) + 1};
                    if(randomizer(100) > 50) bola.velocidade.y *= -1;
                } else {
                    bola.velocidade = (Vector2){5, randomizer(6) + 1};
                    if(randomizer(100) > 50) bola.velocidade.y *= -1;
                }
            }
        }

        if(bola.atividade){
            // quando a bola estiver ativa ele muda a posição da bola de acordo com a velocidade dela
            bola.posicao.x += bola.velocidade.x;
            bola.posicao.y += bola.velocidade.y;

            // se a bola ultrapassar alguma das extremidades laterais (esquerda ou direita)
            if(bola.posicao.x + bola.raio >= LARGURA_TELA || bola.posicao.x - bola.raio <= 0){
                // reinicia a posição dos jogadores
                jogador1.posicao = (Vector2){50, (ALTURA_TELA - 180) / 2 + (ALTURA_JOGADOR / 2)};
                jogador2.posicao = (Vector2){LARGURA_TELA - 50, (ALTURA_TELA - 180) / 2 + (ALTURA_JOGADOR / 2)};
                // reinicia a valocidade da bola
                bola.velocidade = (Vector2){0,0}; 
                // muda a a variável 'atividade' para false
                bola.atividade = false;
                // reinicia a velocidade dos jogadores
                *velocidadeJogadores = 5;

                PlaySound(*GOL);

                // adiciona os gols dependendo se a bola passou pela direita ou esquerda
                if(bola.posicao.x + bola.raio >= LARGURA_TELA) *golJogador1 += 1;
                if(bola.posicao.x - bola.raio <= 0) *golJogador2 += 1;
            }

            // se tocar em cima ou em baixo da tela
            if(bola.posicao.y + bola.raio <= 0 || bola.posicao.y + bola.raio >= ALTURA_TELA) {
                // inverte a velocidade da bola para simular uma reflexão
                bola.velocidade.y *= -1;
                PlaySound(*CONTATO);
            }

            // altera a posição dos jogadores dependendo da tecla que for acionada
            if(IsKeyDown(KEY_S)) jogador1.posicao.y += *velocidadeJogadores;
            // se chegar ao limite da tela a posição não é alterada
            if(jogador1.posicao.y + ALTURA_JOGADOR >= ALTURA_TELA) jogador1.posicao.y = ALTURA_TELA - ALTURA_JOGADOR;
            if(IsKeyDown(KEY_W)) jogador1.posicao.y -= *velocidadeJogadores;
            if(jogador1.posicao.y + 1 <= 0) jogador1.posicao.y = 1;

            // verifica se a bola colidiu com algum dos jogadores
            if(CheckCollisionCircleRec(bola.posicao, bola.raio, 
            (Rectangle){jogador1.posicao.x, jogador1.posicao.y, LARGURA_JOGADOR, ALTURA_JOGADOR})){

                PlaySound(*CONTATO);

                if(bola.velocidade.x < 0){
                    bola.velocidade.x *= -1;
                    // adiciona o contador de vezes que a bola chocou com um dos jogadores 
                    *con += 1;
                    if(*con >= 3){
                        // aumenta a velocidade da bola
                        bola.velocidade.x *= 2;
                        // aumenta a velocidade do jogador
                        *velocidadeJogadores *= 2;
                        // reinicia a contagem de colisão 
                        *con = 0;
                    }
                }
            }

            if(IsKeyDown(KEY_DOWN)) jogador2.posicao.y += *velocidadeJogadores;
            if(jogador2.posicao.y + ALTURA_JOGADOR >= ALTURA_TELA) jogador2.posicao.y = ALTURA_TELA - ALTURA_JOGADOR;
            if(IsKeyDown(KEY_UP)) jogador2.posicao.y -= *velocidadeJogadores;
            if(jogador2.posicao.y + 1 <= 0) jogador2.posicao.y = 1;

            if(CheckCollisionCircleRec(bola.posicao, bola.raio, 
            (Rectangle){jogador2.posicao.x, jogador2.posicao.y , LARGURA_JOGADOR, ALTURA_JOGADOR})){

                PlaySound(*CONTATO);

                if(bola.velocidade.x > 0){
                    bola.velocidade.x *= -1;
                    *con += 1;
                    if(*con >= 3){
                        bola.velocidade.x *= 2;
                        *velocidadeJogadores *= 2;
                        *con = 0;
                    }
                }
            }
        } else {
            // reiniciar a posição da bola para o meio da tela
            bola.posicao = (Vector2){LARGURA_TELA / 2, (ALTURA_TELA) / 2 - 10};
        }

    } else if (*mostrarPause == 1) {
        DrawText("JOGO PAUSADO", LARGURA_TELA / 2 - MeasureText("JOGO PAUSADO", 50) / 2, ALTURA_TELA / 2, 50, WHITE);
    } 

    // se atingir o limite máximo de gols o jogo acaba e a função retorna false
    if (*golJogador1 >= maximoGols || *golJogador2 >= maximoGols) {
        *mostrarPause = 0;
        return false; 
    }
    else {  // se não tiver acabado a função retorna true
        *mostrarPause = 1;
        return true;
    } 
}

// Função que caso o arquivo de save não exista ele o cria e se ele existir, lê suas informações
void lerHistorico() {
    if (!(FileExists("resources/historico.csv"))) {
        SaveFileText("resources/historico.csv", "AAAAA;0;0\nBBBBB;0;0\nCCCCC;0;0\nDDDDD;0;0\nEEEEE;0;0");
    }

    FILE *pontArquivo = fopen("resources/historico.csv", "r+");
    
    if (pontArquivo != NULL) {
        for (int i = 0; i < 5; i++) {
            fscanf(pontArquivo, "%5c;%d;%d\n", historico[i].nickVencedor, &historico[i].gol1, &historico[i].gol2);
        }
    }
    
    fclose(pontArquivo);
}


void desenharJogo(int golJogador1, int golJogador2, Texture2D imagem){
    // Usa as posições dos jogadores para desenhá-los no campo
    Rectangle Jogador1 = { jogador1.posicao.x, jogador1.posicao.y, LARGURA_JOGADOR, ALTURA_JOGADOR } ;
    Rectangle Jogador2 = { jogador2.posicao.x, jogador2.posicao.y, LARGURA_JOGADOR, ALTURA_JOGADOR } ;

    // Desenha o campo
    DrawTexture(imagem, -43, 0, RAYWHITE);

    // Desenha os jogadores
    DrawRectangleRounded(Jogador1, 20, 4, YELLOW);
    DrawRectangleRounded(Jogador2, 20, 4, BLACK);

    // Desenha a bola
    DrawCircleV(bola.posicao, bola.raio, BLUE);

    // Desenha o placar
    DrawText(TextFormat("%d", golJogador1), LARGURA_TELA/ 4, 50, 70, WHITE);
    DrawText(TextFormat("%d", golJogador2), LARGURA_TELA/ 2 + LARGURA_TELA/4, 50, 70, WHITE); 
}   

#endif /* JOGO_H_INCLUDED */
