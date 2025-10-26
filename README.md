# Coloretto
Este proyecto consiste en el desarrollo del clásico juego de estrategia Coloreto, implementado completamente en C++, haciendo uso de memoria dinámica para la gestión eficiente de los datos del juego

@startuml Coloretto_UML

' Configuracion de estilos
skinparam classAttributeIconSize 0
skinparam backgroundColor #FEFEFE
skinparam class {
    BackgroundColor<<Stella>> LightGreen
    BackgroundColor<<Ivan>> LightBlue
    BackgroundColor<<Juan>> LightCoral
    BorderColor Black
    ArrowColor Black
}

title Diagrama de Clases UML - Proyecto Coloretto

' ============================================
' ENUMERACION COLOR (STELLA)
' ============================================
enum Color <<Stella>> {
    COLOR_RED = 0
    COLOR_BLUE
    COLOR_GREEN
    COLOR_YELLOW
    COLOR_ORANGE
    COLOR_BROWN
    COLOR_GRAY
    COLOR_LAST_ROUND
}

' ============================================
' CLASE CARD (STELLA)
' ============================================
class Card <<Stella>> {
    - color : Color
    - value : int
    __
    + create_card(color: Color, value: int) : Card*
    + free_card(card: Card*) : void
    + print_card(card: Card*) : void
}

' ============================================
' CLASE DECK (STELLA)
' ============================================
class Deck <<Stella>> {
    - cards : Card**
    - count : int
    - capacity : int
    - top_index : int
    __
    + create_full_deck() : Deck*
    + shuffle_deck(deck: Deck*) : void
    + draw_card(deck: Deck*) : Card*
    + deck_is_empty(deck: Deck*) : bool
    + free_deck(deck: Deck*) : void
    + print_deck(deck: Deck*) : void
}

' ============================================
' CLASE PILE (STELLA)
' ============================================
class Pile <<Stella>> {
    - cards : Card**
    - count : int
    - is_full : bool
    __
    + create_pile() : Pile*
    + add_card_to_pile(pile: Pile*, card: Card*) : bool
    + pile_is_full(pile: Pile*) : bool
    + pile_is_empty(pile: Pile*) : bool
    + clear_pile(pile: Pile*) : void
    + free_pile(pile: Pile*) : void
    + print_pile(pile: Pile*, number: int) : void
}

' ============================================
' CLASE PLAYER (IVAN)
' ============================================
class Player <<Ivan>> {
    - name : char*
    - id : int
    - collected_cards : Card**
    - card_count : int
    - max_cards : int
    - score : int
    - has_passed : bool
    __
    + create_player(name: char*, id: int) : Player*
    + free_player(player: Player*) : void
    + add_card_to_collection(player: Player*, card: Card*) : void
    + resize_card_collection(player: Player*) : void
    + count_cards_by_color(player: Player*, color: Color) : int
    + calculate_score(player: Player*) : void
    + get_positive_score(count: int) : int
    + get_negative_score(count: int) : int
    + find_top_3_colors(counts: int[], indices: int[]) : void
    + print_player_collection(player: Player*) : void
    + print_score_breakdown(player: Player*) : void
    + print_player_summary(player: Player*) : void
    + reset_player_turn(player: Player*) : void
}

' ============================================
' CLASE GAME (JUAN)
' ============================================
class Game <<Juan>> {
    - players : Player**
    - num_players : int
    - main_deck : Deck*
    - piles : Pile**
    - num_piles : int
    - current_player_idx : int
    - current_round : int
    - last_round_triggered : bool
    - game_over : bool
    - players_passed_count : int
    __
    + init_game() : Game*
    + assign_initial_colors(game: Game*) : void
    + prepare_game(game: Game*) : void
    + game_is_running(game: Game*) : bool
    + player_turn(game: Game*) : void
    + handle_draw_card_action(game: Game*, pile_idx: int) : bool
    + handle_take_pile_action(game: Game*, pile_idx: int) : bool
    + next_player(game: Game*) : void
    + all_players_passed(game: Game*) : bool
    + reset_round(game: Game*) : void
    + calculate_scores_and_declare_winner(game: Game*) : void
    + get_winner(game: Game*) : Player*
    + cleanup(game: Game*) : void
    + display_game_state(game: Game*) : void
    + display_piles(game: Game*) : void
    + display_menu() : void
    + display_welcome_message() : void
    + display_final_results(game: Game*) : void
    + get_player_choice() : int
    + get_pile_selection(max_index: int) : int
    + wait_for_enter() : void
    + clear_input_buffer() : void
    + print_separator() : void
}

' ============================================
' RELACIONES
' ============================================

' Card depende de Color
Card --> Color : uses

' Deck contiene Cards
Deck "1" o-- "*" Card : contains

' Pile contiene hasta 3 Cards
Pile "1" o-- "0..3" Card : contains

' Player colecciona Cards
Player "1" o-- "*" Card : collects

' Game compone 3-5 Players (variable)
Game "1" *-- "3..5" Player : has

' Game compone 1 Deck
Game "1" *-- "1" Deck : has

' Game compone 3-5 Piles (variable)
Game "1" *-- "3..5" Pile : has

' ============================================
' NOTAS
' ============================================

note right of Card
  *Responsable: Stella*
  
  Representa una carta individual
  con un color y valor.
  
  value:
  - 1 para colores normales
  - 0 para carta ultima ronda
end note

note right of Deck
  *Responsable: Stella*
  
  Baraja principal del juego.
  Usa algoritmo Fisher-Yates
  para barajar.
  
  Total: 55 cartas
  - 54 cartas de colores (6x9)
  - 1 carta ultima ronda
  
  Usa malloc para crear el
  array dinamico de cartas.
end note

note right of Player
  *Responsable: Ivan*
  
  Gestiona la coleccion de cartas
  de cada jugador.
  
  Sistema de puntuacion:
  - Top 3 colores: puntos positivos
  - Resto de colores: puntos negativos
  
  Usa realloc para redimensionar
  el array de cartas dinamicamente.
end note

note right of Game
  *Responsable: Juan*
  
  Contenedor principal que
  coordina todo el juego.
  
  Soporta 3-5 jugadores (variable).
  El numero de pilas es igual al
  numero de jugadores.
  
  Controla:
  - Ciclo de turnos
  - Estado del juego
  - Interfaz de usuario
  - Limpieza de memoria
  
  Usa malloc para crear arrays
  dinamicos de jugadores y pilas.
end note

note bottom of Pile
  *Responsable: Stella*
  
  Maximo 3 cartas por pila.
  Numero de pilas en mesa: 3-5
  (igual al numero de jugadores).
end note

' ============================================
' LEYENDA
' ============================================

legend right
  |= Simbolo |= Significado |
  | ◆―――→ | Composicion (parte de) |
  | ◇―――→ | Agregacion (contiene) |
  | ―――→ | Dependencia (usa) |
  
  |= Color |= Responsable |
  | <back:LightGreen>Verde</back> | Stella (Deck & Common) |
  | <back:LightBlue>Azul</back> | Ivan (Player Management) |
  | <back:LightCoral>Rojo</back> | Juan (Core Logic) |
  
  *Nota:* Este proyecto usa memoria dinamica
  (malloc/realloc/free) segun las especificaciones.
endlegend

@enduml