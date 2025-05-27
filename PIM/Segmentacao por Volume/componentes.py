class Celula:
    def __init__(self, img, x, y):
        self.img = img
        self.x = x
        self.y = y
        self.visitada = False
        self.componente = None

class ComponenteConexo:
    def __init__(self, elementos, tipo):
        self.elementos = elementos
        self.tipo = tipo

def contar_celulas(matriz):
    proliferativas = {
        "tipo": "PROLIFERATIVA",
        "elementos": []
    }
    quiescentes = {
        "tipo": "QUIESCENTE",
        "elementos": []
    }
    necroticas = {
        "tipo": "NECROTICA",
        "elementos": []
    }

    for img in range(len(matriz)):
        for x in range(len(matriz[0])):
            for y in range(len(matriz[0][0])):
                tipoCelula = matriz[img][x][y]
                celula = Celula(img, x, y)
                if tipoCelula == 140:
                    necroticas["elementos"].append(celula)
                elif tipoCelula == 200:
                    quiescentes["elementos"].append(celula)
                elif tipoCelula == 255:
                    proliferativas["elementos"].append(celula)

    print("Número de células Proliferativas:", len(proliferativas["elementos"]))
    print("Número de células Quiescentes:", len(quiescentes["elementos"]))
    print("Número de células Necróticas:", len(necroticas["elementos"]))

    return proliferativas, quiescentes, necroticas

def obter_vizinhos(matriz, img, x, y):
    vizinhos = []
    profundidade = len(matriz)
    linhas = len(matriz[0])
    colunas = len(matriz[0][0])

    for dz in [-1, 0, 1]:
        for dx in [-1, 0, 1]:
            for dy in [-1, 0, 1]:
                if dx == 0 and dy == 0 and dz == 0:
                    continue  # Ignora a própria célula
                ni, nx, ny = img + dz, x + dx, y + dy
                if 0 <= ni < profundidade and 0 <= nx < linhas and 0 <= ny < colunas:
                    vizinhos.append((ni, nx, ny))
    return vizinhos

def componentizar_celulas(celulas_dict, matriz):
    componentes = []
    mapa = {}  # (img, x, y) -> objeto Celula

    for celula in celulas_dict["elementos"]:
        chave = (celula.img, celula.x, celula.y)
        mapa[chave] = celula

    def dfs(celula, componente):
        celula.visitada = True
        celula.componente = componente
        componente.elementos.append(celula)

        vizinhos = obter_vizinhos(matriz, celula.img, celula.x, celula.y)

        for viz_coord in vizinhos:
            vizinho = mapa.get(viz_coord)
            if vizinho and not vizinho.visitada:
                dfs(vizinho, componente)

    for celula in celulas_dict["elementos"]:
        if not celula.visitada:
            componente = ComponenteConexo([], celulas_dict["tipo"])
            dfs(celula, componente)
            componentes.append(componente)

    max_componente = componentes[0]
    cont = 0

    print(f"Distribuição dos componentes:")
    for i, comp in enumerate(componentes):
        print(f"Componente {i}: {len(componentes[i].elementos)}")
        if len(componentes[i].elementos) > len(max_componente.elementos):
            max_componente = componentes[i]
        cont += (len(comp.elementos))

    print(f"Soma dos componentes do tipo {celulas_dict["tipo"]}: {cont}")
    print(f"Maior componente do tipo {celulas_dict["tipo"]} tem tamanho: {len(max_componente.elementos)}")
    
    return max_componente