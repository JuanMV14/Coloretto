#include "../include/game_logic.h"
#include <cstdlib>  // Para malloc, free
#include <cstring>  // Para strlen, strcpy

Game::Game()
    : players(nullptr), numPlayers(0), deck(), piles(nullptr), numPiles(0), 
      currentPlayerIdx(0), currentRound(1), lastRoundTriggered(false), 
      gameOver(false), playersPassedCount(0) {
    std::printf("\nCuantos jugadores van a jugar? (3-5): ");
    int nPlayers = 0;
    while (nPlayers < 3 || nPlayers > 5) {
        if (!(std::cin >> nPlayers)) {
            clearInputBuffer();
            std::printf("Entrada invalida. Intenta de nuevo (3-5): ");
            continue;
        }
        clearInputBuffer();
        if (nPlayers < 3 || nPlayers > 5) {
            std::printf("Numero invalido. Debe ser entre 3 y 5: ");
        }
    }
    
    numPlayers = static_cast<std::size_t>(nPlayers);
    numPiles = numPlayers;
    
    // Asignar memoria para jugadores usando malloc (luego usamos placement new)
    players = static_cast<Player*>(std::malloc(numPlayers * sizeof(Player)));
    if (players == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para jugadores\n");
        std::exit(EXIT_FAILURE);
    }
    
    // Asignar memoria para pilas usando malloc (luego usamos placement new)
    piles = static_cast<Pile*>(std::malloc(numPiles * sizeof(Pile)));
    if (piles == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para pilas\n");
        std::exit(EXIT_FAILURE);
    }

    // Leer nombres de jugadores
    char nameBuffer[256];  // Buffer temporal para leer nombres
    for (std::size_t i = 0; i < numPlayers; i++) {
        std::printf("Nombre del jugador %zu: ", i + 1);
        if (!(std::cin >> nameBuffer)) {
            clearInputBuffer();
            i--;
            continue;
        }
        clearInputBuffer();
        
        // Crear jugador usando placement new en el array
        new (&players[i]) Player(nameBuffer, static_cast<int>(i));
    }
    
    // Inicializar pilas con placement new
    for (std::size_t i = 0; i < numPiles; i++) {
        new (&piles[i]) Pile();
    }
    std::printf("\nJuego inicializado para %zu jugadores\n", numPlayers);
}

Game::~Game() {
    // Llamar destructores explícitamente para jugadores
    if (players != nullptr) {
        for (std::size_t i = 0; i < numPlayers; i++) {
            players[i].~Player();
        }
        std::free(players);  // Liberar memoria asignada con malloc
        players = nullptr;
    }
    
    // Llamar destructores explícitamente para pilas
    if (piles != nullptr) {
        for (std::size_t i = 0; i < numPiles; i++) {
            piles[i].~Pile();
        }
        std::free(piles);  // Liberar memoria asignada con malloc
        piles = nullptr;
    }
}

void Game::assignInitialColors() {
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("     DISTRIBUCION DE CARTAS INICIALES  \n");
    std::printf("=======================================\n");
    std::printf("  Cada jugador recibe una carta inicial\n");
    std::printf("  al azar de la baraja:\n");
    std::printf("\n");
    for (std::size_t i = 0; i < numPlayers; i++) {
        Card initial;
        if (deck.draw(initial)) {
            players[i].addCard(initial);
            std::printf("  - %s recibe: %s\n", players[i].getName(), color_to_string(initial.getColor()));
        }
    }
    std::printf("=======================================\n");
}

void Game::prepare() {
    std::printf("\nPreparando el juego...\n");
    // Inicializar la baraja (crear cartas y elegir color removido)
    deck.initialize();
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("     CONFIGURACION DE LA PARTIDA       \n");
    std::printf("=======================================\n");
    std::printf("  Para esta partida se ha removido\n");
    std::printf("  aleatoriamente el color: %s\n", color_to_string(deck.getRemovedColor()));
    std::printf("  (Este color no aparecera en la baraja)\n");
    std::printf("\n");
    std::printf("  Baraja creada: %d cartas totales\n", deck.getTotalCards());
    std::printf("  Colores activos en juego: 6\n");
    std::printf("=======================================\n");
    deck.shuffle();
    std::printf("\nJuego listo para comenzar\n");
    waitForEnter();
}

