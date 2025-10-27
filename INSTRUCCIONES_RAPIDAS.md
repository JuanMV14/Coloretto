# Instrucciones Rapidas para Ejecutar

## En el Laboratorio (Linux/macOS)

```bash
# 1. Clonar el repositorio
git clone https://github.com/JuanMV14/Coloretto.git
cd Coloretto

# 2. Compilar
make

# 3. Ejecutar
./coloretto
```

## En Windows con VS Code

```bash
# 1. Clonar el repositorio
git clone https://github.com/JuanMV14/Coloretto.git
cd Coloretto

# 2. Abrir en VS Code
code .

# 3. Compilar y ejecutar
# Presionar Ctrl+Shift+B para compilar
# Presionar F5 para ejecutar
```

## Compilacion Manual (si make no funciona)

```bash
gcc -Wall -Wextra -g -Iinclude src/deck.c src/player.c src/game_logic.c src/main.c -o coloretto
```

## Requisitos

- gcc (GNU Compiler Collection)
- make (opcional, pero recomendado)

## Verificar que gcc este instalado

```bash
gcc --version
```

Si gcc no esta instalado, consultar el README.md principal para instrucciones de instalacion.

