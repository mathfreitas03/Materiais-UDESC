import os
import sys
import time
import threading

def countdown(n):
    while n > 0:
        n -= 1

num_cores = os.cpu_count()

start = time.time()

threads = []

for i in range(num_cores - 1):
  thread = threading.Thread(target=countdown, args=(1000000000,))
  threads.append(thread)
  thread.start()

for t in threads:
   t.join()

end = time.time()

print('GIL habilitado: ' + str(sys._is_gil_enabled()))
print('Número de núcleos usados: ' + str(num_cores - 1))
print('Tempo decorrido: ' + str(end-start))
print('=============================')