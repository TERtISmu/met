import networkx as nx
from random import randrange

import random

# Чтение файла 'results'
with open('results', 'r') as file:
    # Считываем содержимое файла и разбиваем строку на отдельные элементы
    contents = file.readline().strip().split()

# Присваиваем значения переменным
graph_number = int(contents[0])
by_exhaust_alg = int(contents[1])
by_alg1 = int(contents[2])
by_alg1_percent = int(contents[3])
by_alg2 = int(contents[4])
by_alg2_percent = int(contents[5])

# Выводим значения переменных в консоль
print(f'graph_number = {graph_number}')
print(f'by_exhaust_alg = {by_exhaust_alg}')
print(f'by_alg1 = {by_alg1}')
print(f'by_alg1_percent = {by_alg1_percent}')
print(f'by_alg2 = {by_alg2}')
print(f'by_alg2_percent = {by_alg2_percent}')



