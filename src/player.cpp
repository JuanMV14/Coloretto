#include "../include/player.h"
#include <cstdlib>  // Para malloc, free
#include <cstring>  // Para strlen, strcpy

Player::Player(const char* name, int id)
    : name(nullptr), id(id), cards(nullptr), cardCount_var(0), cardCapacity(4), score(0), hasPassed(false) {
    // Asignar memoria para el nombre usando malloc
    if (name != nullptr) {
        std::size_t nameLen = std::strlen(name) + 1;
        this->name = static_cast<char*>(std::malloc(nameLen));
        if (this->name == nullptr) {
            std::fprintf(stderr, "Error: No se pudo asignar memoria para el nombre del jugador\n");
            std::exit(EXIT_FAILURE);
        }
        std::strcpy(this->name, name);
    }
    
    // Asignar memoria inicial para las cartas usando new[]
    cards = new Card[cardCapacity];
    if (cards == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para las cartas del jugador\n");
        std::exit(EXIT_FAILURE);
    }
}

Player::~Player() {
    // Liberar memoria del nombre
    if (name != nullptr) {
        std::free(name);
        name = nullptr;
    }
    
    // Liberar memoria de las cartas
    if (cards != nullptr) {
        delete[] cards;
        cards = nullptr;
    }
}

void Player::resizeCardCollection() {
    if (cardCount_var >= cardCapacity) {
        std::size_t newCapacity = cardCapacity * 2;
        Card* newCards = new Card[newCapacity];
        if (newCards == nullptr) {
            std::fprintf(stderr, "Error: No se pudo redimensionar la coleccion de cartas\n");
            return;
        }
        
        // Copiar cartas existentes
        for (std::size_t i = 0; i < cardCount_var; i++) {
            newCards[i] = cards[i];
        }
        
        delete[] cards;
        cards = newCards;
        cardCapacity = newCapacity;
    }
}

void Player::addCard(const Card &card) {
    resizeCardCollection();  // Redimensionar si es necesario
    if (cardCount_var < cardCapacity && cards != nullptr) {
        cards[cardCount_var++] = card;
    }
}

int Player::countByColor(Color color) const {
    int count = 0;
    for (std::size_t i = 0; i < cardCount_var; i++) {
        if (cards[i].getColor() == color) count++;
    }
    return count;
}

int Player::positiveScore(int cardCount) {
    switch (cardCount) {
        case 1: return 1;
        case 2: return 3;
        case 3: return 6;
        case 4: return 10;
        case 5: return 15;
        default: return 21; // 6 o más
    }
}

int Player::negativeScore(int cardCount) {
    switch (cardCount) {
        case 1: return -1;
        case 2: return -3;
        case 3: return -6;
        default: return -10; // 4 o más
    }
}

bool Player::isInTop3(int colorIndex, const int topIdx[TOP_COLORS_COUNT]) {
    for (int i = 0; i < TOP_COLORS_COUNT; i++) {
        if (topIdx[i] == colorIndex) return true;
    }
    return false;
}

void Player::findTop3Colors(const int* counts, int topIdx[TOP_COLORS_COUNT], int numColors) {
    for (int i = 0; i < TOP_COLORS_COUNT; i++) topIdx[i] = -1;
    for (int k = 0; k < TOP_COLORS_COUNT; k++) {
        int maxCount = -1;
        int maxIndex = -1;
        for (int color = 0; color < numColors; color++) {
            if (static_cast<Color>(color) == Color::LAST_ROUND) continue;
            if (isInTop3(color, topIdx)) continue;
            if (counts[color] > maxCount) {
                maxCount = counts[color];
                maxIndex = color;
            }
        }
        if (maxIndex != -1 && maxCount > 0) topIdx[k] = maxIndex;
    }
}

