#!/usr/bin/env python

import SocketServer

import sys
import time
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import pyqtgraph as pg

from collections import deque

#import collections  # for the buffer


# define a subclass of UDPServer


UDP_IP = "10.42.0.1"
UDP_PORT = 12321



class MyUDPHandler(SocketServer.DatagramRequestHandler):

    def handle(self):

        # Receive a message from a client
        #global buffer

        #print("Got an UDP Message from {}".format(self.client_address[0]))
        msgRecvd = self.rfile.readline().strip()
        print("| UDP msg | The Message is {}".format(msgRecvd))
        #print(msgRecvd)
        data = msgRecvd
        buffer.appendleft(data)  # append to LEFT of buffer
        



class App(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(App, self).__init__(parent)

        #self.buffer = deque()

        #### Create Gui Elements ###########
        
        self.app = QtGui.QApplication([])

        self.win = pg.GraphicsWindow() 
        self.p1 = self.win.addPlot(row=0, col=0) 
        self.p2 = self.win.addPlot(row=1, col=0)

        self.curve1 = self.p1.plot()
        self.curve2 = self.p1.plot()
        self.curve1.setPen('r')
        self.curve2.setPen('b')

        self.curve3 = self.p2.plot()
        self.curve4 = self.p2.plot()
        self.curve3.setPen('r')
        self.curve4.setPen('b')


        #### Set Data  #####################

        #readData = [0.0, 0.0, 0.0, 0.0, 0.0]
        self.x  = np.arange(1000)
        self.y1 = np.zeros(1000, dtype=float)
        self.y2 = np.zeros(1000, dtype=float)
        self.y3 = np.zeros(1000, dtype=float)
        self.y4 = np.zeros(1000, dtype=float)

        self.indx = 0
         
        self.counter = 0
        self.fps = 0.
        self.lastupdate = time.time() 

        self._update()
    
    def _buffer(self, _buf ):
        self.buffer = _buf


    def _update(self):

        self.data = ""
        addr = 0
        #global buffer
        
        try:
            #buffer = buffer.deque()
            self.data = self.buffer.pop()#.deque()
            #self.data, addr = self.sock.recvfrom(1024) # buffer size is 1024 bytes 
            self.data = self.data[:-1]   
            data_array = self.data.split(',');
            print "| QT win | received message:", self.data , "data: ", data_array
 
            if len(data_array)==6: 
                #global curve1, curve2, curve3, curve4, indx, y1, y2, y3, y3  # curve3,

                # function that reads data from the sensor it returns a list of 3 elements as the y-coordinates for the updating plots
                readData = data_array
                self.x[self.indx] = int(readData[0])/1000.0
                self.y1[self.indx] = float(readData[2])
                self.y2[self.indx] = float(readData[3])
                self.y3[self.indx] = float(readData[4])
                self.y4[self.indx] = float(readData[5])

                if self.indx == 999:
                    self.indx = 0  
                    
                self.indx += 1
                self.curve1.setData(self.x,self.y1)  # , symbolBrush=('b'))
                self.curve2.setData(self.x,self.y2)  # , symbolBrush=('r'))

                self.curve3.setData(self.x,self.y3)  # , symbolBrush=('b'))
                self.curve4.setData(self.x,self.y4)  # , symbolBrush=('r'))
                #curve3.setData(y3)
                self.app.processEvents()




            now = time.time()
            dt = (now-self.lastupdate)
            if dt <= 0:
                dt = 0.000000000001
            fps2 = 1.0 / dt
            self.lastupdate = now
            self.fps = self.fps * 0.9 + fps2 * 0.1
            tx = 'Mean Frame Rate:  {fps:.3f} FPS'.format(fps=self.fps )
            self.label.setText(tx)
            QtCore.QTimer.singleShot(1, self._update)
            self.counter += 1

        except :
            print("An exception occurred")
 
        


if __name__ == '__main__':

    server_IP = UDP_IP
    server_port = UDP_PORT

    buf = deque()

    serverAddress = (server_IP, server_port)
    serverUDP = SocketServer.UDPServer(serverAddress, MyUDPHandler) 

    app = QtGui.QApplication(sys.argv)
    thisapp = App()
    thisapp.buffer(buf) 
    thisapp.show()
    sys.exit(app.exec_()) 

    serverUDP.serve_forever()

