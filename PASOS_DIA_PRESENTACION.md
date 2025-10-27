# Pasos para el Dia de la Presentacion (WINDOWS)

## Antes de Llegar al Laboratorio

1. Verifica que tu repositorio este actualizado en GitHub:
   - URL: https://github.com/JuanMV14/Coloretto

2. Lleva en USB como respaldo:
   - La carpeta: C:\mingw64\bin\gcc.exe\mingw64\
   - El ejecutable: coloretto.exe

## Al Llegar al Laboratorio

### Paso 1: Verificar que gcc este instalado

Abre PowerShell y escribe:
```powershell
gcc --version
```

Si aparece la version de gcc, continua al Paso 2.

Si NO aparece, consulta INSTRUCCIONES_WINDOWS.md

### Paso 2: Clonar el repositorio

```powershell
cd Desktop
git clone https://github.com/JuanMV14/Coloretto.git
cd Coloretto
```

### Paso 3: Compilar el proyecto

**Windows NO tiene make**, compila manualmente:

```powershell
gcc -Wall -Wextra -Iinclude src/*.c -o coloretto.exe
```

Si aparece un warning sobre "comparison", es normal.

### Paso 4: Ejecutar el juego

```powershell
.\coloretto.exe
```

O haz doble click en el archivo coloretto.exe

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

