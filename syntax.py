import tkinter.font as tkFont
from tkinter import *
word_list=['REPEAT','CIRCLE','ARC','TREE','PAINT','DEF','ADD','MOVE','TURN','COLOR','CLOAK','WHILE'
           ,'END','FUNC','CALL','@SIZE','@BACKGROUND','@POSITION','POS','PAINT','ARG','ARC','TREE',
           'THICK','EQ','AND','OR','XOR','NOT','NEQ','LT','LEQ','GT','GEQ','SET','SUB','ADD','MOD','MUL',
           'DIV','IF','RETURN','@FRAME','ELSE']
oper=['RETURN','DEF','REPEAT','FUNC','CALL','END','WHILE','IF','ELSE']
indent_word=['WHILE','REPEAT','FUNC','IF','ELSE']
INIT=['@SIZE','@BACKGROUND','@POSITION','@FRAME']
def update(text):
    ft = tkFont.Font(size=12,weight=tkFont.BOLD)
    fd = tkFont.Font(size=12)
    cur=text.index(INSERT)
    t=text.get(0.0,END)
    text.delete(0.0,END)
    text.insert(0.0,t[0:-1])
    text.mark_set("insert",cur)
    r=1
    sum=0
    voc=t.split()
    p=0
    posp=0
    for x in voc:
        while(t[p]=='\n' or t[p]==' ' or t[p]=='\t'):
            if(t[p]=='\n'):
                r+=1
                posp=-1
            p+=1
            posp+=1
        j=len(x)
        posj=posp+j
        j+=p
        if x in word_list or x[0].isdigit() or x[0]=='-' or x=='TRUE' or x=='FALSE':
            text.tag_add(str(sum),str(r)+'.'+str(posp),str(r)+'.'+str(posj))
            if x in oper:
                text.tag_config(str(sum),foreground='brown',font=ft)
            elif x in INIT:
                text.tag_config(str(sum),foreground='Magenta',font=fd)
            elif x[0].isdigit() or x[0]=='-' or x=='TRUE' or x=='FALSE':
                text.tag_config(str(sum),foreground='ForestGreen',font=fd)
            else:
                text.tag_config(str(sum),foreground='blue',font=fd)
            sum+=1
        p=j+1
        posp=posj+1
        if t[j]=='\n':
            r+=1
            posp=0

def Format(text):
    t=text.get(0.0,END)
    text.delete(0.0,END)
    r=1
    voc=t.split()
    p=0
    posp=0
    tab=''
    pre=''
    tt=''
    for x in voc:
        while(t[p]=='\n' or t[p]==' ' or t[p]=='\t'):
            if(t[p]=='\n'):
                r+=1
                posp=-1
            p+=1
            posp+=1
        j=len(x)
        posj=posp+j
        j+=p
        if x in word_list:
            if x=='END':
                tab=tab[0:-1]
            if pre!='END':
                tt+='\n'
                tt+=tab[:]
            if (x in indent_word) and pre!='END':
                tab+='\t'
        tt+=x+' '
        p=j+1
        posp=posj+1
        if t[j]=='\n':
            r+=1
        pre=x
    text.insert(0.0,tt[1:])
    update(text)

def auto_indent(text):
    cur=text.index(INSERT)
    t=text.get(0.0,cur)
    r=1
    voc=t.split()
    p=0
    posp=0
    tab=''
    pre=''
    tt=''
    pp=''
    for x in voc:
        pp=pre
        while(t[p]=='\n' or t[p]==' ' or t[p]=='\t'):
            if(t[p]=='\n'):
                r+=1
                posp=-1
            p+=1
            posp+=1
        j=len(x)
        posj=posp+j
        j+=p
        if x in word_list:
            if x=='END':
                tab=tab[0:-1]
            if (x in indent_word) and pre!='END':
                tab+='\t'
        p=j+1
        posp=posj+1
        if t[j]=='\n':
            r+=1
        pre=x
    cur=text.index(INSERT)
    text.insert(cur,tab)
    cur=text.index(INSERT)
    if pp in indent_word:
        text.insert(cur,'\n'+tab[0:-1]+"END "+pp)
        update(text)
    text.mark_set("insert",cur)
