# 'calculate' function is an inside-class thread and main thread use calculated self.y array
import numpy as np
import pyqtgraph as pg
import threading
import socket
from collections import deque 
import time
import pprint

UDP_IP = "10.42.0.1"
UDP_PORT = 12321

pp = pprint.PrettyPrinter(indent=4)

class FuncThread(threading.Thread):
    signal = pyqtSignal(object)

    def __init__(self,t,*a):
        self._t=t
        self._a=a 
        threading.Thread.__init__(self)
    def run(self):
        self._t(*self._a)

class MyApp():
    def __init__(self):
        self.number_of_data_steps = 100000
        self.qapp = pg.mkQApp()
        self.win = pg.GraphicsWindow()

        self.buffer = deque()

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

        #curve3 = p3.plot()

        self.total_points = 10

        self.readData = [0.0, 0.0, 0.0, 0.0, 0.0]
        self.x  = np.arange(self.total_points)
        self.y1 = np.zeros(self.total_points, dtype=float)
        self.y2 = np.zeros(self.total_points, dtype=float)
        self.y3 = np.zeros(self.total_points, dtype=float)
        self.y4 = np.zeros(self.total_points, dtype=float)

        self.indx = 0

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((UDP_IP, UDP_PORT))
 
        #calculating_thread = FuncThread(self.calculate)
        #calculating_thread.start()            

        while self.win.isVisible():   
            #readData = data_array
            #queue_e_cnt = self.buffer([])
            try:
                #print str(self.buffer)
                self.calculate()
                self.readData = self.buffer.pop() 
            except:
                self.readData = []

            if len(self.readData)==6: 
                self.x[self.indx] = int(self.readData[0])/1000.0
                self.y1[self.indx] = float(self.readData[2])
                self.y2[self.indx] = float(self.readData[3])
                self.y3[self.indx] = float(self.readData[4])
                self.y4[self.indx] = float(self.readData[5])

                if self.indx == self.total_points-1:
                    self.indx = 0  
                    
                self.indx += 1
                self.curve1.setData(self.x,self.y1)  # , symbolBrush=('b'))
                self.curve2.setData(self.x,self.y2)  # , symbolBrush=('r'))
                self.curve3.setData(self.x,self.y3)  # , symbolBrush=('b'))
                self.curve4.setData(self.x,self.y4)  # , symbolBrush=('r'))
                self.qapp.processEvents()
                print "| QT win | received message:", self.readData
                #time.sleep(1)


    def calculate(self):
        data_arr = []
        #while self.win.isVisible():   
        msg, client = self.sock.recvfrom(1024)
        data = msg[:-1]   
        data_arr=data.split(',')
        self.buffer.appendleft(data_arr)
        return data_arr
            #print str(self.buffer)
            #self.readData = data.split(',')
            #self.buffer. 
            #print "| QT calc | received message:", self.readData

            
                    
if __name__=='__main__':  
    m = MyApp()
