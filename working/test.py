from PIL import Image
import time


def colorful_shio(brightness_f, brightness_b):
    '''给定两个参数调整里外图亮度以求得最佳幻坦效果'''
    start = time.time()
    image_f = Image.open("C:\\Users\\15879\\Desktop\\QQ图片20240511002927.jpg")
    image_b = Image.open("C:\\Users\\15879\\Desktop\\QQ图片20240511002931.jpg")
    #导出宽高信息
    w_f, h_f = image_f.size
    w_b, h_b = image_b.size
    #注意：jep图片的像素信息储存格式为RGB，缺少透明度的设置
    #所以需要新建一个RGBA格式的照片文件
    w_min = min(w_f, w_b)
    h_min = min(h_f, h_b)
    new_image = Image.new('RGBA', (w_min, h_min))  #此处使用的是两者较大一方的参数
    #load()将图片的像素信息储存成array，提供位置坐标即可调出
    # 其速度优于open()
    array_f = image_f.load()
    array_b = image_b.load()
    #调整为同比例图片（计算宽高比例）
    scale_h_f = int(h_f / h_min)
    scale_w_f = int(w_f / w_min)
    scale_h_b = int(h_b / h_min)
    scale_w_b = int(w_b / w_min)
    #确定较小的比例为参照比例
    scale_f = min(scale_h_f, scale_w_f)
    scale_b = min(scale_h_b, scale_w_b)
    #使选中像素点居于原图片中央
    trans_f_x = int((w_f - w_min * scale_f) / 2)
    trans_b_x = int((w_b - w_min * scale_b) / 2)
    #设置修正参数
    #待选值有：10-8，11-7，11-8
    #均为实验所得 格式为a-b
    a = brightness_f
    b = brightness_b
    for i in range(0, w_min):
        for j in range(0, h_min):
            #注意：像素点位置是修正过的
            R_f, G_f, B_f = array_f[trans_f_x + i * scale_f, j * scale_f]
            R_b, G_b, B_b = array_b[trans_b_x + i * scale_b, j * scale_b]
            #对亮度信息进行修正
            R_f *= a / 10
            R_b *= b / 10
            G_f *= a / 10
            G_b *= b / 10
            B_f *= a / 10
            B_b *= b / 10
            #注意：下面的系数变量及结果通过LAB颜色空间求颜色近似度得到
            delta_r = R_b - R_f
            delta_g = G_b - G_f
            delta_b = B_b - B_f
            coe_a = 8 + 255 / 256 + (delta_r - delta_b) / 256
            coe_b = 4 * delta_r + 8 * delta_g + 6 * delta_b + (
                (delta_r - delta_b) *
                (R_b + R_f)) / 256 + (delta_r**2 - delta_b**2) / 512
            A_new = 255 + coe_b / (2 * coe_a)
            A_new = int(A_new)
            #A_new可能存在不属于0-255的情况，需要进行修正
            if A_new <= 0:
                A_new = 0
                R_new = 0
                G_new = 0
                B_new = 0
            elif A_new >= 255:
                A_new = 255
                R_new = int((255 * (R_b) * b / 10) / A_new)
                G_new = int((255 * (G_b) * b / 10) / A_new)
                B_new = int((255 * (B_b) * b / 10) / A_new)
            else:
                A_new = A_new
                R_new = int((255 * (R_b) * b / 10) / A_new)
                G_new = int((255 * (G_b) * b / 10) / A_new)
                B_new = int((255 * (B_b) * b / 10) / A_new)
            pixel_new = (R_new, G_new, B_new, A_new)
            #注：刚发现调试是可以看到临时数据的，需要设置断点
            # print(pixel_new)
            #导入像素信息
            new_image.putpixel((i, j), pixel_new)
    #保存新图片

    new_image.save('phantom.png')
    #计算运行程序总时间
    end = time.time()
    print('running time:%ds' % (end - start))


if __name__ == '__main__':
    colorful_shio(12, 7)
