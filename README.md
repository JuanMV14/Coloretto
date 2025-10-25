# Coloretto - Implementacion en C

## Descripcion

Este proyecto es una implementacion digital del juego de mesa **Coloretto** para 3 a 5 jugadores. El objetivo es obtener la mayor puntuacion al final de la partida coleccionando estrategicamente cartas de colores. El proyecto hace uso de **memoria dinamica** (malloc) para la gestion de estructuras de datos clave como lo requiere el proyecto.

## Equipo de Desarrollo

| Nombre | Rol | Responsabilidad |
|--------|-----|-----------------|
| **Juan** | Coordinador | Logica principal del juego, ciclo de turnos, inicializacion |
| **Ivan** | Developer | Gestion de jugadores, sistema de puntuacion |
| **Stella** | Developer | Gestion de baraja, cartas y pilas |

## Caracteristicas Implementadas

- Soporte para **3 a 5 jugadores** (dinamico)
- Uso de **6 colores activos** (se remueve 1 de los 7 colores base)
- Gestion dinamica de la **Baraja** y las **Pilas de Cartas**
- Implementacion del marcador de **"Ultima Ronda"**
- Calculo de puntuacion final segun las reglas oficiales de Coloretto
- **Memoria dinamica (malloc)** para todas las estructuras principales
- Sistema completo de turnos con opciones de jugador

## Reglas del Juego

### Objetivo
Acumular la mayor cantidad de puntos recolectando cartas de colores. **Solo los 3 colores con mas cartas dan puntos positivos**, el resto dan puntos negativos.

### Mecanica
1. En tu turno puedes:
   - **Robar una carta** del mazo y colocarla en una de las pilas disponibles
   - **Tomar una pila completa** y pasar el turno

2. Cada pila puede tener un maximo de **3 cartas**

3. Cuando todos los jugadores han tomado una pila, se inicia una nueva ronda

4. El juego termina cuando aparece la carta **"Ultima Ronda"**

### Sistema de Puntuacion

**Colores con MAS cartas (Top 3)** - Puntos POSITIVOS:
- 1 carta = 1 punto
- 2 cartas = 3 puntos
- 3 cartas = 6 puntos
- 4 cartas = 10 puntos
- 5 cartas = 15 puntos
- 6+ cartas = 21 puntos

**Otros colores** - Puntos NEGATIVOS:
- 1 carta = -1 punto
- 2 cartas = -3 puntos
- 3 cartas = -6 puntos
- 4+ cartas = -10 puntos

## Estructura del Proyecto

```
Coloretto/
├── src/
│   ├── main.c           # Funcion principal y flujo del juego
│   ├── game_logic.c     # Ciclo de juego y control de partida
│   ├── player.c         # Gestion de jugadores y puntuacion
│   └── deck.c           # Gestion de baraja, cartas y pilas
├── include/
│   ├── common.h         # Constantes y tipos comunes
│   ├── game_logic.h     # Declaraciones de logica de juego
│   ├── player.h         # Declaraciones de jugadores
│   └── deck.h           # Declaraciones de baraja
├── obj/                 # Archivos objeto (generado automaticamente)
├── README.md
├── Makefile
└── .gitignore
```

## Compilacion y Ejecucion

### Requisitos
- **GCC** (GNU Compiler Collection)
- **Make**
- **Valgrind** (opcional, para verificar memoria)

### Comandos
```bash
# Compilar el proyecto
make

# Compilar y ejecutar
make run

# Limpiar archivos compilados
make clean

# Verificar memoria con Valgrind
make valgrind

# Ver ayuda de comandos
make help
```

## Verificacion de Memoria

Para asegurar que no hay fugas de memoria (memory leaks):

```bash
make valgrind
```

**Resultado esperado**: `All heap blocks were freed -- no leaks are possible`

## Uso de Memoria Dinamica

El proyecto cumple con el requisito de usar **asignacion dinamica de memoria** de manera explicita mediante `malloc()` en las siguientes estructuras:

- Array de jugadores (Player**)
- Array de pilas (Pile**)
- Coleccion de cartas de cada jugador
- Cartas individuales (Card*)
- Estructura del mazo (Deck)
- Estructura principal del juego (Game)

Toda la memoria se libera correctamente al final del programa usando `free()`.

## Convenciones de Codigo

- **Funciones**: `snake_case` (ejemplo: `init_game`, `draw_card`)
- **Structs**: `PascalCase` (ejemplo: `Game`, `Player`, `Deck`)
- **Variables**: `snake_case` (ejemplo: `current_player`, `card_count`)
- **Constantes**: `UPPER_SNAKE_CASE` (ejemplo: `MAX_CARDS`, `NUM_PLAYERS`)

## Consideraciones del Proyecto

Este proyecto implementa una **version reducida** del juego original Coloretto segun las especificaciones del curso:

1. Modalidad de juego para **3 a 5 jugadores** (cantidad variable)
2. Uso obligatorio de **memoria dinamica** via malloc
3. Proyecto documentado y versionado en **Git**
4. Todos los integrantes demuestran aportes al proyecto mediante commits

## Autores

- **Juan** - Coordinador y Core Logic
- **Ivan** - Player Management System
- **Stella** - Deck and Cards System

---

**Version**: 1.0.0  
**Fecha**: Octubre 2025  
**Curso**: Estructura de Datos

Disfruta jugando Coloretto!
