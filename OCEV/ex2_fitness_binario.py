import random
import pop_generator as generator
import pandas as pd
import math

# Gerar população
p0 = generator.generatePopulation(codification="BIN", population_size=10, dimension_size=16)
p0df = pd.DataFrame(p0)

x_min = -2
x_max = 2

def f_obj(x):
    return math.cos(20.0 * x) - abs(x) / 2.0 + (x**3) / 4.0

def fitness_max(x): 
    return 4 + f_obj(x)

def fitness_min(x):
    return 2 - f_obj(x)

def binario_para_inteiro(bits) -> int:
    v = 0
    for b in bits:
        v = (v << 1) | (b & 1)
    return v

def decodificar_x(bits, x_min, x_max):
    if not bits:
        return x_min
    l = len(bits)
    max_int = (1 << l) - 1
    k = binario_para_inteiro(bits)
    if max_int == 0:
        return x_min
    
    x = x_min + (x_max - x_min) * (k / max_int)
    return round(x, 4)  

xs = []
f_vals = []

for ind in p0:
    x = decodificar_x(ind, x_min, x_max)
    xs.append(x)
    f_vals.append(round(f_obj(x), 4))
    f_scaled = (f_obj(x) + 2) / (2 - (-4))

# Adicionar ao DataFrame
p0df["x_decodificado"] = xs
p0df["f_obj(x)"] = f_vals
p0df["fitness_max"] = [round(fitness_max(x), 4) for x in xs]
p0df["fitness_min"] = [round(fitness_min(x), 4) for x in xs]

print(p0df)