import pandas as pd
import pop_generator as generator

# EX 1: Gerar a população inicial.
# 	COD = ?
# 	POP=10
# DIM=15		
# 	. BIN
# 	. INT - Bounds = [-5, 10]
# 	. INT-PERM
# 	. REAL - Bounds = [-10, 10]

if __name__ == "__main__":

    bin_population = generator.generatePopulation("BIN", 10, 15)
    int_population = generator.generatePopulation("INT", 10, 15, bounds=[-5, 10])
    int_perm_population = generator.generatePopulation("INT-PERM", 10, 15)
    real_population = generator.generatePopulation("REAL", 10, 15, bounds=[-10, 10])

    bin_df = pd.DataFrame(bin_population)
    int_df = pd.DataFrame(int_population)  
    int_perm_df = pd.DataFrame(int_perm_population)
    real_df = pd.DataFrame(real_population)

    print("Binary Population:")
    print(bin_df)
    print("\nInteger Population:")
    print(int_df)
    print("\nInteger Permutation Population:")
    print(int_perm_df)
    print("\nReal Population:")
    print(real_df)