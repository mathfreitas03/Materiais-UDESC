from PIL import Image

img = Image.open("img/girassol.jpg")
imgOutput = Image.new("RGB", img.size, (255, 255, 255))

def lightness_method_rgb_convert(img):
    x, y = img.size
    for i in range(x):
        for j in range(y):
            pixel = img.getpixel((i, j))
            grayscale = int((min(pixel[0], pixel[1], pixel[2]) + max(pixel[0], pixel[1], pixel[2]))/2)
            imgOutput.putpixel((i, j), (grayscale, grayscale, grayscale))
    imgOutput.save("imgOutputLightness.jpg")

def average_method_rgb_convert(img):
    x, y = img.size
    for i in range(x):
        for j in range(y):
            rgb = img.getpixel((i, j))
            grayscale = int(sum(rgb)/3)
            imgOutput.putpixel((i, j), (grayscale, grayscale, grayscale))
    imgOutput.save("imgOutputAverage.jpg")

def luminosity_method_rgb_convert(img):
    pixels = img.load()
    width, height = img.size

    for x in range(width):
        for y in range(height):
            rgb = img.getpixel((x, y))
            grayscale = int(0.3 * rgb[0] + 0.59 * rgb[1] + 0.11 * rgb[2])
            imgOutput.putpixel((x, y), (grayscale, grayscale, grayscale))
    imgOutput.save("imgOutputLuminosity.jpg")

lightness_method_rgb_convert(img)
average_method_rgb_convert(img)
luminosity_method_rgb_convert(img)
