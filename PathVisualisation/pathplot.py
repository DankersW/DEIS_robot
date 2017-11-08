import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

fig = plt.figure()
ax1 = fig.add_subplot(111)

def animate(i):
    pullData = open("posData.txt","r").read()
    dataArray = pullData.split('\n')
    xar = [[],[],[],[],[]]
    yar = [[],[],[],[],[]]
    for eachLine in dataArray:
        if len(eachLine)>1:
            data = eachLine.split(',')
            xar[int(data[0])-1].append(int(data[1]))
            yar[int(data[0])-1].append(int(data[2]))
    ax1.clear()
    ax1.set_xlim(0, 100)
    ax1.set_ylim(0, 100)

    legend  = []
    for i in range(len(xar)):
        ax1.plot(xar[i],yar[i])
        if (len(xar[i])>0):
            legend.append("R" + str(i+1))

    plt.legend(legend, loc='upper left') #fix robots names auto

ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()