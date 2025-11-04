#include "../include/game_logic.h"

void displayIntro() {
    std::printf("\n");
    std::printf("=============================================================\n");
    std::printf("                                                             \n");
    std::printf("              BIENVENIDO A COLORETTO                        \n");
    std::printf("                                                             \n");
    std::printf("=============================================================\n");
    std::printf("\n");
    std::printf("  COLORETTO es un juego de estrategia y coleccion de        \n");
    std::printf("  cartas donde tu objetivo es acumular la mayor cantidad    \n");
    std::printf("  de puntos recolectando cartas de colores.                 \n");
    std::printf("\n");
    std::printf("  REGLAS PRINCIPALES:                                       \n");
    std::printf("  - Solo tus 3 colores con mas cartas dan puntos positivos  \n");
    std::printf("  - Los demas colores dan puntos negativos                  \n");
    std::printf("  - En tu turno puedes robar una carta o tomar una pila     \n");
    std::printf("  - El juego termina cuando aparece la carta Ultima Ronda   \n");
    std::printf("\n");
    std::printf("=============================================================\n");
    std::printf("\n");
    std::printf("Presiona ENTER para comenzar...");
    std::cin.get();
    std::printf("\n");
}

int main(void) {
    displayIntro();
    Game game;
    game.displayWelcomeMessage();
    std::printf("Inicializando juego...\n\n");
    game.prepare();
    game.assignInitialColors();
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("        COMIENZA EL JUEGO!\n");
    std::printf("=======================================\n");
    while (game.isRunning()) {
        game.playerTurn();
    }
    game.calculateScoresAndDeclareWinner();
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("     GRACIAS POR JUGAR!\n");
    std::printf("=======================================\n\n");
    return 0;
}

