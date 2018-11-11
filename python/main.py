import interface
import conn
import thread

def main():
    # maakt het object window
    interface.Window()
    thread.background(conn.Connection())


if __name__ == '__main__':
    main()
