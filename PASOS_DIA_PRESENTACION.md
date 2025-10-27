# Pasos para el Dia de la Presentacion

## Antes de Llegar al Laboratorio

1. Verifica que tu repositorio este actualizado en GitHub:
   - URL: https://github.com/JuanMV14/Coloretto

## Al Llegar al Laboratorio

### Paso 1: Verificar que gcc este instalado

Abre la terminal y escribe:
```bash
gcc --version
```

Si aparece la version de gcc, continua al Paso 2.

Si NO aparece, pide ayuda al profesor o usa otra computadora.

### Paso 2: Clonar el repositorio

```bash
cd ~/Desktop
git clone https://github.com/JuanMV14/Coloretto.git
cd Coloretto
```

### Paso 3: (Opcional) Ejecutar prueba de compilacion

```bash
chmod +x test_compilacion.sh
./test_compilacion.sh
```

Este script verifica que todo este correcto antes de compilar.

### Paso 4: Compilar el proyecto

**Opcion A (Con make):**
```bash
make
```

**Opcion B (Manual, si make no funciona):**
```bash
gcc -Wall -Wextra -g -Iinclude src/deck.c src/player.c src/game_logic.c src/main.c -o coloretto
```

### Paso 5: Ejecutar el juego

```bash
./coloretto
```

## Si Algo Sale Mal

### Error: "gcc: command not found"
- La computadora no tiene gcc instalado
- Intenta en otra computadora del laboratorio

### Error: "make: command not found"
- Usa la opcion B (compilacion manual)

### Errores de compilacion
- Muestra el codigo fuente al profesor
- Muestra el diagrama UML del README
- Explica la estructura del proyecto

## Plan B (Si no puedes compilar)

1. Muestra el codigo fuente bien organizado
2. Explica la estructura con el README.md y el UML
3. Muestra los commits de git para demostrar el trabajo en equipo
4. Explica como usaste memoria dinamica (malloc/realloc)

## Checklist Final

- [ ] Repositorio clonado
- [ ] gcc verificado
- [ ] Proyecto compilado sin errores
- [ ] Ejecutable funciona correctamente
- [ ] README.md y UML listos para mostrar

## Contactos de Emergencia

- Repositorio: https://github.com/JuanMV14/Coloretto
- Si algo falla, tienes este documento en el repositorio

