#!/bin/bash

# Функция для удаления файлов в папке
delete_files_in_folder() {
  folder_path=$1
  if [ -d "$folder_path" ]; then
    # Удаляем все файлы и папки внутри заданной папки
    rm -rf "$folder_path"/*
    echo "Все файлы в папке $folder_path удалены."
  else
    echo "Папка $folder_path не существует."
  fi
}

# Указываем пути к папкам
folders=(
  "./graphs"
  "./lists"
  "./colorings"
)

# Удаляем файлы в каждой из указанных папок
for folder in "${folders[@]}"; do
  delete_files_in_folder "$folder"
done

rm "./not_solved_graphs_by_alg1"

rm "./not_solved_graphs_by_alg2"

rm "./results"

# Компиляция файла test.cpp с помощью gcc и линковка с библиотекой стандартного C++
gcc main.cpp -o main.exe -lstdc++

echo ""
echo "Compilation finished :)"
echo ""
echo "running code..."
echo ""

# Запуск исполняемого файла main.exe
./main.exe

py tabucol.py