bool Game::isRunning() {
    if (gameOver) return false;
    if (deck.isEmpty()) { gameOver = true; return false; }
    return true;
}

bool Game::hasAvailablePile() const {
    for (std::size_t i = 0; i < numPiles; i++) {
        if (!piles[i].isTaken() && !piles[i].isFull()) return true;
    }
    return false;
}

void Game::playerTurn() {
    Player &current = players[static_cast<std::size_t>(currentPlayerIdx)];

    if (current.hasPassedThisRound()) {
        if (allPlayersPassed()) {
            std::printf("\nTodos los jugadores han pasado. Nueva ronda.\n");
            resetRound();
            waitForEnter();
            return;
        }
        nextPlayer();
        return;
    }

    displayGameState();
    std::printf("\n=======================================\n");
    std::printf("   TURNO DE: %s\n", current.getName());
    std::printf("=======================================\n");
    
    // Verificar si hay pilas disponibles antes de mostrar el menú
    bool canDrawCard = hasAvailablePile();
    
    if (!canDrawCard) {
        std::printf("\n");
        std::printf("=======================================\n");
        std::printf("   NO HAY PILAS DISPONIBLES\n");
        std::printf("=======================================\n");
        std::printf("  Todas las pilas estan llenas o tomadas.\n");
        std::printf("  No puedes robar mas cartas.\n");
        std::printf("  Debes tomar una pila completa.\n");
        std::printf("=======================================\n\n");
        displayPiles();
        std::printf("\nQue pila deseas tomar? (1-%zu): ", numPiles);
        int pileIdx = getPileSelection(static_cast<int>(numPiles)) - 1;
        if (handleTakePileAction(pileIdx)) {
            std::printf("%s tomo la pila %d\n", current.getName(), pileIdx + 1);
            current.markPassed();
            playersPassedCount++;
        } else {
            std::printf("No se pudo tomar la pila\n");
        }
        if (allPlayersPassed()) {
            std::printf("\nTodos los jugadores han pasado. Nueva ronda.\n");
            resetRound();
        }
        nextPlayer();
        waitForEnter();
        return;
    }
    
    displayMenu();
    int choice = getPlayerChoice();

    if (choice == 1) {
        Card drawn;
        if (!deck.draw(drawn)) {
            std::printf("\nNo hay mas cartas en la baraja\n");
            return;
        }
        std::printf("\nCarta robada: %s\n", color_to_string(drawn.getColor()));
        if (drawn.getColor() == Color::LAST_ROUND) {
            std::printf("\n");
            std::printf("=======================================\n");
            std::printf("   ULTIMA RONDA ACTIVADA!\n");
            std::printf("=======================================\n");
            lastRoundTriggered = true;
            if (!deck.draw(drawn)) { return; }
            std::printf("Nueva carta: %s\n", color_to_string(drawn.getColor()));
        }
        bool placed = false;
        while (!placed) {
            std::printf("\nEn que pila deseas colocar la carta? (1-%zu): ", numPiles);
            int pileIdx = getPileSelection(static_cast<int>(numPiles)) - 1;
            if (piles[static_cast<std::size_t>(pileIdx)].isTaken()) {
                std::printf("Esa pila ya fue tomada en esta ronda. Elige otra pila.\n");
                continue;
            }
            if (piles[static_cast<std::size_t>(pileIdx)].isFull()) {
                std::printf("La pila esta llena. Elige otra pila.\n");
                continue;
            }
            if (piles[static_cast<std::size_t>(pileIdx)].add(drawn)) {
                std::printf("Carta colocada en la pila %d\n", pileIdx + 1);
                placed = true;
            } else {
                std::printf("No se pudo colocar la carta. Intenta otra pila.\n");
            }
        }
    } else if (choice == 2) {
        displayPiles();
        std::printf("\nQue pila deseas tomar? (1-%zu): ", numPiles);
        int pileIdx = getPileSelection(static_cast<int>(numPiles)) - 1;
        if (handleTakePileAction(pileIdx)) {
            std::printf("%s tomo la pila %d\n", current.getName(), pileIdx + 1);
            current.markPassed();
            playersPassedCount++;
        } else {
            std::printf("No se pudo tomar la pila\n");
        }
    }

    if (allPlayersPassed()) {
        std::printf("\nTodos los jugadores han pasado. Nueva ronda.\n");
        resetRound();
    }
    nextPlayer();
    waitForEnter();
}

