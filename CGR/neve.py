import pygame
import random
import sys

pygame.init()

LARGURA, ALTURA = 800, 600
tela = pygame.display.set_mode((LARGURA, ALTURA))
pygame.display.set_caption("Neve")


COR_FUNDO = (0, 0, 30)

class FlocoNeve:
    def __init__(self):
        self.reset()
        self.tamanho = random.randrange(1, 4)
        
    def reset(self):
        self.x = random.randrange(0, LARGURA)
        self.y = random.randrange(-50, -10)
        self.velocidade = random.uniform(1.0, 3.0)  # Velocidade mais lenta e variada
        
    def atualizar(self):
        self.y += self.velocidade
        
        # variação horizontal aleatória
        self.x += random.uniform(-0.3, 0.3)
        
        if self.y > ALTURA + 10 or self.x < -10 or self.x > LARGURA + 10:
            self.reset()
    
    def desenhar(self, tela):
        pygame.draw.circle(tela, (255, 255, 255), (int(self.x), int(self.y)), self.tamanho)

flocos = [FlocoNeve() for _ in range(400)]

relogio = pygame.time.Clock()

while True:
    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
    
    tela.fill(COR_FUNDO)
    
    for floco in flocos:
        floco.atualizar()
        floco.desenhar(tela)
    
    pygame.display.update()
    
    # velocidade do loop
    relogio.tick(60)