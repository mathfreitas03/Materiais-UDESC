import pop_generator as generator
import pandas as pd

lx_min = 0
lx_max = 16

st_min = 0
st_max = 24

sts = []
lxs = []

r = -1

p0 = generator.generatePopulation(codification="BIN", population_size=10, dimension_size=10)

for ind in p0:
    lx = []
    st = []
    for i in range(0, 5):
        st.append(ind[i])
    
    sts.append(st)

    for j in range(5, len(ind)):
        lx.append(ind[j])

    lxs.append(lx)

    print("LX: ", lx)
    print("ST: ", st)

def f_obj_norm(st, lx):
    return (30 * st + 40 * lx)/1360

def h_norm(st, lx):
    return max(0, ((st + 2 * lx - 40)/16))

def fitness(st, lx):
    return f_obj_norm(st, lx) + r * h_norm(st, lx)

def binario_para_inteiro(bits) -> int:
    v = 0
    for b in bits:
        v = (v << 1) | (b & 1)
    return v

p0df = pd.DataFrame(p0)

lxs_int = []
sts_int = []

for bin in lxs:
    integer = binario_para_inteiro(bin)
    lxs_int.append(integer)

for bin in sts:
    integer = binario_para_inteiro(bin)
    sts_int.append(integer)

p0df["ST"] = sts
p0df["LX"] = lxs
p0df["ST_INT"] = sts_int
p0df["LX_INT"] = lxs_int

fitness_values = []

for i in range(len(p0)):
    fitness_values.append(fitness(sts_int[i], lxs_int[i]))

p0df["FIT"] = fitness_values

print(p0df)