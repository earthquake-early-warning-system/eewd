#!/usr/bin/python


from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg

import time
import numpy as np
import socket


UDP_IP = "10.42.0.1"
UDP_PORT = 12321

app = QtGui.QApplication(['eewd'])

win = pg.GraphicsWindow() 
p1 = win.addPlot(row=0, col=0) 
p2 = win.addPlot(row=1, col=0)

curve1 = p1.plot()#title="g")
curve2 = p1.plot()#title="dB")
curve1.setPen('r')
curve2.setPen(color='g', width=3)

curve3 = p2.plot()
curve4 = p2.plot()
curve5 = p2.plot()
curve3.setPen('r')
curve4.setPen('g')
curve5.setPen('g', width=3)

#curve3 = p3.plot()

sample_cnt = 128

readData = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
x  = [] # np.arange(0, dtype=float)
y1 = [] # np.zeros(0, dtype=float)
y2 = [] # np.zeros(0, dtype=float)
y3 = [] # np.zeros(0, dtype=float)
y4 = [] # np.zeros(0, dtype=float)
y5 = []
# x = np.concatenate([x,[readData[0]]])
# y1 = np.concatenate([y1,[readData[0]]])
# y2 = np.concatenate([y2,[readData[0]]])
# y3 = np.concatenate([y3,[readData[0]]])
# y4 = np.concatenate([y4,[readData[0]]])

indx = -sample_cnt
last_time_data =0 

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))


def update():
    global curve1, curve2, curve3, curve4, indx, x, y1, y2, y3, y4, y5, last_time_data  # curve3,

    msg, client = sock.recvfrom(1024)
    data = msg[:-1]   
    readData=data.split(',')

    # function that reads data from the sensor it returns a list of 3 elements as the y-coordinates for the updating plots
    #readData = np.random.rand(5)
    # x[indx] = (readData[0]) 
    # y1[indx] = (readData[2])
    # y2[indx] = (readData[3])
    # y3[indx] = (readData[4])
    # y4[indx] = (readData[5])
    if int(readData[0]) < last_time_data :  
        x = []
        y1 = []
        y2 = []
        y3 = []
        y4 = []
        y5 = []



    last_time_data = int(readData[0])

    if len(x) < sample_cnt:

        # x = np.concatenate([x,[readData[0]]])
        # y1 = np.concatenate([y1,[readData[2]]])
        # y2 = np.concatenate([y2,[readData[3]]])
        # y3 = np.concatenate([y3,[readData[4]]])
        # y4 = np.concatenate([y4,[readData[5]]])
        x.append(readData[0])
        y1.append(readData[2])
        y2.append(readData[3])
        y3.append(readData[4])
        y4.append(readData[5])
        y5.append(readData[6])
        
    
    else:
        x[:-1] = x[1:]
        y1[:-1] = y1[1:]
        y2[:-1] = y2[1:]
        y3[:-1] = y3[1:]
        y4[:-1] = y4[1:]
        y5[:-1] = y5[1:]

        x[-1] = readData[0]
        y1[-1] = readData[2]
        y2[-1] = readData[3]
        y3[-1] = readData[4]
        y4[-1] = readData[5]
        y5[-1] = readData[6]

    if indx == sample_cnt-1:
        indx = 0  
        
    indx += 1
    #print len(x),' ', len(y1) , ' ', x
    curve1.setData(np.asarray(x).astype(np.float),np.asarray(y1).astype(np.float))  # , symbolBrush=('b'))
    #curve1.setPos(indx,0)
    curve2.setData(np.asarray(x).astype(np.float),np.asarray(y2).astype(np.float))  # , symbolBrush=('r'))
    #curve2.setPos(indx,0)

    curve3.setData(np.asarray(x).astype(np.float),np.asarray(y3).astype(np.float))  # , symbolBrush=('b'))
    curve4.setData(np.asarray(x).astype(np.float),np.asarray(y4).astype(np.float))  # , symbolBrush=('r'))
    curve5.setData(np.asarray(x).astype(np.float),np.asarray(y5).astype(np.float))
    #curve3.setData(y3)
    app.processEvents()


timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(0)


if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_'):
        QtGui.QApplication.instance().exec_()
