import threading
import multiprocessing
import time
import math
import os

def primo(n):
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    limite = int(math.sqrt(n)) + 1
    for i in range(3, limite, 2):
        if n % i == 0:
            return False
    return True

def count_primos(start, end):
    count = 0
    for n in range(start, end):
        if primo(n):
            count += 1
    return count

NUM = 200_000
num_cores = os.cpu_count()

# Sequencial

start = time.time()
count_primos(0, NUM)
print("Sequencial:", time.time() - start)

# THREADS 

def thread_task(start, end):
    count_primos(start, end)

threads = []
chunk = NUM // num_cores

start = time.time()

for i in range(num_cores):
    s = i * chunk
    e = (i + 1) * chunk
    t = threading.Thread(target=thread_task, args=(s, e))
    threads.append(t)
    t.start()

for t in threads:
    t.join()

print("Threads:", time.time() - start)

# MULTIPROCESSING

def process_task(args):
    return count_primos(*args)

if __name__ == "__main__":
    start = time.time()

    with multiprocessing.Pool(num_cores) as pool:
        ranges = [(i * chunk, (i + 1) * chunk) for i in range(num_cores)]
        pool.map(process_task, ranges)

    print("Multiprocessing:", time.time() - start)