bool Game::handleDrawCardAction(int pileIndex) {
    if (pileIndex < 0 || pileIndex >= static_cast<int>(numPiles)) return false;
    if (piles[static_cast<std::size_t>(pileIndex)].isFull()) {
        std::printf("La pila esta llena\n");
        return false;
    }
    Card drawn;
    if (!deck.draw(drawn)) { std::printf("No hay mas cartas en la baraja\n"); return false; }
    std::printf("Carta robada: %s\n", color_to_string(drawn.getColor()));
    if (drawn.getColor() == Color::LAST_ROUND) {
        std::printf("\n=======================================\n");
        std::printf("   ULTIMA RONDA ACTIVADA!\n");
        std::printf("=======================================\n");
        lastRoundTriggered = true;
        if (!deck.draw(drawn)) return false;
        std::printf("Nueva carta: %s\n", color_to_string(drawn.getColor()));
    }
    return piles[static_cast<std::size_t>(pileIndex)].add(drawn);
}

bool Game::handleTakePileAction(int pileIndex) {
    if (pileIndex < 0 || pileIndex >= static_cast<int>(numPiles)) return false;
    Pile &pile = piles[static_cast<std::size_t>(pileIndex)];
    Player &current = players[static_cast<std::size_t>(currentPlayerIdx)];
    if (pile.isEmpty()) { std::printf("La pila esta vacia\n"); return false; }
    if (pile.isTaken()) { std::printf("Esa pila ya fue tomada en esta ronda\n"); return false; }
    const Card* pileCards = pile.getCards();
    std::size_t cardCount = pile.getCardCount();
    for (std::size_t i = 0; i < cardCount; i++) {
        current.addCard(pileCards[i]);
    }
    std::printf("%s recibio %d carta(s)\n", current.getName(), pile.size());
    pile.markTaken();
    return true;
}

void Game::nextPlayer() {
    int attempts = 0;
    do {
        currentPlayerIdx = (currentPlayerIdx + 1) % static_cast<int>(numPlayers);
        attempts++;
        if (attempts > static_cast<int>(numPlayers)) break;
    } while (players[static_cast<std::size_t>(currentPlayerIdx)].hasPassedThisRound());
}

bool Game::allPlayersPassed() const {
    return playersPassedCount >= static_cast<int>(numPlayers);
}

void Game::resetRound() {
    for (std::size_t i = 0; i < numPiles; i++) {
        piles[i].clear();
    }
    for (std::size_t i = 0; i < numPlayers; i++) {
        players[i].resetTurn();
    }
    playersPassedCount = 0;
    currentRound++;
    if (lastRoundTriggered) {
        std::printf("\nUltima ronda completada. Fin del juego.\n");
        gameOver = true;
    }
}

void Game::calculateScoresAndDeclareWinner() {
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("     CALCULANDO PUNTUACIONES\n");
    std::printf("=======================================\n\n");
    for (std::size_t i = 0; i < numPlayers; i++) {
        players[i].calculateScore();
        players[i].printScoreBreakdown();
        waitForEnter();
    }
    displayFinalResults();
    // Ganador
    int bestIdx = 0;
    for (int i = 1; i < static_cast<int>(numPlayers); i++) {
        if (players[static_cast<std::size_t>(i)].getScore() > players[static_cast<std::size_t>(bestIdx)].getScore()) {
            bestIdx = i;
        }
    }
    std::printf("\n=======================================\n\n");
    std::printf("   GANADOR: %s\n", players[static_cast<std::size_t>(bestIdx)].getName());
    std::printf("   Puntuacion: %d\n", players[static_cast<std::size_t>(bestIdx)].getScore());
    std::printf("\n=======================================\n");
}

