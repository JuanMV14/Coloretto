#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "deck.h"

#define TOP_COLORS_COUNT 3

// ============================================
// Clase Player (RAII)
// ============================================
class Player {
public:
    Player(const std::string &name, int id);

    void addCard(const Card &card);
    int countByColor(Color color) const;
    void calculateScore();
    int getScore() const { return score; }
    const std::string& getName() const { return name; }
    int cardCount() const { return static_cast<int>(cards.size()); }
    void printCollection() const;
    void printSummary() const;
    void printScoreBreakdown() const;
    void resetTurn() { hasPassed = false; }
    bool hasPassedThisRound() const { return hasPassed; }
    void markPassed() { hasPassed = true; }

private:
    static int positiveScore(int cardCount);
    static int negativeScore(int cardCount);
    static void findTop3Colors(const std::vector<int> &counts, int topIdx[TOP_COLORS_COUNT]);
    static bool isInTop3(int colorIndex, const int topIdx[TOP_COLORS_COUNT]);

private:
    std::string name;
    int id;
    std::vector<Card> cards;
    int score;
    bool hasPassed;
};

#endif // PLAYER_H