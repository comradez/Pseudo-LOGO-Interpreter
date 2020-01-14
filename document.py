from tkinter import *
import os
import tkinter.messagebox
from tkinter import filedialog as fd
import cv2 as cv
import numpy as np
import platform
file_name=''
dir_path=''
def is_error():
    with open(dir_path+"/error.log") as fi:
        for line in fi:
            a=line.split()
            if 'halt' in a:
                return False
    return True
def save_item(text):
    global file_name
    if file_name=='':
        file_name=fd.asksaveasfilename()
    if file_name=='':
        return
    FI=open(file_name,"w")
    fi=text.get(0.0,END)
    FI.write(fi)
def osave_item(text):
    file_name=fd.asksaveasfilename()
    FI=open(file_name,"w")
    fi=text.get(0.0,END)
    FI.write(fi)
def open_item(text):
    global file_name
    fn=fd.askopenfilename()
    if fn=='':
        return
    file_name=fn
    fi=open(fn,'r')
    text.delete(0.0,END)
    for line in fi:
        text.insert(END,line)
    fi.close()

def compiler(text):
    global dir_path, file_name
    if file_name == '':
        save_item(text)
    file_name_ = ""
    if dir_path == '':
        pos = 0
        for i in range(len(file_name)):
            if file_name[i] == '/':
                pos = i
        dir_path = file_name[0:pos]
        pos = 0
        for i in range(len(file_name)):
            if file_name[i] == '.':
                pos = i
        file_name_ = file_name[0:pos]
    if dir_path == '':
        return
    if not os.path.exists(dir_path + "/pics"):
        print(file_name + '**\n')
        print(dir_path + "/pics")
        os.mkdir(dir_path + "/pics")
    if platform.system() == "Windows":
        os.system("interpreter \"" + file_name + '"')
    else:
        os.system("./interpreter \"" + file_name + '"')
    i = p = j = 0
    if is_error():
        LINE = ''
        with open(dir_path + "/error.log") as fi:
            LINE += (next(fi) + '\n')
        result = tkinter.messagebox.showerror(title = 'Error Message', message = LINE)
        return
    with open(file_name, 'r') as fi:
        line = next(fi)
        line = next(fi)
        line = next(fi)
        line = next(fi)
        l = line.split(' ')
        if (l[1] != '0\n'):
            fps = 24
            img_tmp = cv.imread(dir_path + "/pics/output_0.jpg", 1)
            (p_height, p_width, p_temp) = img_tmp.shape
            size = (p_width, p_height)
            print(p_width, p_height)
            fourcc = cv.VideoWriter_fourcc(*'XVID')
            video = cv.VideoWriter(file_name_+".avi", fourcc, fps, size)
            picnum = len(os.listdir(dir_path + "/pics"))
            for foo in range(0, picnum):
                img_temp = cv.imread(dir_path + "/pics/output_" + str(foo) + ".jpg")
                video.write(img_temp)
                
##    if os.path.exists(dir_path + "/pics"):
##        file_list = os.listdir(dir_path + "/pics")
##        for each_file in file_list:
##            os.remove(dir_path + "/pics/" + each_file)
##        os.removedirs(dir_path + "/pics")    
     
    with open(dir_path + "/rgb_mat.out", 'r') as fi:
        ax = (0, 1, 2)
        line = next(fi)
        a = line.split()
        X = int(a[0])
        Y = int(a[1])
        img = np.zeros([X, Y, 3])
        for line in fi:
            a = line.split()
            for x in a:
                img[i][p][ax[j]] = int(x)
                j += 1
                if j == 3:
                    j = 0
                    p += 1
                if p == Y:
                    p = 0
                    i += 1
        img1 = cv.transpose(img)
        cv.imwrite(file_name_ + ".bmp", img1)
        print(file_name_ + ".bmp")
        img2 = cv.imread(file_name_ + ".bmp", 1)
        ##if(os.path.exists(dir_path + "rgb_mat.out")):
        ##    os.remove(dir_path + "rgb_mat.out")
        cv.imshow('output_image', img2)
        cv.moveWindow('output_image', 100, 100)
        cv.waitKey(30)
        if cv.waitKey(0) == 27:
            cv.destroyAllWindows()
    os.remove(dir_path + "/rgb_mat.out")
    return False
