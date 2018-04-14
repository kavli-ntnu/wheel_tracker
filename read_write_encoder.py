## Read write encoder
import sys,os
from datetime import datetime,timezone,timedelta
import serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import multiprocessing
from scipy.ndimage.filters import gaussian_filter1d

port = 'COM13'
max_num_values = 600
ser = serial.Serial(port, 250000, timeout=1)
start_value = int(ser.readline().decode("utf-8"))

positions = []
positions_diff = [0]

win = pg.GraphicsWindow()
win.setWindowTitle('Wheel position')
p = win.addPlot()
p_diff = win.addPlot()

# Create pens (display properties lines)
pen_curve = pg.mkPen('g', width=2, style=QtCore.Qt.SolidLine)
pen_curve_diff = pg.mkPen('w', width=2, style=QtCore.Qt.SolidLine)

p.addLegend();p_diff.addLegend()

# Create curves
curve = p.plot(np.zeros(100),pen=pen_curve,name='Abs position')
curve_diff = p_diff.plot(np.zeros(100),pen=pen_curve_diff,name='Rel position')

# Add arrow to the first plot (will indicate current position)
curvePoint = pg.CurvePoint(curve)
p.addItem(curvePoint)
text = pg.TextItem("test", anchor=(0.5, -1.0))
text.setParentItem(curvePoint)
arrow = pg.ArrowItem(angle=90,pen=2,brush=2)
arrow.setParentItem(curvePoint)


def update_plot():
    global p,curve
    if len(positions)>1:
        curve.setData(positions[-max_num_values:])
        curvePoint.setPos(positions[-1])
        text.setText('{}'.format(positions[-1]))

        # add values to the difference plot - filter a snippet with 1D gaussian
        filtered_diff = gaussian_filter1d(positions_diff[-max_num_values:],1.5,mode='nearest')
        curve_diff.setData(filtered_diff)

def update():
    line = ser.readline().decode("utf-8")
    #print('{}:   {}'.format(datetime.strftime(datetime.now(),'%H:%M:%S'),line))

    positions.append(int(line)-start_value)
    if len(positions) > 1:
        positions_diff.append(positions[-1]-positions[-2])
    # Call the update!
    update_plot()

# Attach timer and call update function every 15 ms
timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(15)


if __name__ == '__main__':
    print('Rotary encoder to .csv')
    print(datetime.strftime(datetime.now(),'%d.%h %Y | %H:%M:%S'))

    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
