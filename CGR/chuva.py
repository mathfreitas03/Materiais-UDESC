import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
import random

WIDTH, HEIGHT = 800, 600
NUM_GOTAS = 500  # quantidade de chuva

class Gota:
    def __init__(self):
        self.reset()
        self.z = random.uniform(-5, 5)
        
    def reset(self):
        self.x = random.uniform(-10, 10)
        self.y = random.uniform(5, 15)
        self.speed = random.uniform(0.5, 1.5)
        self.length = random.uniform(0.2, 0.5)
        
    def update(self):
        self.y -= self.speed
        if self.y < -5:
            self.reset()

def inicializar():
    pygame.init()
    pygame.display.set_mode((WIDTH, HEIGHT), DOUBLEBUF | OPENGL)
    gluPerspective(45, (WIDTH/HEIGHT), 0.1, 50.0)
    glTranslatef(0.0, 0.0, -20)

def desenhar_chuva(gotas):
    glBegin(GL_LINES)
    for gota in gotas:
        glColor3f(0.5, 0.5, 1.0)
        glVertex3f(gota.x, gota.y, gota.z)
        glVertex3f(gota.x, gota.y - gota.length, gota.z)
    glEnd()

def main():
    inicializar()
    pygame.display.set_caption('Chuva')
    gotas = [Gota() for _ in range(NUM_GOTAS)]
    
    clock = pygame.time.Clock()
    
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return
        
        for gota in gotas:
            gota.update()
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        
        desenhar_chuva(gotas)
        
        pygame.display.flip()
        clock.tick(60)

if __name__ == "__main__":
    main()