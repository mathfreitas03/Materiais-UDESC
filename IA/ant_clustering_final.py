import numpy as np
import random
import pygame
import os
import sys
import matplotlib.pyplot as plt
from datetime import datetime

class AntClusteringSimulation:
    def __init__(self, grid_size=64, num_live_ants=100, vision_radius=1, group_sizes=[100, 100, 100, 100], k1=0.3, k2=0.6, alpha=11.8029, move_radius=1, steps = 0):
        
        self.grid_size = grid_size
        self.num_live_ants = num_live_ants
        self.vision_radius = vision_radius
        self.group_sizes = group_sizes
        self.total_data_points = sum(group_sizes)
        self.steps = steps

        # parâmetros da dinâmica
        self.k1 = k1
        self.k2 = k2
        self.alpha = alpha
        self.move_radius = move_radius
        
        # Inicializar o grid (None = vazio, dicionário = item com dados)
        self.grid = np.full((grid_size, grid_size), None, dtype=object)
        
        self.data_points = self.generate_heterogeneous_data()
        self.distribute_data_points()
        
        # Inicializar formigas vivas
        self.live_ants = []
        for i in range(num_live_ants):
            x, y = random.randint(0, grid_size-1), random.randint(0, grid_size-1)
            self.live_ants.append({
                'id': i,
                'pos': (x, y),
                'state': 0,  # 0 = vazia, 1 = carregando item
                'carrying_item': None  
            })
    
        self.colors = {
            1: (128, 0, 128),
            2: (255, 165, 0),
            3: (255, 0, 0),
            4: (255, 255, 0),
            5: (0, 128, 0),
            6: (0, 0, 255),
            7: (255, 192, 203),
            8: (0, 255, 255),
            9: (165, 42, 42),
            10: (255, 20, 147),
            11: (173, 216, 230),
            12: (0, 100, 0),
            13: (255, 215, 0),
            14: (70, 130, 180),
            15: (128, 128, 128)
        }

    def generate_heterogeneous_data(self):
        data_points = []
        centers = [
            (-20, -20), (20, 20), (-20, 20), (20, -20),  # 4 grupos originais
            (-10, -10), (10, 10), (-10, 10), (10, -10),
            (0, 20), (0, -20), (20, 0), (-20, 0),
            (0, 0), (15, -15), (-15, 15)
        ]
        
        for g, size in enumerate(self.group_sizes, start=1):
            cx, cy = centers[g-1]
            for _ in range(size):
                x = random.gauss(cx, 2)
                y = random.gauss(cy, 2)
                data_points.append({'values': np.array([x, y]), 'group': g})
        return data_points
    
    def distribute_data_points(self):
        """Distribui os dados aleatoriamente no grid"""
        all_positions = [(i, j) for i in range(self.grid_size) for j in range(self.grid_size)]
        random_positions = random.sample(all_positions, self.total_data_points)
        for i, data_item in enumerate(self.data_points):
            x, y = random_positions[i]
            self.grid[x, y] = data_item
    
    def get_toroidal_position(self, x, y):
        return x % self.grid_size, y % self.grid_size
    
    def get_neighborhood_items(self, x, y):
        items = []
        for dx in range(-self.vision_radius, self.vision_radius + 1):
            for dy in range(-self.vision_radius, self.vision_radius + 1):
                if dx == 0 and dy == 0:
                    continue
                nx, ny = self.get_toroidal_position(x + dx, y + dy)
                if self.grid[nx, ny] is not None:
                    items.append(self.grid[nx, ny])
        return items
    
    def calculate_euclidean_distance(self, item1, item2):
        return np.linalg.norm(item1['values'] - item2['values'])
    
    def calculate_average_similarity(self, item, neighborhood_items):
        if not neighborhood_items:
            return 0
        similarities = []
        for neighbor in neighborhood_items:
            d = self.calculate_euclidean_distance(item, neighbor)
            similarities.append(max(0, 1 - d / self.alpha))  # normalizado
        return sum(similarities) / len(similarities)
    
    def calculate_pickup_probability(self, item, neighborhood_items):
        f = self.calculate_average_similarity(item, neighborhood_items)
        return (self.k1 / (self.k1 + f))**2
    
    def calculate_drop_probability(self, item, neighborhood_items):
        f = self.calculate_average_similarity(item, neighborhood_items)
        if f < self.k2:
            return 2 * f
        else:
            return 1.0
    
    def move_ant(self, ant):
        x, y = ant['pos']
        on_item = (self.grid[x, y] is not None)
        
        if ant['state'] == 0:  # vazia
            if on_item:
                neighborhood_items = self.get_neighborhood_items(x, y)
                p_pick = self.calculate_pickup_probability(self.grid[x, y], neighborhood_items)
                if random.random() < p_pick:
                    ant['state'] = 1
                    ant['carrying_item'] = self.grid[x, y]
                    self.grid[x, y] = None
        else:  # carregando
            if not on_item:
                neighborhood_items = self.get_neighborhood_items(x, y)
                p_drop = self.calculate_drop_probability(ant['carrying_item'], neighborhood_items)
                if random.random() < p_drop:
                    self.grid[x, y] = ant['carrying_item']
                    ant['carrying_item'] = None
                    ant['state'] = 0
        
        dx = random.randint(-self.move_radius, self.move_radius)
        dy = random.randint(-self.move_radius, self.move_radius)
        ant['pos'] = self.get_toroidal_position(x + dx, y + dy)
    
    def run_simulation(self, steps=1000, render_mode=True):
        num_ants = self.num_live_ants

        if not render_mode:
            self.save_snapshot(f"screenshots/initial_result_{len(self.group_sizes)}_groups_{num_ants}_ants_{self.alpha}_alpha_{steps}_iterations.png")

        if render_mode:
            pygame.init()
            cell_size = max(1, 600 // self.grid_size)
            screen = pygame.display.set_mode((self.grid_size * cell_size, self.grid_size * cell_size))
            pygame.display.set_caption("Ant Clustering Simulation")
        
        for step in range(steps):
            if render_mode and step % 5000 == 0:
                self.render(screen, cell_size)
                pygame.display.flip()
            
            random.shuffle(self.live_ants)
            for ant in self.live_ants:
                self.move_ant(ant)
        
        if not render_mode:
            self.save_snapshot(f"screenshots/final_result_{len(self.group_sizes)}_groups_{num_ants}_ants_{self.alpha}_alpha_{steps}_iterations.png")
        else:
            pygame.quit()
    
    def render(self, screen, cell_size):
        
        screen.fill((0, 0, 0))
        for x in range(self.grid_size):
            for y in range(self.grid_size):
                if self.grid[x, y] is not None:
                    item = self.grid[x, y]
                    pygame.draw.rect(screen, self.colors[item['group']], (y * cell_size, x * cell_size, cell_size, cell_size))

    
    def save_snapshot(self, filename="snapshot.png"):
    
        # matriz com cores RGB
        array = np.zeros((self.grid_size, self.grid_size, 3), dtype=np.uint8)

        for x in range(self.grid_size):
            for y in range(self.grid_size):
                if self.grid[x, y] is not None:
                    array[x, y] = self.colors[self.grid[x, y]['group']]

        # for ant in self.live_ants:
        #     x, y = ant['pos']
        #     if ant['state'] == 0:
        #         array[x, y] = [0, 255, 0]   # verde = carregando
        #     else:
        #         array[x, y] = [0, 0, 255]   # azul = sem itens

        plt.imshow(array)
        plt.axis("off")
        plt.savefig(filename, dpi=300, bbox_inches="tight", edgecolor="black")
        plt.close()

if __name__ == "__main__":

    parametros = [
    # Teste original
    {
        "grid_size": 64,
        "num_live_ants": 100,
        "vision_radius": 1,
        "group_sizes": [100, 100, 100, 100],
        "k1": 0.9,
        "k2": 0.05,
        "alpha": 0.11,
        "steps" : 2000000
    },
    # Teste 2
    {
        "grid_size": 64,
        "num_live_ants": 100,
        "vision_radius": 1,
        "group_sizes": [100, 100, 100, 100],
        "k1": 0.01,
        "k2": 0.015,
        "alpha": 30,
        "steps" : 2000000
    },
    # Teste 3 (15 grupos)
    {
        "grid_size": 64,
        "num_live_ants": 15,
        "vision_radius": 1,
        "group_sizes": [40] * 15, # 15 grupos com 40 itens
        "k1": 0.5,
        "k2": 0.025,
        "alpha": 0.35,
        "steps" : 50000000 
    },
    # Teste 4
    {
        "grid_size": 64,
        "num_live_ants": 15,
        "vision_radius": 1,
        "group_sizes": [40] * 15,  
        "k1": 0.9,
        "k2": 0.05,
        "alpha": 0.11,
        "steps" : 50000000 
    }
    ]

    i = int(sys.argv[1]) or 0

    sim = AntClusteringSimulation(**parametros[i])

    sim.run_simulation(sim.steps, render_mode=False)