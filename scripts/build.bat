@echo off
title build
echo DEBUGGING

"../mingw-w64/mingw64/bin/gcc.exe" -v -w -g "../src/main.c" -o "../build/game.exe" -I "../sdl/include" -L "../sdl/lib" -lmingw32 -lSDL2main -lSDL2
IF %ERRORLEVEL%==0 (
	echo BUILD SUCCESFUL.
	if exist "../build/game.exe" (
		echo starting...
		start "Active Process" "..\build\game.exe"
	) else ( 
		echo File does not exist
		pause >nul
	)
) else (
	pause >nul
)

exit
