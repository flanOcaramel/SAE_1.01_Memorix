@echo off
chcp 65001 >nul
cd /d "%~dp0"

set "GCC=C:\msys64\ucrt64\bin\gcc.exe"
if not exist "%GCC%" set "GCC=gcc"

echo Compilation...
rem Le projet est decoupe en 4 fichiers .c : il faut tous les compiler,
rem sinon l'editeur de liens ne trouve pas les fonctions des autres modules.
"%GCC%" -Wall -Wextra -std=c17 -Iinclude src\main.c src\jeu.c src\creation.c src\ui.c -o build\memorix.exe
if errorlevel 1 goto erreur

echo.
if not exist build\memorix.exe goto erreur_fichier

build\memorix.exe
echo.
echo Partie terminee.
pause
exit /b 0

:erreur
echo.
echo ECHEC DE LA COMPILATION. Lis les messages ci-dessus.
pause
exit /b 1

:erreur_fichier
echo.
echo ECHEC : build\memorix.exe n'a pas ete cree.
pause
exit /b 1
