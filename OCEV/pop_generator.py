
# This script generates a population of individuals with random attributes and saves it to a file.

# Funcionalidades:
# Representação: Binária, Inteira e Real
# Tipo da representação (COD = BIN, INT, INT-PERM ou REAL)
# Tamanho da população (POP)
# Tamanho do cromossomo (DIM)

import argparse
import random

parser = argparse.ArgumentParser()

parser.add_argument("--cod", type=str, required=True, choices=["BIN", "INT", "INT-PERM", "REAL"])
parser.add_argument("--pop", type=int, required=True)
parser.add_argument("--dim", type=int, required=True)
parser.add_argument("--bounds", type=float, nargs=2, default=[-10, 10])

args = parser.parse_args()

def generatePopulation(codification, population_size, dimension_size, bounds):  
    population = []
    match codification:
        case "BIN":
            cromossomo = []
            for j in range(population_size):
                cromossomo = []
                for i in range(dimension_size):
                    cromossomo.append(random.randint(0, 1))
                population.append(cromossomo)  
        case "INT":
            cromossomo = []
            for j in range(population_size):
                for i in range(dimension_size):
                    cromossomo.append(random.randint(int(bounds[0]), int(bounds[1])))
                population.append(cromossomo)   
        case "INT-PERM":
            for j in range(population_size):    
                cromossomo = list(range(dimension_size))
                random.shuffle(cromossomo)
                population.append(cromossomo)
        case "REAL":   
            for j in range(population_size):
                cromossomo = []
                for i in range(dimension_size):
                    cromossomo.append(random.uniform(bounds[0], bounds[1]))
                population.append(cromossomo)
    return population

if __name__ == "__main__":
    population = generatePopulation(args.cod, args.pop, args.dim, args.bounds)
    print(population)