//-----------------------------------------------------------------------------------------------------------
// salvar
// comentários 
// colocar no github *
// explicar o código *
//-----------------------------------------------------------------------------------------------------------

#include "raylib.h"
#include "resources/jogo.h"

int main(void) {
    // Configurações iniciais
    srand(time(NULL));
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Futebol");
    SetTargetFPS(60);
    HideCursor();
    InitAudioDevice();

    // Importa SONS do jogo e configura volume.
    Sound GOL = LoadSound("resources/GOL.wav");
    Sound CONTATO = LoadSound("resources/CONTATO.wav");

    SetSoundVolume(GOL, 0.7);
    SetSoundVolume(CONTATO, 0.7);
    
    // Declaração de variaveis
    int nickCount = 0;
    int golJogador1 = 6, golJogador2 = 0;
    bool pausa = false;
    int con = 0;
    int velocidadeJogadores = 5;
    int selecaoMenu = 0, estaNosRecordes = 0, estaNoJogo = 0;
    int mostrarPause = 1;
    Texture2D imagem = LoadTexture("resources/campo.png");

    comecarJogo();
    lerHistorico();

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(DARKGREEN);

        posicaoMenu(&selecaoMenu, &estaNosRecordes, &estaNoJogo);

        if(estaNoJogo){
            if (atualizacaoDeQuadros(&pausa, &mostrarPause, &con, &velocidadeJogadores, &GOL, &CONTATO, &golJogador1, &golJogador2)) {
                desenharJogo(golJogador1, golJogador2, imagem);
            } else {
                fimDeJogo(&estaNoJogo, &nickCount, &golJogador1, &golJogador2);
            }
        } else {
            menuInicial(estaNosRecordes, selecaoMenu);
        }


        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}