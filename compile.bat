@echo off
setlocal EnableExtensions EnableDelayedExpansion
pushd "%~dp0"

set "SRC_DIR=src"
set "INC_DIR=include"
set "OBJ_DIR=obj"

if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%" >nul 2>&1

rem Detectar g++ en PATH o en rutas comunes
set "GPP_PATH="
where g++ >nul 2>&1 && set "GPP_PATH=g++"
if not defined GPP_PATH if exist "C:\msys64\mingw64\bin\g++.exe" set "GPP_PATH=C:\msys64\mingw64\bin\g++.exe"
if not defined GPP_PATH if exist "C:\mingw64\bin\g++.exe" set "GPP_PATH=C:\mingw64\bin\g++.exe"

rem Si no hay g++, intentar con gcc en modo C++
set "USE_GCC=0"
set "GCC_PATH="
if not defined GPP_PATH (
  where gcc >nul 2>&1 && for /f "delims=" %%i in ('where gcc') do set "GCC_PATH=%%i"
  if not defined GCC_PATH if exist "C:\msys64\mingw64\bin\gcc.exe" set "GCC_PATH=C:\msys64\mingw64\bin\gcc.exe"
  if not defined GCC_PATH if exist "C:\mingw64\bin\gcc.exe" set "GCC_PATH=C:\mingw64\bin\gcc.exe"
  if defined GCC_PATH set "USE_GCC=1"
)

rem Sanitizar rutas (quitar comillas si vinieron con ellas)
if defined GPP_PATH set "GPP_PATH=%GPP_PATH:\"=%"
if defined GPP_PATH set "GPP_PATH=%GPP_PATH:"=%"
if defined GCC_PATH set "GCC_PATH=%GCC_PATH:\"=%"
if defined GCC_PATH set "GCC_PATH=%GCC_PATH:"=%"

if not defined GPP_PATH if "%USE_GCC%" NEQ "1" (
  echo [ERROR] No se encontro g++ ni gcc. Instala MSYS2/MinGW y vuelve a intentar.
  popd
  exit /b 1
)

echo ================= Compilando Coloretto (C++) =================

if "%USE_GCC%"=="1" (
  echo Usando GCC como C++: %GCC_PATH%
  %GCC_PATH% -Wall -Wextra -g -std=c++17 -x c++ -I"%INC_DIR%" -c "%SRC_DIR%\main.cpp" -o "%OBJ_DIR%\main.o" || goto :error
  %GCC_PATH% -Wall -Wextra -g -std=c++17 -x c++ -I"%INC_DIR%" -c "%SRC_DIR%\game_logic.cpp" -o "%OBJ_DIR%\game_logic.o" || goto :error
  %GCC_PATH% -Wall -Wextra -g -std=c++17 -x c++ -I"%INC_DIR%" -c "%SRC_DIR%\player.cpp" -o "%OBJ_DIR%\player.o" || goto :error
  %GCC_PATH% -Wall -Wextra -g -std=c++17 -x c++ -I"%INC_DIR%" -c "%SRC_DIR%\deck.cpp" -o "%OBJ_DIR%\deck.o" || goto :error

  %GCC_PATH% -o "coloretto.exe" ^
    "%OBJ_DIR%\main.o" "%OBJ_DIR%\game_logic.o" "%OBJ_DIR%\player.o" "%OBJ_DIR%\deck.o" -lstdc++ || goto :error
) else (
  echo Usando G++: %GPP_PATH%
  %GPP_PATH% -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\main.cpp" -o "%OBJ_DIR%\main.o" || goto :error
  %GPP_PATH% -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\game_logic.cpp" -o "%OBJ_DIR%\game_logic.o" || goto :error
  %GPP_PATH% -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\player.cpp" -o "%OBJ_DIR%\player.o" || goto :error
  %GPP_PATH% -Wall -Wextra -g -std=c++17 -I"%INC_DIR%" -c "%SRC_DIR%\deck.cpp" -o "%OBJ_DIR%\deck.o" || goto :error

  %GPP_PATH% -o "coloretto.exe" ^
    "%OBJ_DIR%\main.o" "%OBJ_DIR%\game_logic.o" "%OBJ_DIR%\player.o" "%OBJ_DIR%\deck.o" || goto :error
)

echo.
echo [OK] Compilacion exitosa. Ejecuta:
echo     coloretto.exe
echo.
popd
exit /b 0

:error
echo.
echo [ERROR] Fallo la compilacion. Revisa el mensaje anterior.
popd
exit /b 1


