from PIL import Image
import winreg
import tkinter as tk

class basedesk():
    def __init__(self,master):
        self.root = master
        self.root.config()
        self.root.title('zhubi')
        self.root.geometry('700x600+300+50')
        face_sym(self.root)

class face_sym():
    def __init__(self,master):
        self.master = master
        self.master.config(bg='white')
        self.pic = Image.new('RGB', (1, 1), color='white')
        self.face_sym = tk.Frame(self.master)
        self.face_sym.pack()

        self.index = tk.Label(self.face_sym, text = "睿智图片生成器", bg = 'white', font=('Arial', 20), width=77,height=2)
        self.index.pack()

        self.label_picpath = tk.Label(self.face_sym, text = "输入图片地址: ", bg = 'white', font = ('Arial', 12), width=77, height=2)
        self.label_picpath.pack()
        self.entry_path = tk.Entry(self.face_sym, show = '',width= 50)
        self.entry_path.pack()

        self.label_perLeft = tk.Label(self.face_sym, text = "输入裁剪比例(Left): ", bg = 'white', font = ('Arial', 12), width=77, height=2)
        self.label_perLeft.pack()
        self.entry_perLeft = tk.Entry(self.face_sym, show='', width= 10)
        self.entry_perLeft.pack()

        self.label_perRight = tk.Label(self.face_sym, text = "输入裁剪比例(right): ", bg = 'white', font = ('Arial', 12), width=77, height=2)
        self.label_perRight.pack()
        self.entry_perRight = tk.Entry(self.face_sym, show='', width= 10)
        self.entry_perRight.pack()

        self.label_savename = tk.Label(self.face_sym, text = "输入保存名字: ", bg = 'white', font = ('Arial', 12), width=77, height=2)
        self.label_savename.pack()
        self.entry_savename = tk.Entry(self.face_sym, show='', width= 10)
        self.entry_savename.pack()

        b1 = tk.Button(self.face_sym,text="finish",width=15,height=2,command=self.get_pic)
        b1.pack()

        b2 = tk.Button(self.face_sym,text="save",width=15,height=2,command=self.get_save)
        b2.pack()

        btn = tk.Button(self.face_sym,text='change function',command=self.change)
        btn.pack(side='bottom')

    def get_desktop(self):
        key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, r'Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders')
        return winreg.QueryValueEx(key, "Desktop")[0]

    def re_path(self, path):
        re_path = '\\'.join(path.split('\\'))
        re_path = ''.join(path.split('"'))
        return re_path

    def get_pic(self):
        pic_path = self.entry_path.get()
        per_getleft = self.entry_perLeft.get()
        per_getright = self.entry_perRight.get()

        im = Image.open(self.re_path(pic_path))
        per_left = int(per_getleft) / 100
        per_right = int(per_getright) / 100

        width = im.width
        height = im.height
        left = int(width * per_left)
        right = int(width * per_right)

        im_blank = Image.new('RGB', ((right - left) * 2, height), 'white')
        im_left = im.copy()
        im_left = im.crop((left, 0, right, height))
        im_right = im_left.copy()
        im_left = im_left.transpose(Image.FLIP_LEFT_RIGHT)

        im_blank.paste(im_right, (0,0))
        im_blank.paste(im_left, (right - left, 0))
        self.pic = im_blank
        im_blank.show()

    def get_save(self):
        save_name = self.entry_savename.get()
        save_path = self.get_desktop()
        save_path = self.re_path(save_path)
        self.pic.save("{}\\{}.jpg".format(save_path, save_name))

    def change(self):
        self.face_sym.destroy()
        face_mix(self.master)


