from PIL import Image

imgL = Image.open("img/brasilgray.png", "r").convert("L")

def pseudocolor(img):
    imgRGB = Image.new("RGB", imgL.size, (255, 255, 255))

    x, y = img.size

    for i in range (x):
        for j in range (y):
            grayScalePixel = img.getpixel((i, j))
            if grayScalePixel < 100:
                imgRGB.putpixel((i, j), (180, 0, 0))
            elif grayScalePixel < 120:
                imgRGB.putpixel((i, j), (200, 32, 32))
            elif grayScalePixel < 160:
                imgRGB.putpixel((i, j), (220, 64, 64))
            elif grayScalePixel < 180:
                imgRGB.putpixel((i, j), (255, 128, 128))
            else:
                imgRGB.putpixel((i, j), (grayScalePixel, grayScalePixel, grayScalePixel))


    imgRGB.save("imgOutputPseudocor.jpg")

pseudocolor(imgL)