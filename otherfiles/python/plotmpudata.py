#!/usr/bin/env python

import socket

UDP_IP = "10.42.0.1"
UDP_PORT = 12321

import matplotlib
matplotlib.use('TkAgg')
#matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

xar = []
yar = []
yar2 = []
yar3 = []
yar4 = []


fig = plt.figure()
ax1 = fig.add_subplot(2,1,1)
ax2 = ax1.twinx()
ax3 = fig.add_subplot(2,1,2)
ax4 = ax3.twinx()

line1, = ax1.plot(xar,yar, color='tab:red') 
line2, = ax2.plot(xar,yar2, color='tab:green') 
line3, = ax3.plot(xar,yar3, color='tab:orange') 
line4, = ax4.plot(xar,yar4, color='tab:blue')

# fig, ax = plt.subplots()
# line, = ax.plot(np.random.randn(100))
plt.show(block=False)
#fig.canvas.draw()

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.setblocking(0)
#sock.settimeout(0.15)
sock.bind((UDP_IP, UDP_PORT))

tstart = time.time()
num_plots = 0

def animate(i):
    global num_plots
    global tstart
    # pullData = open("sampleText.txt","r").read()
    # dataArray = pullData.split('\n')
    data = ""
    addr = 0
    data_array = []

    try:
        data, addr = sock.recvfrom(512) # buffer size is 1024 bytes 
        data = data[:-1]   
        data_array = data.split(',');
        print "received message:", data , "data: ", data_array
    except:
        print("An exception occurred")

    num_plots += 1

    if time.time()-tstart >= 1:
        print "***draw rate: ", num_plots;
        tstart=time.time();
        num_plots=0;
 
    if len(data_array)>1: 
        if float(data_array[4])>20:
            data_array[4] = 20.0

        if float(data_array[5])>20:
            data_array[5] = 20.0

        xar.append((float(data_array[0])/1000.0))
        yar.append(float(data_array[2]) )
        yar2.append(float(data_array[3]) )
        yar3.append(float(data_array[4]))
        yar4.append(float(data_array[5]))
        

        if len(xar) > 30:
            xar.pop(0) 
            yar.pop(0) 
            yar2.pop(0) 
            yar3.pop(0) 
            yar4.pop(0) 


        # line1.set_xdata(xar)
        # line1.set_ydata(yar)
        # # ax1.draw_artist(ax1.patch)
        # # ax1.draw_artist(line1)
        
        # line2.set_xdata(xar)
        # line2.set_ydata(yar2)
        # # ax2.draw_artist(ax2.patch)
        # # ax2.draw_artist(line2)


        # line3.set_xdata(xar)
        # line3.set_ydata(yar3)
        # # ax3.draw_artist(ax3.patch)
        # # ax3.draw_artist(line3)
        
        # line4.set_xdata(xar)
        # line4.set_ydata(yar4)
        # # ax4.draw_artist(ax4.patch)
        # # ax4.draw_artist(line4)


        # fig.canvas.draw_idle()
        # fig.canvas.flush_events()
        
        ax1.clear()
        ax2.clear()
        ax3.clear()
        ax4.clear() 
    
        ax1.plot(xar,yar, color='tab:red') 
        ax2.plot(xar,yar2, color='tab:green') 
        ax3.plot(xar,yar3, color='tab:orange') 
        ax4.plot(xar,yar4, color='tab:blue')
 
while True:
    #ani = animation.FuncAnimation(fig, animate, interval=1000)
    animate(0)
    #plt.grid()
    #plt.draw() # draw the plot
    plt.pause(0.000000000001) # show it for 5 seconds
    
# import sys
# import time
# from pyqtgraph.Qt import QtCore, QtGui
# import numpy as np
# import pyqtgraph as pg


# class App(QtGui.QMainWindow):
#     def __init__(self, parent=None):
#         super(App, self).__init__(parent)

#         #### Create Gui Elements ###########
#         self.mainbox = QtGui.QWidget()
#         self.setCentralWidget(self.mainbox)
#         self.mainbox.setLayout(QtGui.QVBoxLayout())

#         self.canvas = pg.GraphicsLayoutWidget()
#         self.mainbox.layout().addWidget(self.canvas)

#         self.label = QtGui.QLabel()
#         self.mainbox.layout().addWidget(self.label)

#         self.view = self.canvas.addViewBox()
#         self.view.setAspectLocked(True)
#         self.view.setRange(QtCore.QRectF(0,0, 100, 100))

#         #  image plot
#         self.img = pg.ImageItem(border='w')
#         self.view.addItem(self.img)

#         self.canvas.nextRow()
#         #  line plot
#         self.otherplot = self.canvas.addPlot()
#         self.h2 = self.otherplot.plot(pen='y')


#         #### Set Data  #####################

#         self.x = np.linspace(0,50., num=100)
#         self.X,self.Y = np.meshgrid(self.x,self.x)

#         self.counter = 0
#         self.fps = 0.
#         self.lastupdate = time.time()

#         #### Start  #####################
#         self.sock = socket.socket(socket.AF_INET, # Internet
#                         socket.SOCK_DGRAM) # UDP
#         self.sock.setblocking(0)
#         #self.sock.settimeout(0.15)
#         self.sock.bind((UDP_IP, UDP_PORT))

#         self._update()

#     def _update(self):

#         self.data = ""
#         addr = 0
        
#         try:
#             self.data, addr = self.sock.recvfrom(1024) # buffer size is 1024 bytes 
#             self.data = self.data[:-1]   
#             data_array = self.data.split(',');
#             print "received message:", self.data , "data: ", data_array
#         except :
#             print("An exception occurred")


#         self.data = np.sin(self.X/3.+self.counter/9.)*np.cos(self.Y/3.+self.counter/9.)
#         self.ydata = np.sin(self.x/3.+ self.counter/9.)

#         self.img.setImage(self.data)
#         self.h2.setData(self.ydata)

#         now = time.time()
#         dt = (now-self.lastupdate)
#         if dt <= 0:
#             dt = 0.000000000001
#         fps2 = 1.0 / dt
#         self.lastupdate = now
#         self.fps = self.fps * 0.9 + fps2 * 0.1
#         tx = 'Mean Frame Rate:  {fps:.3f} FPS'.format(fps=self.fps )
#         self.label.setText(tx)
#         QtCore.QTimer.singleShot(1, self._update)
#         self.counter += 1


# if __name__ == '__main__':

#     app = QtGui.QApplication(sys.argv)
#     thisapp = App()
#     thisapp.show()
#     sys.exit(app.exec_())