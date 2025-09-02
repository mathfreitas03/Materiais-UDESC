from os import system
import random
import time

class Item:
    def __init__(self):
        pass

class Formiga:
    def __init__(self, x, y, vision_field):
        self.x = x
        self.y = y
        self.is_alive = True
        self.holding_item = False
        self.vision_field = vision_field

    def move_ant(self, formigueiro):
        op = [-1, 0, 1]

        # limpa posição antiga (se não houver item ali)

        if (self.x, self.y) in formigueiro.itens:
            formigueiro.grid[self.x][self.y] = "I"
        else:
            formigueiro.grid[self.x][self.y] = "."

        while True:
            new_x = self.x + random.choice(op)
            new_y = self.y + random.choice(op)

            if new_x < 0:
                new_x = formigueiro.grid_size - 1
            elif new_x >= formigueiro.grid_size:
                new_x = 0

            if new_y < 0:
                new_y = formigueiro.grid_size - 1
            elif new_y >= formigueiro.grid_size:
                new_y = 0

            # só aceita a nova posição se não tiver outra formiga
            if formigueiro.grid[new_x][new_y] != 'F':
                break

        self.x = new_x
        self.y = new_y

        formigueiro.grid[self.x][self.y] = "F"

class Formigueiro:
    def __init__(self, grid_size, n_formigas, n_itens):
        self.grid_size = grid_size

        # inicializa grid vazio
        self.grid = [["." for _ in range(grid_size)] for i in range(grid_size)]

        positions = [(i, j) for i in range(grid_size) for j in range(grid_size)]
        random.shuffle(positions)

        self.formigas = []
        for _ in range(n_formigas):
            x, y = positions.pop()
            formiga = Formiga(x, y, vision_field=1)
            self.formigas.append(formiga)
            self.grid[x][y] = "F" 

        self.itens = []
        for _ in range(n_itens):
            x, y = positions.pop()
            self.itens.append((x, y))
            self.grid[x][y] = "I" 

    def mostrar_grid(self):
        for linha in self.grid:
            print(" ".join(linha))
        print()

def simulate():
    formig = Formigueiro(grid_size=20, n_formigas=10, n_itens=5)
    for _ in range (10):
        system("cls")
        for formiga in formig.formigas:
            formiga.move_ant(formig)
        formig.mostrar_grid()
        time.sleep(.5)

simulate()