class face_mix():

    def __init__(self, master):
        self.master = master
        self.master.config(bg='white')
        self.pic = Image.new('RGB', (1, 1), color='white')
        self.face_mix = tk.Frame(self.master)
        self.face_mix.pack()

        self.index = tk.Label(self.face_mix,
                              text="图片合成器",
                              bg='white',
                              font=('Arial', 20),
                              width=77,
                              height=2)
        self.index.pack()

        self.label_picpath_1 = tk.Label(self.face_mix,
                                        text="输入图片 1 地址: ",
                                        bg='white',
                                        font=('Arial', 12),
                                        width=77,
                                        height=2)
        self.label_picpath_1.pack()
        self.entry_path_1 = tk.Entry(self.face_mix, show='', width=50)
        self.entry_path_1.pack()

        self.label_picpath_2 = tk.Label(self.face_mix,
                                        text="输入图片 2 地址: ",
                                        bg='white',
                                        font=('Arial', 12),
                                        width=77,
                                        height=2)
        self.label_picpath_2.pack()
        self.entry_path_2 = tk.Entry(self.face_mix, show='', width=50)
        self.entry_path_2.pack()

        self.option = tk.Label(self.face_mix,
                               text="输入合成方法(1, 2): ",
                               bg='white',
                               font=('Arial', 12),
                               width=77,
                               height=2)
        self.option.pack()
        self.entry_option = tk.Entry(self.face_mix, show='', width=10)
        self.entry_option.pack()

        self.label_savename = tk.Label(self.face_mix,
                                       text="输入保存名字: ",
                                       bg='white',
                                       font=('Arial', 12),
                                       width=77,
                                       height=2)
        self.label_savename.pack()
        self.entry_savename = tk.Entry(self.face_mix, show='', width=10)
        self.entry_savename.pack()

        b1 = tk.Button(self.face_mix,
                       text="finish",
                       width=15,
                       height=2,
                       command=self.get_mix)
        b1.pack()

        b2 = tk.Button(self.face_mix,
                       text="save",
                       width=15,
                       height=2,
                       command=self.get_save)
        b2.pack()

        btn = tk.Button(self.face_mix,
                        text='change function',
                        command=self.change)
        btn.pack(side='bottom')

    def get_desktop(self):
        key = winreg.OpenKey(
            winreg.HKEY_CURRENT_USER,
            r'Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders'
        )
        return winreg.QueryValueEx(key, "Desktop")[0]

    def re_path(self, path):
        re_path = '\\'.join(path.split('\\'))
        re_path = ''.join(path.split('"'))
        return re_path

    def get_mix(self):
        picpath_1 = self.entry_path_1.get()
        picpath_2 = self.entry_path_2.get()
        option = self.entry_option.get()

        im_1 = Image.open(self.re_path(picpath_1)).convert("RGBA")
        im_2 = Image.open(self.re_path(picpath_2)).convert("RGBA")

        if option == '1':
            # x = im_1.width if im_1.width < im_2.width else im_2.width
            # y = im_1.height if im_1.height < im_2.height else im_2.height
            # im_1.resize((x, y))
            # im_2.resize((x, y))
            # self.pic = Image.blend(im_1, im_2, 0.5)
            im_1.resize(im_2.size)
            r, g, b, alpha = im_1.split()
            alpha = alpha.point(lambda i: i > 0 and 204)
            self.pic = Image.composite(im_1, im_2, alpha)
            self.pic.show()

        elif option == '2':
            # x = im_1.width if im_1.width < im_2.width else im_2.width
            # y = im_1.height if im_1.height < im_2.height else im_2.height
            # im_1.resize((x, y))
            # im_2.resize((x, y))
            # self.pic = Image.blend(im_2, im_1, 0.5)
            im_2.resize(im_1.size)
            r, g, b, alpha = im_2.split()
            alpha = alpha.point(lambda i: i > 0 and 204)
            self.pic = Image.composite(im_2, im_1, alpha)
            self.pic.show()

    def get_save(self):
        save_name = self.entry_savename.get()
        save_path = self.get_desktop()
        save_path = self.re_path(save_path)
        self.pic.save("{}\\{}.jpg".format(save_path, save_name))

    def change(self):
        self.face_mix.destroy()
        face_sym(self.master)

class make_gif():
    pass

if __name__ == '__main__':
    window = tk.Tk()
    tips = tk.Label(window, text = "welcome 2 铸币编辑器!", bg = 'white',font = ('Arial', 12), width=77, height=1)
    tips.pack()

    basedesk(window)

    tipp = tk.Label(window, text = "由醉花阴制作", bg = 'white',font = ('Arial', 12), width=77, height=2)
    tipp.pack(side='bottom', expand=True)

    window.mainloop()
