from time import sleep
from PIL import Image

imgSrc = Image.open("charmander.png")
imgDst = Image.open("charmeleon.png")
imgDst2 = Image.open("charizard.png")

frames = []

def crossfading(imgSource, imgDestiny):
    u = .0
    width, height = imgSource.size

    for i in range(12):
        if u > 1:
            break
        imgTransition = Image.new("RGB", imgSource.size, (255, 255, 255))
        
        for x in range(width):
            for y in range(height):
                pixel1 = imgSource.getpixel((x, y))
                pixel2 = imgDestiny.getpixel((x, y))
                blendedR = int((1 - u) * pixel1[0] + u * pixel2[0])
                blendedG = int((1 - u) * pixel1[1] + u * pixel2[1])
                blendedB = int((1 - u) * pixel1[2] + u * pixel2[2])
                imgTransition.putpixel((x, y), (blendedR, blendedG, blendedB))
        frames.append(imgTransition)
        u += 0.1

crossfading(imgSrc, imgDst)
crossfading(imgDst, imgDst2)

frames[0].save("fade.gif", save_all=True, append_images=frames[1:], duration=100, loop=0)
