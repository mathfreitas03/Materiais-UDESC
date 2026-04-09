import os
import sys
import time
import multiprocessing

def countdown(n):
    while n > 0:
        n -= 1

num_cores = os.cpu_count()

if __name__ == "__main__":
   
    start = time.time()

    processos = []

    for i in range(num_cores - 1):
        processo = multiprocessing.Process(target=countdown, args=(1000000000,))
        processos.append(processo)
        processo.start()

    for t in processos:
        t.join()

    end = time.time()

    print('GIL habilitado: ' + str(sys._is_gil_enabled()))
    print('Número de núcleos usados: ' + str(num_cores - 1))
    print('Tempo decorrido: ' + str(end-start))
    print('=============================')