#!/usr/bin/python


from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg

import time
import numpy as np


app = QtGui.QApplication([])

win = pg.GraphicsWindow() 
p1 = win.addPlot(row=0, col=0) 
p2 = win.addPlot(row=1, col=0)

curve1 = p1.plot()
curve2 = p1.plot()
curve1.setPen('r')
curve2.setPen('b')

curve3 = p2.plot()
curve4 = p2.plot()
curve3.setPen('r')
curve4.setPen('b')

#curve3 = p3.plot()

readData = [0.0, 0.0, 0.0, 0.0, 0.0]
x  = np.arange(1000)
y1 = np.zeros(1000, dtype=float)
y2 = np.zeros(1000, dtype=float)
y3 = np.zeros(1000, dtype=float)
y4 = np.zeros(1000, dtype=float)

indx = 0


def update():
    global curve1, curve2, curve3, curve4, indx, y1, y2, y3, y3  # curve3,

    # function that reads data from the sensor it returns a list of 3 elements as the y-coordinates for the updating plots
    readData = np.random.rand(5)
    x[indx] = indx
    y1[indx] = readData[0]
    y2[indx] = readData[1]
    y3[indx] = readData[2]
    y4[indx] = readData[3]

    if indx == 999:
        indx = 0  
        
    indx += 1
    curve1.setData(x,y1)  # , symbolBrush=('b'))
    curve2.setData(x,y2)  # , symbolBrush=('r'))

    curve3.setData(x,y3)  # , symbolBrush=('b'))
    curve4.setData(x,y4)  # , symbolBrush=('r'))
    #curve3.setData(y3)
    app.processEvents()


timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(0)


if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_'):
        QtGui.QApplication.instance().exec_()
