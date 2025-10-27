# Instrucciones para Ejecutar en Windows

## Pasos para el Dia de la Presentacion (Windows)

### Opcion A: Si las Computadoras YA Tienen gcc

**Verificar primero:**
```powershell
gcc --version
```

Si funciona, continua al Paso 2.

---

### Opcion B: Si NO Tienen gcc Instalado

#### 1. Llevar MinGW en USB

Copia esta carpeta completa en una USB:
```
C:\mingw64\bin\gcc.exe\mingw64\
```

En el laboratorio:
1. Copia la carpeta `mingw64` a `C:\` de la computadora
2. Abre PowerShell y ejecuta:
```powershell
$env:Path = "C:\mingw64\bin;" + $env:Path
```

---

### Paso 2: Clonar el Repositorio

```powershell
cd Desktop
git clone https://github.com/JuanMV14/Coloretto.git
cd Coloretto
```

---

### Paso 3: Compilar

**NO hay make en Windows**, asi que compila manualmente:

```powershell
gcc -Wall -Wextra -Iinclude src/*.c -o coloretto.exe
```

Si sale un warning sobre "comparison", es normal, no afecta.

---

### Paso 4: Ejecutar

```powershell
Start-Process -FilePath ".\coloretto.exe" -NoNewWindow -Wait
```

O simplemente haz doble click en `coloretto.exe`

---

## Plan B: Llevar el Ejecutable Ya Compilado

Si en el laboratorio NO tienen gcc y no puedes instalarlo:

1. Lleva el archivo `coloretto.exe` en tu USB
2. Copialo a la computadora
3. Haz doble click para ejecutar

**IMPORTANTE:** El .exe solo funciona en Windows de 64 bits.

---

## Verificar Antes de la Presentacion

**1 dia antes**, ve al laboratorio y prueba:

```powershell
# Verificar gcc
gcc --version

# Si funciona, prueba compilar
cd Desktop
git clone https://github.com/JuanMV14/Coloretto.git
cd Coloretto
gcc -Wall -Wextra -Iinclude src/*.c -o coloretto.exe
Start-Process -FilePath ".\coloretto.exe" -NoNewWindow -Wait
```

---

## Resumen Rapido

### Si tienen gcc:
```powershell
git clone https://github.com/JuanMV14/Coloretto.git
cd Coloretto
gcc -Wall -Wextra -Iinclude src/*.c -o coloretto.exe
.\coloretto.exe
```

### Si NO tienen gcc:
- Lleva MinGW en USB, o
- Lleva `coloretto.exe` ya compilado

---

## Contacto de Emergencia

- Repositorio: https://github.com/JuanMV14/Coloretto
- Ejecutable ya compilado en tu computadora: `C:\Users\Contact Cemter UR\Coloretto\coloretto.exe`

