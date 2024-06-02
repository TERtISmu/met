from collections import deque
from random import randrange
import networkx as nx
import matplotlib.pyplot as plt
import random
import time


def read_array_from_file(filename):
    with open(filename, 'r') as file:
        numberofvertex = int(file.readline().strip())
        array = list(map(int, file.readline().strip().split()))
    return array

def read_matrix_from_file(filename):
    with open(filename, 'r') as file:
        numberofvertex, numberofcolors = map(int, file.readline().strip().split())
        matrix = []
        for _ in range(numberofvertex):
            row = list(map(int, file.readline().strip().split()))
            matrix.append(row)
    return matrix, numberofcolors

def get_random_index(matrix, row_index):
    row = matrix[row_index]
    indices_with_one = [index for index, value in enumerate(row) if value == 1]
    if not indices_with_one:
        return None
    return random.choice(indices_with_one)

def get_unsolved_graphs(filename):
    with open(filename, 'r') as infile:
        data = infile.read()
    iterations = [int(i) for i in data.split()]
    return iterations

def tabucol(graph, alg1_coloring, color_lists, number_of_colors, tabu_size=7, reps=100, max_iterations=50, debug=False):
    iterations = 0
    tabu = deque()
    solution = dict()
    
    for i in range(len(color_lists)):
        if (i <= len(alg1_coloring) - 1):
            solution[i] = alg1_coloring[i]
        else:
            solution[i] = get_random_index(color_lists, i)
        
    aspiration_level = dict()

    while iterations < max_iterations:
        move_candidates = set() 
        conflict_count = 0
        for i in range(len(graph)):
            for j in range(i+1, len(graph)): 
                if graph[i][j] > 0: 
                    if solution[i] == solution[j]: 
                        move_candidates.add(i)
                        move_candidates.add(j)
                        conflict_count += 1
        move_candidates = list(move_candidates) 

        if conflict_count == 0:
            break

        new_solution = None
        for r in range(reps):
            node = move_candidates[randrange(0, len(move_candidates))]
            
            new_color = get_random_index(color_lists, node)
            if solution[node] == new_color:
                new_color = get_random_index(color_lists, node)

            new_solution = solution.copy()
            new_solution[node] = new_color
            new_conflicts = 0
            for i in range(len(graph)):
                for j in range(i+1, len(graph)):
                    if graph[i][j] > 0 and new_solution[i] == new_solution[j]:
                        new_conflicts += 1
            if new_conflicts < conflict_count:
                if new_conflicts <= aspiration_level.setdefault(conflict_count, conflict_count - 1):
                    aspiration_level[conflict_count] = new_conflicts - 1

                    if (node, new_color) in tabu:
                        tabu.remove((node, new_color))
                        if debug:
                            print("tabu permitted;", conflict_count, "->", new_conflicts)
                        break
                else:
                    if (node, new_color) in tabu:
                        continue
                if debug:
                    print (conflict_count, "->", new_conflicts)
                break

        tabu.append((node, solution[node]))
        if len(tabu) > tabu_size: 
            tabu.popleft() 

        solution = new_solution
        iterations += 1
        if debug and iterations % 500 == 0:
            print("iteration:", iterations)

    if conflict_count != 0:
        print("No coloring found with {} colors.".format(number_of_colors))
        return None
    else:
        print("Found coloring:\n", solution)
        return solution


def test(nx_graph, list_path, coloring_path, draw=False):
    graph = nx.adjacency_matrix(nx_graph)
    alg1_coloring = read_array_from_file(coloring_path)
    color_lists, numberofcolors = read_matrix_from_file(list_path)
    coloring = tabucol(graph.todense(), alg1_coloring, color_lists, numberofcolors, debug=False)
    
    if draw:
        values = list(coloring.values())
        nx.draw(nx_graph, node_color=values, with_labels=True, pos=nx.shell_layout(nx_graph))
        plt.show()

    if coloring != None:
        return 1
    else:
        return 0

if __name__ == "__main__":
    start_time = time.time()

    unsolved_graphs = get_unsolved_graphs('./not_solved_graphs_by_alg1')

    after_alg1_colorizing_succeded_count = 0
    for idx, elem in enumerate(unsolved_graphs):
        graph_path = f'graphs/graphml{elem}'
        list_path = f'lists/list{elem}'
        coloring_path = f'colorings/coloring{elem}'
        
        G = nx.read_graphml(graph_path)

        if (test(G, list_path, coloring_path, False) == 1):
            after_alg1_colorizing_succeded_count += 1
    
    unsolved_graphs = get_unsolved_graphs('./not_solved_graphs_by_alg2')
    after_alg2_colorizing_succeded_count = 0
    for idx, elem in enumerate(unsolved_graphs):
        graph_path = f'graphs/graphml{elem}'
        list_path = f'lists/list{elem}'
        coloring_path = f'colorings/coloring{elem}'
        
        G = nx.read_graphml(graph_path)

        if (test(G, list_path, coloring_path, False) == 1):
            after_alg2_colorizing_succeded_count += 1

    
    with open('results', 'r') as file:
        contents = file.readline().strip().split()
    
    graph_number = int(contents[0])
    by_exhaust_alg = int(contents[1])
    by_alg1 = int(contents[2])
    by_alg2 = int(contents[4])
    print('')
    alg1_tabucol = by_alg1 + after_alg1_colorizing_succeded_count
    alg2_tabucol = by_alg2 + after_alg2_colorizing_succeded_count
    print('alg1 with tabucol:', alg1_tabucol, "{:.2f}".format(alg1_tabucol / by_exhaust_alg * 100), "%")
    print('alg2 with tabucol:', alg2_tabucol, "{:.2f}".format(alg2_tabucol / by_exhaust_alg * 100), "%")

    end_time = time.time()

    elapsed_time = end_time - start_time
    print(f"The program ran for: {elapsed_time:.6f} seconds")
