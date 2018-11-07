from tkinter import *
import serial


window = Tk()


def new_winF(): # new window definition
    newwin = Toplevel(window)
    newwin.geometry("300x400")

    newentry = Entry(newwin)
    newentry.pack()
    display = Label(newwin, text="Humm, see a new window !")
    display.pack()


window.title('Zeng ltd.')
window.geometry("800x400")
window.resizable(0, 0)

photo1 = PhotoImage(file="img/arrow-up.GIF")
photo2 = PhotoImage(file="img/arrow-down.GIF")
photo3 = PhotoImage(file="img/cogs.GIF")
photo4 = PhotoImage(file="img/chart-bar.GIF")

a = Button(window, image=photo1)
b = Button(window, image=photo2)
c = Button(window, image=photo3, command=new_winF)
d = Button(window, image=photo4)

a.config(width=125, height=125)
b.config(width=125, height=125)
c.config(width=125, height=125)
d.config(width=125, height=125)

a.place(relx=0.20, rely=0.30, anchor=CENTER)
b.place(relx=0.40, rely=0.30, anchor=CENTER)
c.place(relx=0.60, rely=0.30, anchor=CENTER)
d.place(relx=0.80, rely=0.30, anchor=CENTER)

window.mainloop()
