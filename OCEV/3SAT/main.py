from pop_generator import generatePopulation
from roletas import roleta_sem_reposicao
from diferenciacao import crossover, mutacao, uniform_crossover
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor
import matplotlib.pyplot as plt
import random
import sys
import os
import time
import pandas as pd

# Leitura do arquivo

with open("uf100-01.cnf.txt", "r") as arq:
    for line in arq:
        if line.startswith("p"):
            _, _, num_vars, num_clauses = line.split()
            num_vars, num_clauses = int(num_vars), int(num_clauses)
            break

pop = generatePopulation("BIN", 50, num_vars)

clauses = []
with open("uf100-01.cnf.txt", "r") as arq:
    for line in arq:
        if line.startswith("p") or line.startswith("c"):
            continue
        if line.startswith("%"):
            break
        clause = [int(x) for x in line.split() if int(x) != 0]
        if clause:
            clauses.append(clause)


# Avaliação e fitness

def avaliar_individuo(individuo, clauses):
    satisfeito = 0
    for clause in clauses:
        for literal in clause:
            var_index = abs(literal) - 1
            if (literal > 0 and individuo[var_index] == 1) or \
               (literal < 0 and individuo[var_index] == 0):
                satisfeito += 1
                break
    return satisfeito

def fitness(individuo, clauses, alpha=1):
    satisfeito = avaliar_individuo(individuo, clauses)
    nao_satisfeito = len(clauses) - satisfeito
    return satisfeito # - alpha * nao_satisfeito

# Otimizações no paralelismo

GLOBAL_CLAUSES = None

def init_worker(clauses):
    global GLOBAL_CLAUSES
    GLOBAL_CLAUSES = clauses

def fitness_worker(individuo):
    return fitness(individuo, GLOBAL_CLAUSES)

def run_ga(pop, clauses, generations=50, crossover_rate=0.8, mutation_rate=0.05, alpha=1):
    history = []
    pop_atual = pop

    best_global = None
    best_global_fitness = 0

    t0 = time.time()

    num_workers = max(1, os.cpu_count() - 1)

    # f not sys._is_gil_enabled():
    #    executor = ThreadPoolExecutor(max_workers=num_workers)
    #    use_threads = True
    # else:
    executor = ProcessPoolExecutor(
        max_workers=num_workers,
        initializer=init_worker,
        initargs=(clauses,)
    )
    use_threads = False

    for gen in range(generations):

        # Avaliação da fitness
        if use_threads:
            fitness_values = list(executor.map(lambda ind: fitness(ind, clauses), pop_atual))
        else:
            fitness_values = list(executor.map(fitness_worker, pop_atual))

        # Estatisticas para o grafico

        melhor = max(fitness_values)
        media = sum(fitness_values) / len(fitness_values)
        pior = min(fitness_values)

        melhor_idx = fitness_values.index(melhor)
        melhor_ind = pop_atual[melhor_idx]

        if melhor > best_global_fitness:
            best_global_fitness = melhor
            best_global = melhor_ind[:]

        history.append({
            "geracao": gen,
            "melhor": melhor,
            "media": media,
            "pior": pior,
            "melhor_global": best_global_fitness,
            "alpha": alpha
        })

        pop_intermediaria = roleta_sem_reposicao(
            pop_atual,
            fitness_values,
            num_select=len(pop_atual)
        )

        new_pop = []
        i = 0

        while i < len(pop_intermediaria):
            p1 = pop_intermediaria[i]

            if i + 1 < len(pop_intermediaria):
                p2 = pop_intermediaria[i + 1]
            else:
                new_pop.append(p1[:])
                break

            # Crossover
            # if random.random() < crossover_rate:
            #     children = crossover(p1, p2)
            #     if isinstance(children, (tuple, list)) and len(children) == 2:
            #         c1, c2 = children
            #     else:
            #         c1 = children
            #         c2 = p2[:]
            # else:
            #     c1, c2 = p1[:], p2[:]
            if random.random() < crossover_rate:
                children = uniform_crossover(p1, p2)
                if isinstance(children, (tuple, list)) and len(children) == 2:
                    c1, c2 = children
                else:
                    c1 = children
                    c2 = p2[:]
            else:
                c1, c2 = p1[:], p2[:]

            # Mutação
            if random.random() < mutation_rate:
                c1 = mutacao(c1)
            if random.random() < mutation_rate:
                c2 = mutacao(c2)

            new_pop.extend([c1, c2])
            i += 2

        new_pop[0] = melhor_ind[:]
        pop_atual = new_pop


    executor.shutdown()

    tf = time.time()
    modo = "threads (nogil)" if use_threads else "processos"
    print(f"Tempo total ({modo}): {tf - t0:.2f} segundos")
    return pop_atual, pd.DataFrame(history), best_global, best_global_fitness

# Execuçao

if __name__ == "__main__":
    final_pop, history, best_global, best_fit = run_ga(
        pop,
        clauses,
        generations=3000,
        crossover_rate=0.8,
        mutation_rate=0.05,
        alpha=1
    )

    print("Melhor fitness global:", best_fit)

    # Gráfico
    plt.plot(history["geracao"], history["melhor"], label="Melhor", color="#00CA0DFF")
    plt.plot(history["geracao"], history["media"], label="Média", color="orange")
    plt.plot(history["geracao"], history["pior"], label="Pior", color="#BE1414")
    plt.axhline(y = num_clauses * history["alpha"][0], label= "Ótimo", color="#1414BE", linestyle="--")
    plt.xlabel("Geração")
    plt.ylabel("Fitness")
    plt.title("Convergência do GA")
    plt.legend()
    plt.grid()
    plt.show()