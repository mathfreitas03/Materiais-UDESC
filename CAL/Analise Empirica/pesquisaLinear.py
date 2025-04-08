def pesquisaLinear(array, n):
    num_iteracoes = 0;
    for i in range(0, len(array) - 1):
        num_iteracoes += 1
        if array[i] == n:
            return num_iteracoes
    return num_iteracoes
