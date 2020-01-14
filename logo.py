from tkinter import *
from tkinter import ttk
from tkinter.scrolledtext import ScrolledText
import os
import syntax as syn
import tkinter
import document as doc
path=os.getcwd()+'/'
cur=0.0
def create_menu(window,text):
    menubar=Menu(window)
    menu1=Menu(window)
    menu1.add_command(label='保存',command=lambda:doc.save_item(text))
    menu1.add_command(label='另存为',command=lambda:doc.osave_item(text))
    menu1.add_command(label='打开',command=lambda:doc.open_item(text))
    menu1.add_command(label='退出',command=window.quit)
    menu2=Menu(window)
    #menu2.add_command(label='复制',command=doc.copy)
    #menu2.add_command(label='粘贴',command=doc.paste)
    #menu2.add_command(label='剪切',command=doc.cut)
    menu2.add_command(label='格式化',command=lambda:syn.Format(text))
    menubar.add_cascade(label='文件',menu=menu1)
    menubar.add_cascade(label='编辑',menu=menu2)
    return menubar
def add_button(window,text):
    save=ttk.Button(window,command=lambda:doc.save_item(text),text='保存')
    save_as=ttk.Button(window,command=lambda:doc.osave_item(text),text='另存为')
    open_file=ttk.Button(window,command=lambda:doc.open_item(text),text='打开')
    Format=ttk.Button(window,command=lambda:syn.Format(text),text='格式化')
    compiler=ttk.Button(window,text='编译运行',command=lambda:doc.compiler(text))
    save.grid(row=1,column=0)
    save_as.grid(row=1,column=1)
    open_file.grid(row=1,column=2)
    compiler.grid(row=1,column=3)
    Format.grid(row=1,column=4)
window=Tk()
window.iconbitmap('logo.ico')
window.title('Logo Editor')
window.geometry('600x650')
window.resizable(width=True,height=True)
text=ScrolledText(window,undo=True,tabs='20',height=40)
add_button(window,text)
text.grid(row=3,columnspan=5)
text.bind('<KeyRelease>',lambda f:syn.update(text))
text.bind('<KeyRelease-Return>',lambda f:syn.auto_indent(text))
#text.bind('<Key>',lambda f:syn.Format(text))
window.config(menu=create_menu(window,text))
window.mainloop()
