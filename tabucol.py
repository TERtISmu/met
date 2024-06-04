from collections import deque
from random import randrange
import networkx as nx
import matplotlib.pyplot as plt
import random
import time
import numpy as np

def fitness(chromosome, graph, color_lists):
    conflicts = 0
    for i in range(len(graph)):
        for j in range(len(graph)):
            if(chromosome[i] >= len(color_lists[i]) or chromosome[j] >= len(color_lists[j])):
                conflicts += 1
            else:
                if ((graph[i][j] == 1 and chromosome[i] == chromosome[j]) or color_lists[i][chromosome[i]] == 0 or color_lists[j][chromosome[j]] == 0):
                    conflicts += 1
    return conflicts

def create_initial_population(color_lists, greedy_solution, population_size):
    population = []
    for _ in range(population_size):
        chromosome = [random.randint(1, 10) for _ in range(len(color_lists))]
        for i in range(len(color_lists)):
            if (i <= len(greedy_solution) - 1):
                chromosome[i] = greedy_solution[i]
            else:
                chromosome[i] = get_random_index(color_lists, i)
        random.shuffle(chromosome)
        population.append(chromosome)
    return population

def selection(population, fitnesses):
    selected = random.choices(population, weights=[1/f for f in fitnesses], k=2)
    return selected

def crossover(parent1, parent2):
    point = random.randint(0, len(parent1)-1)
    child1 = parent1[:point] + parent2[point:]
    child2 = parent2[:point] + parent1[point:]
    return child1, child2

def mutate(chromosome, mutation_rate, color_lists):
    if random.random() < mutation_rate:
        index = random.randint(0, len(chromosome)-1)
        chromosome[index] = get_random_index(color_lists, index)
    return chromosome

def genetic_algorithm(graph, greedy_coloring, color_lists, population_size=200, generations=20, mutation_rate=0.09):
    population = create_initial_population(color_lists, greedy_coloring, population_size)
    best_chromosome = None
    best_fitness = float('inf')

    for generation in range(generations):
        fitnesses = [fitness(chromo, graph, color_lists) for chromo in population]

        for i, fit in enumerate(fitnesses):
            if fit < best_fitness:
                best_fitness = fit
                best_chromosome = population[i]

        if best_fitness == 0:
            break

        new_population = []
        for _ in range(population_size // 2):
            parent1, parent2 = selection(population, fitnesses)
            child1, child2 = crossover(parent1, parent2)
            new_population.append(mutate(child1, mutation_rate, color_lists))
            new_population.append(mutate(child2, mutation_rate, color_lists))

        population = new_population
    
    if best_fitness == 0:
        return best_chromosome
    else:
        return None


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

def tabucol(graph, alg1_coloring, color_lists, number_of_colors, tabu_size=7, reps=25, max_iterations=25, debug=False):
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

def test1(nx_graph, list_path, coloring_path, draw=False):
    graph = nx.adjacency_matrix(nx_graph)
    alg1_coloring = read_array_from_file(coloring_path)
    color_lists, numberofcolors = read_matrix_from_file(list_path)
    coloring = genetic_algorithm(graph.todense(), alg1_coloring, color_lists)
    
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
    genetic_after_alg1_colorizing_succeded_count = 0
    for idx, elem in enumerate(unsolved_graphs):
        graph_path = f'graphs/graphml{elem}'
        list_path = f'lists/list{elem}'
        coloring_path = f'colorings/coloring{elem}'
        
        G = nx.read_graphml(graph_path)

        if (test(G, list_path, coloring_path, False) == 1):
            after_alg1_colorizing_succeded_count += 1
        
        if (test1(G, list_path, coloring_path, False) == 1):
            genetic_after_alg1_colorizing_succeded_count += 1
    
    unsolved_graphs = get_unsolved_graphs('./not_solved_graphs_by_alg2')
    after_alg2_colorizing_succeded_count = 0
    genetic_after_alg2_colorizing_succeded_count = 0
    for idx, elem in enumerate(unsolved_graphs):
        graph_path = f'graphs/graphml{elem}'
        list_path = f'lists/list{elem}'
        coloring_path = f'colorings/coloring{elem}'
        
        G = nx.read_graphml(graph_path)

        if (test(G, list_path, coloring_path, False) == 1):
            after_alg2_colorizing_succeded_count += 1
        
        if (test1(G, list_path, coloring_path, False) == 1):
            genetic_after_alg2_colorizing_succeded_count += 1

    
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
    
    alg1_genetic = by_alg1 + genetic_after_alg1_colorizing_succeded_count
    alg2_genetic = by_alg2 + genetic_after_alg2_colorizing_succeded_count
    
    print('alg1 with genetic:', alg1_genetic, "{:.2f}".format(alg1_genetic / by_exhaust_alg * 100), "%")
    print('alg2 with genetic:', alg2_genetic, "{:.2f}".format(alg2_genetic / by_exhaust_alg * 100), "%")

    end_time = time.time()

    elapsed_time = end_time - start_time
    print(f"The program ran for: {elapsed_time:.6f} seconds")
