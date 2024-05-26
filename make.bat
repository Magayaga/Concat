@echo off
REM Set the name of the output executable
set OUTPUT_FILE=concat.exe

REM Set the name of the source file
set SOURCE_FILE=src\color.c src\concat.c

REM Compilation command
gcc -o %OUTPUT_FILE% %SOURCE_FILE%

REM Check if compilation was successful
if %ERRORLEVEL% EQU 0 (
    echo Compilation successful. Concat command-based utility.
) else (
    echo Compilation failed!
)
