import random
import plotly.express as px
import pandas as pd
from pesquisaLinear import pesquisaLinear
from pesquisaBinaria import pesquisaBinaria

random.seed()

dados = {
    "Tamanho": [],
    "Iterações": [],
    "Algoritmo": []
}

def gerar_array_ordenado(n):
    return list(range(1, n + 1))

tamanhos = [10 * (2 ** i) for i in range(8)]

for tamanho in tamanhos:
    array = gerar_array_ordenado(tamanho)
    num = random.randint(1, len(array) + 1)

    # Pesquisa Linear
    iter_linear = pesquisaLinear(array, num)
    dados["Tamanho"].append(len(array))
    dados["Iterações"].append(iter_linear)
    dados["Algoritmo"].append("Pesquisa Linear")

    # Pesquisa Binária
    iter_binaria = pesquisaBinaria(array, num)
    dados["Tamanho"].append(len(array))
    dados["Iterações"].append(iter_binaria)
    dados["Algoritmo"].append("Pesquisa Binária")

df = pd.DataFrame(dados)

fig = px.line(
    df, 
    x="Tamanho", 
    y="Iterações", 
    color="Algoritmo", 
    title="Análise Empírica de Algoritmos de Busca", 
    height=400, 
    width=1000, 
    line_shape="spline", 
    markers=True
)

fig.show()