void Game::displayMenu() const {
    std::printf("\n---------------------------------------\n");
    std::printf("            TUS OPCIONES               \n");
    std::printf("---------------------------------------\n");
    std::printf("  1. Robar carta y colocar en pila    \n");
    std::printf("  2. Tomar una pila completa           \n");
    std::printf("---------------------------------------\n");
}

void Game::displayGameState() const {
    std::printf("\n\n");
    std::printf("=======================================\n");
    std::printf("         ESTADO DEL JUEGO              \n");
    std::printf("=======================================\n");
    std::printf("  Ronda: %d\n", currentRound);
    std::printf("  Cartas restantes: %d\n", deck.remaining());
    // Nota: si se desea mostrar conteo real, se puede exponer Deck::print o un getter
    if (lastRoundTriggered) { std::printf("  [!] ULTIMA RONDA ACTIVA\n"); }
    std::printf("=======================================\n\n");
    displayPiles();
    std::printf("\nESTADO DE JUGADORES:\n");
    std::printf("-----------------------------------------\n");
    for (std::size_t i = 0; i < numPlayers; i++) {
        const Player &p = players[i];
        std::printf("  %s: %d cartas", p.getName(), p.cardCount());
        if (p.hasPassedThisRound()) std::printf(" [PASO]");
        if ((int)i == currentPlayerIdx) std::printf(" <- TURNO ACTUAL");
        std::printf("\n");
    }
    std::printf("-----------------------------------------\n");
}

void Game::displayPiles() const {
    std::printf("\nPILAS DISPONIBLES:\n");
    std::printf("-----------------------------------------\n");
    for (std::size_t i = 0; i < numPiles; i++) {
        piles[i].print(static_cast<int>(i + 1));
    }
    std::printf("-----------------------------------------\n");
}

void Game::displayWelcomeMessage() const {
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("         CONFIGURACION DEL JUEGO       \n");
    std::printf("=======================================\n");
    std::printf("\n");
}

void Game::displayFinalResults() const {
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("        RESULTADOS FINALES\n");
    std::printf("=======================================\n\n");
    // Ordenar copia de índices por score usando malloc para array dinámico
    int* idx = static_cast<int*>(std::malloc(numPlayers * sizeof(int)));
    if (idx == nullptr) {
        std::fprintf(stderr, "Error: No se pudo asignar memoria para índices\n");
        return;
    }
    for (std::size_t i = 0; i < numPlayers; i++) {
        idx[i] = static_cast<int>(i);
    }
    // Ordenamiento simple (bubble sort) para mantener uso de memoria dinámica
    for (std::size_t i = 0; i < numPlayers - 1; i++) {
        for (std::size_t j = 0; j < numPlayers - i - 1; j++) {
            if (players[idx[j]].getScore() < players[idx[j + 1]].getScore()) {
                int temp = idx[j];
                idx[j] = idx[j + 1];
                idx[j + 1] = temp;
            }
        }
    }
    for (std::size_t i = 0; i < numPlayers; i++) {
        std::printf("%zu. ", i + 1);
        players[idx[i]].printSummary();
    }
    std::printf("\n");
    std::free(idx);  // Liberar memoria
}

int Game::getPlayerChoice() const {
    int choice = 0;
    while (choice != 1 && choice != 2) {
        std::printf("\nTu eleccion (1-2): ");
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            std::printf("Entrada invalida. Intenta de nuevo.\n");
            continue;
        }
        clearInputBuffer();
        if (choice != 1 && choice != 2) {
            std::printf("Opcion invalida. Elige 1 o 2.\n");
        }
    }
    return choice;
}

int Game::getPileSelection(int maxIndex) const {
    int selection = 0;
    while (selection < 1 || selection > maxIndex) {
        if (!(std::cin >> selection)) {
            clearInputBuffer();
            std::printf("Entrada invalida. Intenta de nuevo: ");
            continue;
        }
        clearInputBuffer();
        if (selection < 1 || selection > maxIndex) {
            std::printf("Seleccion invalida. Elige entre 1 y %d: ", maxIndex);
        }
    }
    return selection;
}

void Game::waitForEnter() const {
    std::printf("\n[Presiona ENTER para continuar]");
    clearInputBuffer();
    std::cin.get();
}

void Game::clearInputBuffer() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


