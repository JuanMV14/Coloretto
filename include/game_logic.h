#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "common.h"
#include "player.h"
#include "deck.h"

// ============================================
// Clase Game (RAII)
// ============================================
class Game {
public:
    Game();

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
    std::vector<Player> players;
    Deck deck;
    std::vector<Pile> piles;
    int currentPlayerIdx;
    int currentRound;
    bool lastRoundTriggered;
    bool gameOver;
    int playersPassedCount;
};

#endif // GAME_LOGIC_H