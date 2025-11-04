#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "deck.h"

#define TOP_COLORS_COUNT 3

// ============================================
// Clase Player (con memoria dinámica explícita)
// ============================================
class Player {
public:
    Player(const char* name, int id);  // Cambiado de std::string a const char*
    ~Player();  // Destructor para liberar memoria
    
    void addCard(const Card &card);
    int countByColor(Color color) const;
    void calculateScore();
    int getScore() const { return score; }
    const char* getName() const { return name; }  // Cambiado de std::string& a const char*
    int cardCount() const { return static_cast<int>(cardCount_var); }
    void printCollection() const;
    void printSummary() const;
    void printScoreBreakdown() const;
    void resetTurn() { hasPassed = false; }
    bool hasPassedThisRound() const { return hasPassed; }
    void markPassed() { hasPassed = true; }

private:
    static int positiveScore(int cardCount);
    static int negativeScore(int cardCount);
    static void findTop3Colors(const int* counts, int topIdx[TOP_COLORS_COUNT], int numColors);
    static bool isInTop3(int colorIndex, const int topIdx[TOP_COLORS_COUNT]);
    void resizeCardCollection();  // Redimensiona el array de cartas

private:
    char* name;         // Nombre del jugador (malloc)
    int id;
    Card* cards;        // Array dinámico de cartas (malloc/new)
    std::size_t cardCount_var;  // Número actual de cartas
    std::size_t cardCapacity;   // Capacidad del array de cartas
    int score;
    bool hasPassed;
};

#endif // PLAYER_H