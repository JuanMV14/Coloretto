#include "../include/player.h"

Player::Player(const std::string &name, int id)
    : name(name), id(id), cards(), score(0), hasPassed(false) {}

void Player::addCard(const Card &card) {
    cards.push_back(card);
}

int Player::countByColor(Color color) const {
    int count = 0;
    for (const auto &c : cards) {
        if (c.getColor() == color) count++;
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

void Player::findTop3Colors(const std::vector<int> &counts, int topIdx[TOP_COLORS_COUNT]) {
    for (int i = 0; i < TOP_COLORS_COUNT; i++) topIdx[i] = -1;
    for (int k = 0; k < TOP_COLORS_COUNT; k++) {
        int maxCount = -1;
        int maxIndex = -1;
        for (int color = 0; color < NUM_COLORS; color++) {
            if (static_cast<Color>(color) == Color::LAST_ROUND) continue;
            if (isInTop3(color, topIdx)) continue;
            if (counts[static_cast<std::size_t>(color)] > maxCount) {
                maxCount = counts[static_cast<std::size_t>(color)];
                maxIndex = color;
            }
        }
        if (maxIndex != -1 && maxCount > 0) topIdx[k] = maxIndex;
    }
}

void Player::calculateScore() {
    std::vector<int> counts(NUM_COLORS, 0);
    for (int i = 0; i < NUM_COLORS; i++) {
        counts[static_cast<std::size_t>(i)] = countByColor(static_cast<Color>(i));
    }
    int topIdx[TOP_COLORS_COUNT];
    findTop3Colors(counts, topIdx);
    score = 0;
    for (int color = 0; color < NUM_COLORS; color++) {
        if (static_cast<Color>(color) == Color::LAST_ROUND) continue;
        int cnt = counts[static_cast<std::size_t>(color)];
        if (cnt == 0) continue;
        if (isInTop3(color, topIdx)) score += positiveScore(cnt);
        else score += negativeScore(cnt);
    }
}

void Player::printCollection() const {
    std::printf("\n---------------------------------------\n");
    std::printf("Coleccion de %s:\n", name.c_str());
    std::printf("---------------------------------------\n");
    if (cards.empty()) {
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
    std::printf("%-15s | %2d cartas | %4d puntos\n", name.c_str(), cardCount(), score);
}

void Player::printScoreBreakdown() const {
    std::printf("\n=======================================\n");
    std::printf("  Desglose de Puntuacion - %s\n", name.c_str());
    std::printf("=======================================\n");
    std::vector<int> counts(NUM_COLORS, 0);
    for (int i = 0; i < NUM_COLORS; i++) {
        counts[static_cast<std::size_t>(i)] = countByColor(static_cast<Color>(i));
    }
    int topIdx[TOP_COLORS_COUNT];
    findTop3Colors(counts, topIdx);
    std::printf("\nColores principales (puntos positivos):\n");
    std::printf("---------------------------------------\n");
    for (int i = 0; i < TOP_COLORS_COUNT; i++) {
        if (topIdx[i] != -1) {
            int cnt = counts[static_cast<std::size_t>(topIdx[i])];
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
        if (counts[static_cast<std::size_t>(color)] == 0) continue;
        if (isInTop3(color, topIdx)) continue;
        int cnt = counts[static_cast<std::size_t>(color)];
        int sc = negativeScore(cnt);
        std::printf("  %s: %d carta(s) = %d puntos\n", color_to_string(static_cast<Color>(color)), cnt, sc);
        hasNegative = true;
    }
    if (!hasNegative) std::printf("  (ninguno)\n");
    std::printf("\n---------------------------------------\n");
    std::printf("  PUNTUACION TOTAL: %d\n", score);
    std::printf("=======================================\n\n");
}

