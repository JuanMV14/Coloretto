#include "../include/deck.h"

const char* color_to_string(Color color) {
    switch(color) {
        case Color::RED:         return "Rojo";
        case Color::BLUE:        return "Azul";
        case Color::GREEN:       return "Verde";
        case Color::YELLOW:      return "Amarillo";
        case Color::ORANGE:      return "Naranja";
        case Color::BROWN:       return "Marron";
        case Color::GRAY:        return "Gris";
        case Color::LAST_ROUND:  return "Ultima Ronda";
        default:                 return "Desconocido";
    }
}

void print_card(const Card &card) {
    std::printf("[%s]", color_to_string(card.getColor()));
}

Deck::Deck() : cards(), topIndex(0) {
    std::srand((unsigned)std::time(nullptr));
    // Elegir un color aleatorio para remover (no incluye LAST_ROUND)
    int removed = std::rand() % NUM_COLORS;
    Color removedColor = static_cast<Color>(removed);
    // Asegurar que no sea LAST_ROUND
    if (removedColor == Color::LAST_ROUND) {
        removedColor = Color::RED;
    }
    std::printf("Color removido para esta partida: %s\n", color_to_string(removedColor));

    // Generar cartas
    for (int c = 0; c < NUM_COLORS; c++) {
        Color color = static_cast<Color>(c);
        if (color == removedColor) continue;
        if (color == Color::LAST_ROUND) continue;
        for (int i = 0; i < CARDS_PER_COLOR; i++) {
            cards.emplace_back(color, 1);
        }
    }
    // Carta de última ronda
    cards.emplace_back(Color::LAST_ROUND, 0);
    std::printf("Baraja creada: %zu cartas totales\n", cards.size());
}

void Deck::shuffle() {
    if (cards.empty()) return;
    for (std::size_t i = cards.size() - 1; i > 0; i--) {
        std::size_t j = std::rand() % (i + 1);
        std::swap(cards[i], cards[j]);
    }

    int lastRoundIdx = -1;
    for (std::size_t i = 0; i < cards.size(); i++) {
        if (cards[i].getColor() == Color::LAST_ROUND) {
            lastRoundIdx = static_cast<int>(i);
            break;
        }
    }
    if (lastRoundIdx != -1) {
        int target = static_cast<int>(cards.size()) - 16;
        if (target < 0) target = 0;
        if (target >= static_cast<int>(cards.size())) target = static_cast<int>(cards.size()) - 1;
        std::swap(cards[lastRoundIdx], cards[target]);
        std::printf("Carta Ultima Ronda colocada en posicion %d (desde arriba: %d desde abajo)\n", 
                    target, static_cast<int>(cards.size()) - target);
    }
    std::printf("Baraja barajada exitosamente\n");
}

bool Deck::isEmpty() const {
    return topIndex >= cards.size();
}

bool Deck::draw(Card &out) {
    if (isEmpty()) return false;
    out = cards[topIndex++];
    return true;
}

void Deck::print() const {
    std::printf("Baraja: %zu cartas (siguiente: %zu)\n", cards.size() - topIndex, topIndex);
}

Pile::Pile() : cards(), count(0), full(false), taken(false) {
    cards.reserve(MAX_PILE_SIZE);
}

bool Pile::add(const Card &card) {
    if (count >= MAX_PILE_SIZE) { full = true; return false; }
    cards.push_back(card);
    count++;
    if (count == MAX_PILE_SIZE) full = true;
    return true;
}

bool Pile::isFull() const { return full; }
bool Pile::isEmpty() const { return count == 0; }

void Pile::clear() {
    cards.clear();
    count = 0;
    full = false;
    taken = false;
}

void Pile::print(int pileNumber) const {
    std::printf("Pila %d [%d/%d]: ", pileNumber, count, MAX_PILE_SIZE);
    if (taken) {
        std::printf("[TOMADA]");
    } else if (count == 0) {
        std::printf("Vacia");
    } else {
        for (int i = 0; i < count; i++) {
            print_card(cards[static_cast<std::size_t>(i)]);
            if (i < count - 1) std::printf(" ");
        }
    }
    if (full) std::printf(" [LLENA]");
    std::printf("\n");
}

