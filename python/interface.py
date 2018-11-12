from tkinter import *
import graphmaker


class Window:

    def __init__(self):

        window = Tk()
        self.startwindow(window)

    def startwindow(self,window):
        window.title('Zeng ltd.')

        window.geometry("800x400")
        window.resizable(0, 0)

        photo1 = PhotoImage(file="img/arrow-up.GIF")
        photo2 = PhotoImage(file="img/arrow-down.GIF")
        photo3 = PhotoImage(file="img/cogs.GIF")
        photo4 = PhotoImage(file="img/chart-bar.GIF")

        a = Button(window, image=photo1)
        b = Button(window, image=photo2)
        c = Button(window, image=photo3, command=self.new_win)
        d = Button(window, image=photo4, command=graphmaker.MakeGraph)

        a.config(width=125, height=125)
        b.config(width=125, height=125)
        c.config(width=125, height=125)
        d.config(width=125, height=125)

        a.place(relx=0.20, rely=0.30, anchor=CENTER)
        b.place(relx=0.40, rely=0.30, anchor=CENTER)
        c.place(relx=0.60, rely=0.30, anchor=CENTER)
        d.place(relx=0.80, rely=0.30, anchor=CENTER)

        window.mainloop()

    def new_win(self):
        # new window definition
        newwin = Toplevel()
        newwin.geometry("300x200")

        display = Label(newwin, text="oprol stand")
        display.pack()
        newentry = Entry(newwin)
        newentry.pack()

        display = Label(newwin, text="uitrol stand")
        display.pack()
        newentry = Entry(newwin)
        newentry.pack()

        save = Button(newwin, text='opslaan')
        save.pack()

