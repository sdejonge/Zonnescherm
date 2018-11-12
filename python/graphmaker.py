import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style


class MakeGraph:

    def __init__(self):
        style.use('fivethirtyeight')

        fig = plt.figure()
        ax1 = fig.add_subplot(211)
        ax2 = fig.add_subplot(212)

        def animate(i):
            graph_data = open('temp.txt','r').read()
            graph_data2 = open('licht.txt', 'r').read()
            lines = graph_data.split('\n')
            lines2 = graph_data2.split('\n')
            xs = []
            ys = []
            xs2 = []
            ys2 = []
            for line in lines:
                if len(line) > 1:
                    x, y = line.split(',')
                    xs.append(x)
                    ys.append(y)
            for line in lines2:
                if len(line) > 1:
                    x2, y2 = line.split(',')
                    xs2.append(x2)
                    ys2.append(y2)

            ax1.clear()
            ax1.plot(xs, ys)
            ax1.set_title('Temperatuur')
            ax1.set_xlabel('tijd')
            ax1.set_ylabel('Graden celcius')

            ax2.clear()
            ax2.plot(xs2, ys2)
            ax2.set_title('Lichtintensiteit')
            ax2.set_xlabel('tijd')
            ax2.set_ylabel('Lichtintensiteit')

        ani = animation.FuncAnimation(fig, animate, interval=1000)
        plt.show()


