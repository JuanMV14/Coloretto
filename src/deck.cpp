#include "../include/deck.h"
#include <cstdlib>  // Para malloc, free
#include <cstring>  // Para memcpy

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

// ============================================
// Implementación de Deck
// ============================================

Deck::Deck() : cards(nullptr), capacity(0), topIndex(0), removedColor(Color::RED) {
    // Constructor vacío - la inicialización se hace en initialize()
}

Deck::~Deck() {
    // Liberar memoria con delete[] (usando new[]) o free (usando malloc)
    if (cards != nullptr) {
        delete[] cards;  // Usando new[] en initialize()
        cards = nullptr;
    }
}

void Deck::resizeCards(std::size_t newCapacity) {
    if (newCapacity == 0) {
        if (cards != nullptr) {
            delete[] cards;
            cards = nullptr;
        }
        capacity = 0;
        return;
    }
    
    Card* newCards = new Card[newCapacity];  // Usando new[] para arrays
    if (cards != nullptr) {
        std::size_t copySize = (topIndex < capacity) ? topIndex : capacity;
        for (std::size_t i = 0; i < copySize && i < newCapacity; i++) {
            newCards[i] = cards[i];
        }
        delete[] cards;
    }
    cards = newCards;
    capacity = newCapacity;
}

void Deck::initialize() {
    // Inicializar el generador de números aleatorios con la hora actual como semilla
    std::srand((unsigned)std::time(nullptr));
    
    // ASIGNACIÓN DEL COLOR REMOVIDO:
    // Se elige aleatoriamente uno de los 7 colores normales para remover usando std::rand()
    // Los colores son: RED(0), BLUE(1), GREEN(2), YELLOW(3), ORANGE(4), BROWN(5), GRAY(6)
    // LAST_ROUND(7) NO puede ser removido
    // Usamos el valor numérico de LAST_ROUND como límite: rand() % LAST_ROUND da valores 0-6
    
    // Usamos malloc para crear un array dinámico con los 7 colores válidos (demostración de memoria dinámica)
    const int numValidColors = static_cast<int>(Color::LAST_ROUND);  // 7 colores (0-6)
    Color* validColors = static_cast<Color*>(std::malloc(numValidColors * sizeof(Color)));
    if (validColors == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para colores válidos\n");
        removedColor = Color::RED;  // Fallback seguro
    } else {
        // Llenar el array con los 7 colores válidos (0-6, excluyendo LAST_ROUND que es 7)
        for (int i = 0; i < numValidColors; i++) {
            validColors[i] = static_cast<Color>(i);
        }
        
        // Elegir aleatoriamente un índice del array usando std::rand()
        int randomIdx = std::rand() % numValidColors;  // Valor entre 0-6
        removedColor = validColors[randomIdx];
        
        // Liberar memoria con free
        std::free(validColors);
    }
    
    // Verificación de seguridad final (no debería ser necesario, pero por seguridad)
    if (removedColor == Color::LAST_ROUND || static_cast<int>(removedColor) >= static_cast<int>(Color::LAST_ROUND)) {
        removedColor = Color::RED;  // Fallback seguro
    }

    // Calcular número total de cartas: 6 colores * 9 cartas + 1 última ronda = 55
    std::size_t totalCards = (COLORS_IN_PLAY * CARDS_PER_COLOR) + 1;
    
    // Liberar memoria anterior si existe
    if (cards != nullptr) {
        delete[] cards;
    }
    
    // Asignar memoria con new[]
    cards = new Card[totalCards];
    capacity = totalCards;
    topIndex = 0;
    
    std::size_t cardIdx = 0;
    
    // Generar cartas
    for (int c = 0; c < NUM_COLORS; c++) {
        Color color = static_cast<Color>(c);
        if (color == removedColor) continue;
        if (color == Color::LAST_ROUND) continue;
        for (int i = 0; i < CARDS_PER_COLOR; i++) {
            if (cardIdx < capacity) {
                cards[cardIdx++] = Card(color, 1);
            }
        }
    }
    // Carta de última ronda
    if (cardIdx < capacity) {
        cards[cardIdx++] = Card(Color::LAST_ROUND, 0);
    }
}

