#include "../include/game_logic.h"

int main(void) {
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

