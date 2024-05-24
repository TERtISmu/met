#!/bin/bash

# Компиляция файла test.cpp с помощью gcc и линковка с библиотекой стандартного C++
gcc main.cpp -o main.exe -lstdc++

# Запуск исполняемого файла main.exe
./main.exe

py testt.py