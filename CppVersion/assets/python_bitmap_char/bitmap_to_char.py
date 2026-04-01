from PIL import Image

alpha = Image.open("/Users/griffinroberts/Movies/CProjects/EcosystemSim/CppVersion/assets/python_bitmap_char/monogram-bitmap.png").convert("RGBA").split()[-1]
alpha_pixels = alpha.load()
binary_array = [
    [1 if alpha_pixels[x, y] == 255 else 0 for x in range(alpha.width)]
    for y in range(alpha.height)]

for row in binary_array:
    print("\t{" + ",".join(map(str, row)) + "},")