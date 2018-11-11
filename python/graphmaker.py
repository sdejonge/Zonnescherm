import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style


class MakeGraph:

    def __init__(self):
        style.use('fivethirtyeight')

        fig = plt.figure()
        ax1 = fig.add_subplot(221)

        def animate(i):
            graph_data = open('C:/Users/Kevin/PycharmProjects/Python/example.txt', 'r').read()
            lines = graph_data.split('\n')
            xs = []
            ys = []
            for line in lines:
                if len(line) > 1:
                    x, y = line.split(',')
                    xs.append(x)
                    ys.append(y)
            ax1.clear()
            ax1.plot(xs, ys)
            print(xs)
            print(ys)

        ani = animation.FuncAnimation(fig, animate, interval=1000)
        plt.show()