void Deck::shuffle() {
    if (cards == nullptr || capacity == 0) return;
    
    // Fisher-Yates shuffle
    for (std::size_t i = capacity - 1; i > 0; i--) {
        std::size_t j = std::rand() % (i + 1);
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    int lastRoundIdx = -1;
    for (std::size_t i = 0; i < capacity; i++) {
        if (cards[i].getColor() == Color::LAST_ROUND) {
            lastRoundIdx = static_cast<int>(i);
            break;
        }
    }
    if (lastRoundIdx != -1) {
        int target = static_cast<int>(capacity) - 16;
        if (target < 0) target = 0;
        if (target >= static_cast<int>(capacity)) target = static_cast<int>(capacity) - 1;
        Card temp = cards[lastRoundIdx];
        cards[lastRoundIdx] = cards[target];
        cards[target] = temp;
        std::printf("Carta Ultima Ronda colocada en posicion %d (desde arriba: %d desde abajo)\n", 
                    target, static_cast<int>(capacity) - target);
    }
    std::printf("Baraja barajada exitosamente\n");
}

bool Deck::isEmpty() const {
    return topIndex >= capacity || cards == nullptr;
}

bool Deck::draw(Card &out) {
    if (isEmpty()) return false;
    out = cards[topIndex++];
    return true;
}

void Deck::print() const {
    std::printf("Baraja: %zu cartas (siguiente: %zu)\n", capacity - topIndex, topIndex);
}

// ============================================
// Implementación de Pile
// ============================================

Pile::Pile() : cards(nullptr), count(0), full(false), taken(false) {
    // Asignar memoria para MAX_PILE_SIZE cartas usando malloc
    cards = static_cast<Card*>(std::malloc(MAX_PILE_SIZE * sizeof(Card)));
    if (cards == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para Pile\n");
        std::exit(EXIT_FAILURE);
    }
    // Inicializar con constructores por defecto (placement new podría ser más correcto, pero esto funciona)
    for (int i = 0; i < MAX_PILE_SIZE; i++) {
        new (&cards[i]) Card();  // Placement new para inicializar
    }
}

Pile::~Pile() {
    if (cards != nullptr) {
        // Llamar destructores explícitamente si usamos placement new
        for (std::size_t i = 0; i < count; i++) {
            cards[i].~Card();
        }
        std::free(cards);  // Liberar memoria asignada con malloc
        cards = nullptr;
    }
}

bool Pile::add(const Card &card) {
    if (count >= MAX_PILE_SIZE) { 
        full = true; 
        return false; 
    }
    if (cards == nullptr) return false;
    
    // Usar placement new para asignar
    new (&cards[count]) Card(card);
    count++;
    if (count == MAX_PILE_SIZE) full = true;
    return true;
}

bool Pile::isFull() const { 
    return full; 
}

bool Pile::isEmpty() const { 
    return count == 0; 
}

void Pile::clear() {
    if (cards != nullptr) {
        // Llamar destructores para las cartas existentes
        for (std::size_t i = 0; i < count; i++) {
            cards[i].~Card();
        }
    }
    count = 0;
    full = false;
    taken = false;
}

void Pile::print(int pileNumber) const {
    std::printf("Pila %d [%zu/%d]: ", pileNumber, count, MAX_PILE_SIZE);
    if (taken) {
        std::printf("[TOMADA]");
    } else if (count == 0) {
        std::printf("Vacia");
    } else {
        for (std::size_t i = 0; i < count; i++) {
            print_card(cards[i]);
            if (i < count - 1) std::printf(" ");
        }
    }
    if (full) std::printf(" [LLENA]");
    std::printf("\n");
}

