from hmac import new
from PIL import Image
import time


def re_path(path):
    re_path = '\\'.join(path.split('\\'))
    re_path = ''.join(path.split('"'))
    return re_path


start = time.time()

path_1 = input("input path 1: ")
path_2 = input("input path 2: ")
img1 = Image.open(re_path(path_1)).convert("RGBA")
img2 = Image.open(re_path(path_2)).convert("RGBA")
w1, h1 = img1.size
w2, h2 = img2.size
w = min(w1, h1)
h = min(h1, h2)

new_img = Image.new("RGBA", (w, h))

array_1 = img1.load()
array_2 = img2.load()

array_1_w = int(w1 / w)
array_1_h = int(h1 / h)
array_2_w = int(w2 / w)
array_2_h = int(h2 / h)

scale_1 = min(array_1_w, array_1_h)
scale_2 = min(array_2_w, array_2_h)

trans_x_1 = int((w1 - w * scale_1) / 2)
trans_x_2 = int((w2 - w * scale_2) / 2)

for i in range(0, w):
    for j in range(0, h):
        r1, g1, b1, a1 = array_1[trans_x_1 + i * scale_1, j * scale_1]
        r2, g2, b2, a2 = array_2[trans_x_2 + i * scale_2, j * scale_2]
        r1 = r1 if r1 > 102 else 102
        g1 = g1 if g1 > 102 else 102
        b1 = b1 if b1 > 102 else 102
        r2 = r2 * 0.4
        g2 = g2 * 0.4
        b2 = b2 * 0.4
        r_new = int(r2 / (1 - (r1 - r2) / 255))
        g_new = int(g2 / (1 - (g1 - g2) / 255))
        b_new = int(b2 / (1 - (b1 - b2) / 255))
        a_new = (3 - ((r1 - r2) / 255) - ((g1 - g2) / 255) -
                 ((b1 - b2) / 255)) / 3
        new_img.putpixel((i, j), (r_new, g_new, b_new))

new_img.save('phantom.png')
end = time.time()
print("cost %d s" % (end - start))
