import networkx as nx
from random import randrange

import random

def get_random_index_with_one(matrix, row_index):
    row = matrix[row_index]
    indices_with_one = [index for index, value in enumerate(row) if value == 1]
    
    if not indices_with_one:
        return None  # Если в строке нет элементов, равных 1

    return random.choice(indices_with_one)

# Пример матрицы
matrix = [
    [1, 1, 0, 1, 1, 1, 1],
    [0, 0, 1, 1, 0, 0, 0],
    [1, 1, 1, 1, 1, 1, 0],
    [1, 0, 1, 0, 1, 1, 1],
    [1, 0, 0, 0, 0, 1, 1],
    [1, 0, 0, 1, 0, 0, 1],
    [0, 0, 1, 1, 0, 1, 0],
    [0, 1, 0, 1, 1, 1, 0],
    [1, 0, 1, 1, 0, 0, 0],
    [1, 1, 1, 1, 1, 0, 0],
    [1, 1, 0, 0, 1, 0, 1],
    [1, 1, 0, 0, 1, 1, 1],
    [1, 0, 1, 1, 1, 1, 0],
    [0, 0, 0, 1, 0, 1, 1],
    [1, 1, 0, 1, 0, 0, 1],
    [1, 1, 1, 0, 1, 1, 0]
]

# Выбор строки для примера
row_index = 0

# Получение случайного индекса столбца, элемент которого равен 1
random_index = get_random_index_with_one(matrix, row_index)
print(random_index)

