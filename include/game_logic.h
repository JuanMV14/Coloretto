#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "common.h"
#include "player.h"
#include "deck.h"

// ============================================
// Clase Game (con memoria dinámica explícita)
// ============================================
class Game {
public:
    Game();
    ~Game();  // Destructor para liberar memoria

    void assignInitialColors();
    void prepare();
    bool isRunning();
    void playerTurn();
    void calculateScoresAndDeclareWinner();

    // Display
    void displayMenu() const;
    void displayGameState() const;
    void displayPiles() const;
    void displayWelcomeMessage() const;
    void displayFinalResults() const;

private:
    bool hasAvailablePile() const;
    bool handleDrawCardAction(int pileIndex);
    bool handleTakePileAction(int pileIndex);
    void nextPlayer();
    bool allPlayersPassed() const;
    void resetRound();
    int getPlayerChoice() const;
    int getPileSelection(int maxIndex) const;
    void waitForEnter() const;
    void clearInputBuffer() const;

private:
    Player* players;        // Array dinámico de jugadores (new/malloc)
    std::size_t numPlayers; // Número de jugadores
    Deck deck;
    Pile* piles;            // Array dinámico de pilas (new/malloc)
    std::size_t numPiles;   // Número de pilas
    int currentPlayerIdx;
    int currentRound;
    bool lastRoundTriggered;
    bool gameOver;
    int playersPassedCount;
};

#endif // GAME_LOGIC_H