import random
import math
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# ========================
# Funções auxiliares
# ========================
def euclidean_distance(v1, v2):
    return math.sqrt(sum((a - b) ** 2 for a, b in zip(v1, v2)))

# ========================
# Classes principais
# ========================
class Item:
    def __init__(self, features):
        self.features = features  # vetor de dados do item

class Formiga:
    def __init__(self, x, y, vision_field, k1=0.1, k2=0.3, alpha=1.0):
        self.x = x
        self.y = y
        self.vision_field = vision_field
        self.holding_item = None
        self.k1 = k1
        self.k2 = k2
        self.alpha = alpha

    def move(self, formigueiro):
        dx, dy = random.choice([(0,1), (0,-1), (1,0), (-1,0)])
        new_x = (self.x + dx) % formigueiro.width
        new_y = (self.y + dy) % formigueiro.height

        # Atualizar posição
        if formigueiro.grid[new_y][new_x] != "A":  # não andar em cima de outra formiga
            formigueiro.grid[self.y][self.x] = None if not self.holding_item else self.holding_item
            self.x, self.y = new_x, new_y
            formigueiro.grid[self.y][self.x] = "A"

    def local_density(self, formigueiro, item):
        neighbors = []
        for dx in range(-self.vision_field, self.vision_field + 1):
            for dy in range(-self.vision_field, self.vision_field + 1):
                nx, ny = (self.x + dx) % formigueiro.width, (self.y + dy) % formigueiro.height
                cell = formigueiro.grid[ny][nx]
                if isinstance(cell, Item):
                    neighbors.append(cell)

        if not neighbors:
            return 0

        sim_sum = 0
        for n in neighbors:
            d = euclidean_distance(item.features, n.features)
            sim_sum += max(0, 1 - (d / self.alpha))
        return sim_sum / len(neighbors)

    def try_grab_or_drop(self, formigueiro):
        cell = formigueiro.grid[self.y][self.x]

        # 🔹 Pegar item (só se não estiver carregando nada)
        if self.holding_item is None and isinstance(cell, Item):
            f = self.local_density(formigueiro, cell)
            p_pick = (self.k1 / (self.k1 + f)) ** 2
            if random.random() < p_pick:
                # guarda item na formiga
                self.holding_item = cell
                # remove item do grid
                formigueiro.grid[self.y][self.x] = "A"

        # 🔹 Largar item (só se estiver carregando e célula estiver vazia)
        elif self.holding_item is not None and cell is None:
            f = self.local_density(formigueiro, self.holding_item)
            p_drop = (f / (self.k2 + f)) ** 2

            if f == 0:  # chance mínima de drop
                p_drop = 0.2  

            if random.random() < p_drop:
                formigueiro.grid[self.y][self.x] = self.holding_item
                self.holding_item = None


class Formigueiro:
    def __init__(self, width, height, items, num_ants):
        self.width = width
        self.height = height
        self.grid = [[None for _ in range(width)] for _ in range(height)]
        self.ants = []

        for item in items:
            x, y = self.random_empty_cell()
            self.grid[y][x] = item

        for f in range(num_ants):
            x, y = self.random_empty_cell()
            ant = Formiga(x, y, vision_field=1, k1=0.3, k2=0.6, alpha=11.8029)
            self.grid[y][x] = "A"
            self.ants.append(ant)

    def random_empty_cell(self):
        while True:
            x = random.randint(0, self.width - 1)
            y = random.randint(0, self.height - 1)
            if self.grid[y][x] is None:
                return x, y

    def step(self):
        for ant in self.ants:
            ant.move(self)
            ant.try_grab_or_drop(self)

# ========================
# Simulação com visualização
# ========================
def simular_visual(f, passos=10000, interval=50):
    fig, ax = plt.subplots(figsize=(6,6))

    def update(frame):
        ax.clear()
        f.step()

        # Plota itens
        xs, ys, colors = [], [], []
        for y in range(f.height):
            for x in range(f.width):
                cell = f.grid[y][x]
                if isinstance(cell, Item):
                    xs.append(x)
                    ys.append(y)
                    # usar primeira feature como cor
                    colors.append((cell.features[0], cell.features[1], 0.5))

        ax.scatter(xs, ys, c=colors, marker="o", s=40, alpha=0.7)

        # Plota formigas
        ax.scatter([ant.x for ant in f.ants],
                   [ant.y for ant in f.ants],
                   c="black", marker="x", s=50)

        ax.set_xlim(0, f.width)
        ax.set_ylim(0, f.height)
        ax.set_title(f"Iteração {frame}")

    ani = animation.FuncAnimation(fig, update, frames=passos, interval=interval, repeat=False)
    plt.show()

# ========================
# Exemplo de uso
# ========================
if __name__ == "__main__":
    # Criar dataset simples (100 itens com 2 atributos normalizados entre 0 e 1)
    items = [Item([random.random(), random.random()]) for _ in range(100)]

    # Criar ambiente
    f = Formigueiro(width=30, height=30, items=items, num_ants=10)

    # Rodar simulação com visualização
    simular_visual(f, passos=500, interval=100)
