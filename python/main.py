import interface
import conn
from multiprocessing import Process


def main():
    # maakt het object window met verschillende processen

    startinter = Process(target=interface.Window)
    connstart = Process(target=conn.Connection)

    startinter.start()
    connstart.start()

    connstart.join()
    startinter.join()


if __name__ == '__main__':
    main()
