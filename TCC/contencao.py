import time
import os
import sys
from concurrent.futures import ProcessPoolExecutor

NUM_WORKERS = 8
ITERATIONS = 2000000

nome_arquivo = os.path.basename(__file__)
gil_enabled = getattr(sys, "_is_gil_enabled", lambda: "unknown")()

def worker(_):
    local_counter = 0
    for _ in range(ITERATIONS):
        local_counter += 1
    return local_counter

def run_test():
    start = time.time()

    with ProcessPoolExecutor(max_workers=NUM_WORKERS) as executor:
        results = list(executor.map(worker, range(NUM_WORKERS)))

    total = sum(results)

    end = time.time()

    print(f"Resultado: {total}")
    print(f"Tempo: {end - start:.4f}s")

if __name__ == "__main__":
    run_test()