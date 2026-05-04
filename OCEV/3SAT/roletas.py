import random

def roleta_sem_reposicao(populacao, fitness, num_select):
    selected = []

    pop = populacao[:]
    fits = fitness[:]

    for _ in range(num_select):
        fits
        total_fit = sum(fits)

        if total_fit == 0:
            # fallback: escolha aleatória
            idx = random.randint(0, len(pop) - 1)
        else:
            r = random.uniform(0, total_fit)
            acc = 0

            for i, f in enumerate(fits):
                acc += f
                if acc >= r:
                    idx = i
                    break

        selected.append(pop[idx])
        pop.pop(idx)
        fits.pop(idx)
    return selected