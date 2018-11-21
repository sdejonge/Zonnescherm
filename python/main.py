import interface
import conn
from threading import Thread


def main():

    startinter = Thread(target=interface.Window)
    connstart = Thread(target=conn.Connection)

    startinter.start()
    connstart.start()

    connstart.join()
    startinter.join()


if __name__ == '__main__':
    main()
