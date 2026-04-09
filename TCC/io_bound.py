import threading
import time
import os
import requests

URL = "https://httpbin.org/delay/2"

def io_task(task_id):
    print(f"Tarefa {task_id} iniciada")
    response = requests.get(URL)
    print(f"Tarefa {task_id} finalizada")

num_threads = 30
threads = []

start = time.time()

for i in range(num_threads):
    t = threading.Thread(target=io_task, args=(i,))
    threads.append(t)
    t.start()

for t in threads:
    t.join()

end = time.time()

print("\nTempo total:", end - start)
