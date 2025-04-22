from time import sleep
from PIL import Image

imgSrc = Image.open("charmander.png").convert("L")
imgDst = Image.open("charmeleon.png").convert("L")
imgDst2 = Image.open("charizard.png").convert("L")

frames = []

def crossfading(imgSource, imgDestiny):
    u = .0
    width, height = imgSource.size

    for i in range(12):
        if u > 1:
            break
        imgTransition = Image.new("L", imgSource.size, 255)
        
        for x in range(width):
            for y in range(height):
                pixel1 = imgSource.getpixel((x, y))
                pixel2 = imgDestiny.getpixel((x, y))
                blended = int((1 - u) * pixel1 + u * pixel2)
                imgTransition.putpixel((x, y), blended)
        frames.append(imgTransition)
        u += 0.1

crossfading(imgSrc, imgDst)
crossfading(imgDst, imgDst2)

frames[0].save("fade.gif", save_all=True, append_images=frames[1:], duration=200, loop=0)
