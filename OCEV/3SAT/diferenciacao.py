
import random

def crossover(p1, p2):
    point = random.randint(1, len(p1) - 1)

    c1 = p1[:point] + p2[point:]
    c2 = p2[:point] + p1[point:]

    return c1, c2

def uniform_crossover(p1, p2):
    ind = []
    for i in range(len(p1)):
        lado = random.random()
        # pega o valor do ponto a
        if lado <= 0.5:
            ind.append(p1[i])
        else:
            # pega o valor do b
            ind.append(p2[i])
    return ind

def mutacao(ind, prob=0.01):
    for i in range(len(ind)):
        if random.random() < prob:
            ind[i] = 1 - ind[i]
    return ind