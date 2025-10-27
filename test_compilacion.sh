#!/bin/bash
# Script de prueba para verificar que todo compile correctamente

echo "=========================================="
echo "  Test de Compilacion - Coloretto"
echo "=========================================="
echo ""

# Verificar gcc
echo "[1/4] Verificando gcc..."
if command -v gcc &> /dev/null; then
    echo "✓ gcc encontrado: $(gcc --version | head -n 1)"
else
    echo "✗ ERROR: gcc no esta instalado"
    echo "  Instalar con: sudo apt install gcc make"
    exit 1
fi

# Verificar make
echo ""
echo "[2/4] Verificando make..."
if command -v make &> /dev/null; then
    echo "✓ make encontrado: $(make --version | head -n 1)"
else
    echo "⚠ make no encontrado (opcional)"
fi

# Compilar
echo ""
echo "[3/4] Compilando proyecto..."
if make clean && make; then
    echo "✓ Compilacion exitosa"
else
    echo "✗ ERROR en la compilacion"
    exit 1
fi

# Verificar ejecutable
echo ""
echo "[4/4] Verificando ejecutable..."
if [ -f "coloretto" ]; then
    echo "✓ Ejecutable 'coloretto' creado correctamente"
    ls -lh coloretto
else
    echo "✗ ERROR: No se creo el ejecutable"
    exit 1
fi

echo ""
echo "=========================================="
echo "  ✓ TODO LISTO PARA EJECUTAR"
echo "=========================================="
echo ""
echo "Para ejecutar el juego:"
echo "  ./coloretto"
echo ""

