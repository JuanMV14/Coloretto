@echo off
setlocal EnableExtensions
pushd "%~dp0"

set "SRC_DIR=src"
set "INC_DIR=include"
set "OBJ_DIR=obj"

if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

set "GPP=C:\mingw64\bin\g++.exe"

if not exist "%GPP%" (
  echo.
  echo ERROR: No se encuentra el compilador en %GPP%
  echo.
  echo Ejecuta: descargar_mingw.ps1 o descargar_mingw.bat
  echo.
  pause
  popd
  exit /b 1
)

echo ================= Compilando Coloretto =================
echo.

"%GPP%" -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\main.cpp" -o "%OBJ_DIR%\main.o"
if errorlevel 1 goto :error

"%GPP%" -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\game_logic.cpp" -o "%OBJ_DIR%\game_logic.o"
if errorlevel 1 goto :error

"%GPP%" -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\player.cpp" -o "%OBJ_DIR%\player.o"
if errorlevel 1 goto :error

"%GPP%" -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\deck.cpp" -o "%OBJ_DIR%\deck.o"
if errorlevel 1 goto :error

"%GPP%" -o "coloretto.exe" "%OBJ_DIR%\main.o" "%OBJ_DIR%\game_logic.o" "%OBJ_DIR%\player.o" "%OBJ_DIR%\deck.o"
if errorlevel 1 goto :error

echo.
echo [OK] Compilacion exitosa. Ejecuta: coloretto.exe
echo.
popd
exit /b 0

:error
echo.
echo [ERROR] Fallo la compilacion.
echo.
popd
exit /b 1


