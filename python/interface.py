from tkinter import *
import graphmaker
import conn
from threading import Thread


class Window:

    def __init__(self):

        window = Tk()
        self.startwindow(window)

    # start thread and run go_up1
    def backgrup(self):
        print('I was at up')
        thrup = Thread(target=conn.go_up1)
        thrup.start()
        thrup.join()

    # Start thread and run go_down
    def backgrdown(self):
        print('I was at down')
        thrdown = Thread(target=conn.go_down())
        thrdown.start()
        thrdown.join()

    # Creates window
    def startwindow(self,window):
        window.title('Zeng ltd.')
        # Set geometry
        window.geometry("800x400")
        window.resizable(0, 0)

        # pictures for the buttons
        photo1 = PhotoImage(file="img/arrow-up.GIF")
        photo2 = PhotoImage(file="img/arrow-down.GIF")
        photo3 = PhotoImage(file="img/cogs.GIF")
        photo4 = PhotoImage(file="img/chart-bar.GIF")

        # Buttons for interface. They call functions on click
        a = Button(window, image=photo1, command=lambda: self.backgrup())
        b = Button(window, image=photo2, command=lambda: self.backgrdown())
        c = Button(window, image=photo3, command=self.new_win)
        d = Button(window, image=photo4, command=graphmaker.MakeGraph)

        # Config for buttons
        a.config(width=125, height=125)
        b.config(width=125, height=125)
        c.config(width=125, height=125)
        d.config(width=125, height=125)

        # button position
        a.place(relx=0.20, rely=0.30, anchor=CENTER)
        b.place(relx=0.40, rely=0.30, anchor=CENTER)
        c.place(relx=0.60, rely=0.30, anchor=CENTER)
        d.place(relx=0.80, rely=0.30, anchor=CENTER)

        window.mainloop()
    # Create new window for saving the distances
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