void Player::calculateScore() {
    // Asignar array dinámico para counts usando malloc
    int* counts = static_cast<int*>(std::malloc(NUM_COLORS * sizeof(int)));
    if (counts == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para counts\n");
        return;
    }
    
    // Inicializar a cero
    for (int i = 0; i < NUM_COLORS; i++) {
        counts[i] = 0;
    }
    
    // Contar cartas por color
    for (int i = 0; i < NUM_COLORS; i++) {
        counts[i] = countByColor(static_cast<Color>(i));
    }
    
    int topIdx[TOP_COLORS_COUNT];
    findTop3Colors(counts, topIdx, NUM_COLORS);
    score = 0;
    for (int color = 0; color < NUM_COLORS; color++) {
        if (static_cast<Color>(color) == Color::LAST_ROUND) continue;
        int cnt = counts[color];
        if (cnt == 0) continue;
        if (isInTop3(color, topIdx)) score += positiveScore(cnt);
        else score += negativeScore(cnt);
    }
    
    // Liberar memoria
    std::free(counts);
}

void Player::printCollection() const {
    std::printf("\n---------------------------------------\n");
    std::printf("Coleccion de %s:\n", name != nullptr ? name : "(sin nombre)");
    std::printf("---------------------------------------\n");
    if (cardCount_var == 0) {
        std::printf("  (sin cartas)\n");
    } else {
        for (int color = 0; color < NUM_COLORS; color++) {
            int cnt = countByColor(static_cast<Color>(color));
            if (cnt > 0) {
                std::printf("  %s: %d carta(s)\n", color_to_string(static_cast<Color>(color)), cnt);
            }
        }
    }
    std::printf("---------------------------------------\n");
}

void Player::printSummary() const {
    std::printf("%-15s | %2d cartas | %4d puntos\n", name != nullptr ? name : "(sin nombre)", cardCount(), score);
}

void Player::printScoreBreakdown() const {
    std::printf("\n=======================================\n");
    std::printf("  Desglose de Puntuacion - %s\n", name != nullptr ? name : "(sin nombre)");
    std::printf("=======================================\n");
    
    // Asignar array dinámico para counts usando malloc
    int* counts = static_cast<int*>(std::malloc(NUM_COLORS * sizeof(int)));
    if (counts == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para counts\n");
        return;
    }
    
    // Inicializar a cero
    for (int i = 0; i < NUM_COLORS; i++) {
        counts[i] = 0;
    }
    
    // Contar cartas por color
    for (int i = 0; i < NUM_COLORS; i++) {
        counts[i] = countByColor(static_cast<Color>(i));
    }
    
    int topIdx[TOP_COLORS_COUNT];
    findTop3Colors(counts, topIdx, NUM_COLORS);
    std::printf("\nColores principales (puntos positivos):\n");
    std::printf("---------------------------------------\n");
    for (int i = 0; i < TOP_COLORS_COUNT; i++) {
        if (topIdx[i] != -1) {
            int cnt = counts[topIdx[i]];
            int sc = positiveScore(cnt);
            std::printf("  %s: %d carta(s) = +%d puntos\n", 
                       color_to_string(static_cast<Color>(topIdx[i])), cnt, sc);
        }
    }
    std::printf("\nOtros colores (puntos negativos):\n");
    std::printf("---------------------------------------\n");
    bool hasNegative = false;
    for (int color = 0; color < NUM_COLORS; color++) {
        if (static_cast<Color>(color) == Color::LAST_ROUND) continue;
        if (counts[color] == 0) continue;
        if (isInTop3(color, topIdx)) continue;
        int cnt = counts[color];
        int sc = negativeScore(cnt);
        std::printf("  %s: %d carta(s) = %d puntos\n", color_to_string(static_cast<Color>(color)), cnt, sc);
        hasNegative = true;
    }
    if (!hasNegative) std::printf("  (ninguno)\n");
    std::printf("\n---------------------------------------\n");
    std::printf("  PUNTUACION TOTAL: %d\n", score);
    std::printf("=======================================\n\n");
    
    // Liberar memoria
    std::free(counts);
}

