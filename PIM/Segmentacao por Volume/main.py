import componentes
import pickle
import sys
import plotly.graph_objects as go

sys.setrecursionlimit(10000)

with open("volume_TAC", "rb") as file:
    dados = pickle.load(file)

# Componentes que guardam o tipo e as coordenadas de cada célula

conjProliferativas, conjQuiescentes, conjNecroticas = componentes.contar_celulas(dados)

maiorComponenteProliferativas = componentes.componentizar_celulas(conjProliferativas, dados)
maiorComponenteQuiescentes = componentes.componentizar_celulas(conjQuiescentes, dados)
maiorComponenteNecroticas = componentes.componentizar_celulas(conjNecroticas, dados)

# grafico = plt.figure()
# ax = grafico.add_subplot(111, projection='3d')

# ax.scatter(maiorComponenteProliferativas.elementos[0].img, maiorComponenteProliferativas.elementos[0].x, maiorComponenteProliferativas.elementos[0].y, c="r", label="Células Proliferativas")

# for celula in maiorComponenteProliferativas.elementos[1:]:
#     ax.scatter(celula.img, celula.x, celula.y, c="r")

# ax.scatter(maiorComponenteQuiescentes.elementos[0].img, maiorComponenteQuiescentes.elementos[0].x, maiorComponenteQuiescentes.elementos[0].y, c="g", label="Células Quiescentes")

# for celula in maiorComponenteQuiescentes.elementos[1:]:
#     ax.scatter(celula.img, celula.x, celula.y, c="g")

# ax.scatter(maiorComponenteNecroticas.elementos[0].img, maiorComponenteNecroticas.elementos[0].x, maiorComponenteNecroticas.elementos[0].y, c="b", label="Células Necróticas")

# for celula in maiorComponenteNecroticas.elementos[1:]:
#     ax.scatter(celula.img, celula.x, celula.y, c="b")

# ax.set_title('Maiores Componentes Conexos de Cada Tipo')
# ax.set_xlabel('Imagem')
# ax.set_ylabel('X')
# ax.set_zlabel('Y')

# ax.legend()

def extrair_coords(componentes):
    return (
        [c.img for c in componentes.elementos],
        [c.x for c in componentes.elementos],
        [c.y for c in componentes.elementos]
    )

x1, y1, z1 = extrair_coords(maiorComponenteProliferativas)
x2, y2, z2 = extrair_coords(maiorComponenteQuiescentes)
x3, y3, z3 = extrair_coords(maiorComponenteNecroticas)

fig = go.Figure()

fig.add_trace(go.Scatter3d(
    x=x1, y=y1, z=z1,
    mode='markers',
    marker=dict(size=3, color='red'),
    name='Células Proliferativas'
))

fig.add_trace(go.Scatter3d(
    x=x2, y=y2, z=z2,
    mode='markers',
    marker=dict(size=3, color='green'),
    name='Células Quiescentes'
))

fig.add_trace(go.Scatter3d(
    x=x3, y=y3, z=z3,
    mode='markers',
    marker=dict(size=3, color='blue'),
    name='Células Necróticas'
))

fig.update_layout(
    title='Maiores Componentes Conexos de Cada Tipo',
    scene=dict(
        xaxis_title='Imagem',
        yaxis_title='X',
        zaxis_title='Y'
    ),
    legend=dict(
        x=0, y=1
    ),
    margin=dict(l=0, r=0, b=0, t=40)
)

if __name__ == "__main__":
    fig.write_html("grafico3d.html")
    fig.show()
