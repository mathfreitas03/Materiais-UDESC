import re
import os
import sys

chamadas_heuristica = 0
chamadas_backtracking = 0

sys.setrecursionlimit(100000)

with open("mapa01.txt", "r") as file:
    lab = file.read()

linhas = lab.split('\n')

print(f"Digite 2 pontos entre 1 e 48 em x e 1 e {len(linhas) - 2} em y na forma \"(x1, y1) (x2, y2)\"")

entrada = input()
duplas = re.findall(r'\((\-?\d+),\s*(\-?\d+)\)', entrada)
p1 = (tuple(map(int, duplas[0])))
p2 = (tuple(map(int, duplas[1])))

sistema = os.name

if sistema == 'posix':
    os.system("clear")
elif sistema == 'nt':
    os.system("cls")

def preparar_lab(linhas):
    labirinto = []
    for linha in linhas:
        nova_linha = []
        for posicao in linha:
            if posicao == '0':
                nova_linha.append(' ')
            else:
                nova_linha.append('█')
        labirinto.append(nova_linha)
    return labirinto

labirinto = preparar_lab(linhas)

def percorrer_lab(ponto1, ponto2):
    global chamadas_heuristica
    chamadas_heuristica += 1


    y1, x1 = map(int, ponto1)
    y2, x2 = map(int, ponto2)

    if labirinto[x2][y2] == '█':
        print("Ponto de destino é inválido")
        return 0
    
    if x1 < 0 or x1 >= len(labirinto) or y1 < 0 or y1 >= len(labirinto[0]) - 2:
        return 0

    if labirinto[x1][y1] == '█' or labirinto[x1][y1] == 'x':
        return 0

    if (x1, y1) == (x2, y2):
        labirinto[x1][y1] = 'F'
        return 1

    labirinto[x1][y1] = 'x'

    movimentos_possiveis = [(x1 - 1, y1), (x1 + 1, y1),(x1, y1 - 1), (x1, y1 + 1)]

    def heuristica(ponto):
        x, y = ponto
        return abs(x - x2) + abs(y - y2)

    movimentos_possiveis.sort(key=heuristica)

    for prox in movimentos_possiveis:
        if percorrer_lab((prox[1], prox[0]), (y2, x2)):
            return 1

    labirinto[x1][y1] = ' '
    return 0

def backtracking_tradicional(ponto1, ponto2, labirinto_bt):
    global chamadas_backtracking
    chamadas_backtracking += 1

    y1, x1 = map(int, ponto1)
    y2, x2 = map(int, ponto2)

    if labirinto[x2][y2] == '█':
        print("Ponto de destino é inválido")
        return 0
    
    if x1 < 0 or x1 >= len(labirinto_bt) or y1 < 0 or y1 >= len(labirinto_bt[0]) - 2:
        return 0

    if labirinto_bt[x1][y1] == '█' or labirinto_bt[x1][y1] == 'x':
        return 0

    if (x1, y1) == (x2, y2):
        labirinto_bt[x1][y1] = 'F'
        return 1

    labirinto_bt[x1][y1] = 'x'

    movimentos_possiveis = [(x1 - 1, y1), (x1 + 1, y1),(x1, y1 - 1), (x1, y1 + 1)]

    for prox in movimentos_possiveis:
        if backtracking_tradicional((prox[1], prox[0]), (y2, x2), labirinto_bt):
            return 1

    labirinto_bt[x1][y1] = ' '
    return 0

if percorrer_lab(p1, p2):
    print("Caminho encontrado pelo método heurístico:")
    for linha in labirinto:
        print(''.join(linha))

labirinto = preparar_lab(linhas)
if backtracking_tradicional(p1, p2, labirinto):
    print("\nCaminho encontrado pelo backtracking tradicional:")
    for linha in labirinto:
        print(''.join(linha))
else:
    print('Caminho não encontrado')
    exit()

print(f"\nChamadas pelo método heurístico: {chamadas_heuristica}\n Chamadas pelo método tradicional: {chamadas_backtracking}")

# (1, 1) (48, 22)
# (1, 1) (20, 22)
# (1, 1) (5, 18)
# (48, 5) (15, 2)