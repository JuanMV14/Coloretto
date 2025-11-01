#include "../include/game_logic.h"

Game::Game()
    : players(), deck(), piles(), currentPlayerIdx(0), currentRound(1),
      lastRoundTriggered(false), gameOver(false), playersPassedCount(0) {
    std::printf("\nCuantos jugadores van a jugar? (3-5): ");
    int numPlayers = 0;
    while (numPlayers < 3 || numPlayers > 5) {
        if (!(std::cin >> numPlayers)) {
            clearInputBuffer();
            std::printf("Entrada invalida. Intenta de nuevo (3-5): ");
            continue;
        }
        clearInputBuffer();
        if (numPlayers < 3 || numPlayers > 5) {
            std::printf("Numero invalido. Debe ser entre 3 y 5: ");
        }
    }
    players.reserve(numPlayers);
    piles.reserve(numPlayers);

    std::string name;
    for (int i = 0; i < numPlayers; i++) {
        std::printf("Nombre del jugador %d: ", i + 1);
        if (!(std::cin >> name)) {
            clearInputBuffer();
            i--;
            continue;
        }
        clearInputBuffer();
        players.emplace_back(name, i);
    }
    for (int i = 0; i < numPlayers; i++) {
        piles.emplace_back();
    }
    std::printf("\nJuego inicializado para %d jugadores\n", numPlayers);
}

void Game::assignInitialColors() {
    std::printf("\nAsignando colores iniciales...\n");
    for (std::size_t i = 0; i < players.size(); i++) {
        Card initial;
        if (deck.draw(initial)) {
            players[i].addCard(initial);
            std::printf("  %s recibe: %s\n", players[i].getName().c_str(), color_to_string(initial.getColor()));
        }
    }
}

void Game::prepare() {
    std::printf("\nPreparando el juego...\n");
    deck.shuffle();
    std::printf("Juego listo para comenzar\n");
    waitForEnter();
}

bool Game::isRunning() {
    if (gameOver) return false;
    if (deck.isEmpty()) { gameOver = true; return false; }
    return true;
}

bool Game::hasAvailablePile() const {
    for (const auto &p : piles) {
        if (!p.isTaken() && !p.isFull()) return true;
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
    std::printf("   TURNO DE: %s\n", current.getName().c_str());
    std::printf("=======================================\n");
    displayMenu();
    int choice = getPlayerChoice();

    if (choice == 1) {
        if (!hasAvailablePile()) {
            std::printf("\n¡No hay pilas disponibles! Todas estan llenas o tomadas.\n");
            std::printf("Debes tomar una pila.\n");
            return;
        }
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
            std::printf("\nEn que pila deseas colocar la carta? (1-%d): ", (int)piles.size());
            int pileIdx = getPileSelection((int)piles.size()) - 1;
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
        std::printf("\nQue pila deseas tomar? (1-%d): ", (int)piles.size());
        int pileIdx = getPileSelection((int)piles.size()) - 1;
        if (handleTakePileAction(pileIdx)) {
            std::printf("%s tomo la pila %d\n", current.getName().c_str(), pileIdx + 1);
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
    if (pileIndex < 0 || pileIndex >= (int)piles.size()) return false;
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
    if (pileIndex < 0 || pileIndex >= (int)piles.size()) return false;
    Pile &pile = piles[static_cast<std::size_t>(pileIndex)];
    Player &current = players[static_cast<std::size_t>(currentPlayerIdx)];
    if (pile.isEmpty()) { std::printf("La pila esta vacia\n"); return false; }
    if (pile.isTaken()) { std::printf("Esa pila ya fue tomada en esta ronda\n"); return false; }
    for (const auto &c : pile.getCards()) { current.addCard(c); }
    std::printf("%s recibio %d carta(s)\n", current.getName().c_str(), pile.size());
    pile.markTaken();
    return true;
}

void Game::nextPlayer() {
    int attempts = 0;
    do {
        currentPlayerIdx = (currentPlayerIdx + 1) % (int)players.size();
        attempts++;
        if (attempts > (int)players.size()) break;
    } while (players[static_cast<std::size_t>(currentPlayerIdx)].hasPassedThisRound());
}

bool Game::allPlayersPassed() const {
    return playersPassedCount >= (int)players.size();
}

void Game::resetRound() {
    for (auto &p : piles) { p.clear(); }
    for (auto &pl : players) { pl.resetTurn(); }
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
    for (auto &p : players) {
        p.calculateScore();
        p.printScoreBreakdown();
        waitForEnter();
    }
    displayFinalResults();
    // Ganador
    int bestIdx = 0;
    for (int i = 1; i < (int)players.size(); i++) {
        if (players[static_cast<std::size_t>(i)].getScore() > players[static_cast<std::size_t>(bestIdx)].getScore()) {
            bestIdx = i;
        }
    }
    std::printf("\n=======================================\n\n");
    std::printf("   GANADOR: %s\n", players[static_cast<std::size_t>(bestIdx)].getName().c_str());
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
    for (std::size_t i = 0; i < players.size(); i++) {
        const Player &p = players[i];
        std::printf("  %s: %d cartas", p.getName().c_str(), p.cardCount());
        if (p.hasPassedThisRound()) std::printf(" [PASO]");
        if ((int)i == currentPlayerIdx) std::printf(" <- TURNO ACTUAL");
        std::printf("\n");
    }
    std::printf("-----------------------------------------\n");
}

void Game::displayPiles() const {
    std::printf("\nPILAS DISPONIBLES:\n");
    std::printf("-----------------------------------------\n");
    for (int i = 0; i < (int)piles.size(); i++) {
        piles[static_cast<std::size_t>(i)].print(i + 1);
    }
    std::printf("-----------------------------------------\n");
}

void Game::displayWelcomeMessage() const {
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("\n");
    std::printf("        COLORETTO\n");
    std::printf("\n");
    std::printf("   Implementacion en C++\n");
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("\n");
}

void Game::displayFinalResults() const {
    std::printf("\n");
    std::printf("=======================================\n");
    std::printf("        RESULTADOS FINALES\n");
    std::printf("=======================================\n\n");
    // Ordenar copia de índices por score
    std::vector<int> idx(players.size());
    for (int i = 0; i < (int)players.size(); i++) idx[static_cast<std::size_t>(i)] = i;
    std::sort(idx.begin(), idx.end(), [&](int a, int b){
        return players[static_cast<std::size_t>(a)].getScore() > players[static_cast<std::size_t>(b)].getScore();
    });
    for (int i = 0; i < (int)idx.size(); i++) {
        std::printf("%d. ", i + 1);
        players[static_cast<std::size_t>(idx[static_cast<std::size_t>(i)])].printSummary();
    }
    std::printf("\n");
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


