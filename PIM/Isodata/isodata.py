from PIL import Image
from pathlib import Path
from grayscale import luminosity_method_rgb_convert
import sys
import os
import random

try:
    path = Path((os.getcwd() + "/img/" + sys.argv[1]))
    img = Image.open(path.resolve())

except IndexError:
        print('Erro: É necessário passar uma imagem como parâmetro.')
        sys.exit(1)

import random

def isodata(img):
    imgL = luminosity_method_rgb_convert(img)
    hist = imgL.histogram()

    limiar1 = 0
    limiar2 = random.randint(0, 255)

    while abs(limiar1 - limiar2) > 0.01:
        limiar1 = limiar2

        mi1 = 0.0
        mi2 = 0.0
        w1 = 0.0
        w2 = 0.0

        for i in range(limiar1 + 1):
            mi1 += i * hist[i]
            w1 += hist[i]
        
        for i in range(limiar1 + 1, 256):
            mi2 += i * hist[i]
            w2 += hist[i]

        if w1 == 0 or w2 == 0:
            break

        mi1 = mi1 / w1
        mi2 = mi2 / w2

        limiar2 = int((mi1 + mi2) / 2)

    return limiar2

t = isodata(img)

def thresholding(img, t):
    img = img.convert("L")
    imgOutput = Image.new("L", img.size, 0)
    w, h = img.size

    for x in range(w):
          for y in range(h):
                if img.getpixel((x, y)) < t:
                    imgOutput.putpixel((x, y), 255)
                else:
                     imgOutput.putpixel((x, y), 0)

    imgOutput.save("imgOutput\\isodataOutput.jpg")

thresholding(img, t)