import networkx as nx
from random import randrange

def read_array_from_file(filename):
    with open(filename, 'r') as file:
        numberofvertex = int(file.readline().strip())
        array = list(map(int, file.readline().strip().split()))
    return array

# Read the array from the file
filename = "array.txt"
array = read_array_from_file(filename)
print(array)



