def pesquisaBinaria(array, num):
    return pesqBin(array, num, 0, len(array) - 1, 0)

def pesqBin(array, num, e, d, num_chamadas):
    num_chamadas += 1
    meio = int((e + d) / 2)
    if e > d:
        return num_chamadas
    elif array[meio] == num:
        return num_chamadas
    elif num < array[meio]:
        d = meio
        return pesqBin(array, num, e, d, num_chamadas)
    elif num > array[meio]:
        e = meio
        return pesqBin(array, num, e, d, num_chamadas)